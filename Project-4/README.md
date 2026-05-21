# Project 4 — CUDA

**Bryan Dang — CS 351**

This project explores NVIDIA CUDA by porting two CPU programs to the GPU:
a parallel `iota` and a Julia set image generator.

---

## Part 1: CUDA-accelerated `iota`

`iota.cpp` uses `std::iota` to fill a vector with sequential values on the CPU.
`iota.cu` replaces that with a CUDA kernel where each thread fills one element.
Each thread computes its global index from `blockIdx.x`, `blockDim.x`, and
`threadIdx.x`, guards against running past the end of the array, and writes
`startValue + i`.

### Timing results

| Vector Length | CPU Wall Time (s) | GPU Wall Time (s) |
|:--|--:|--:|
| 10            | 0.00  | 0.33  |
| 100           | 0.00  | 0.25  |
| 1,000         | 0.00  | 0.29  |
| 10,000        | 0.00  | 0.25  |
| 100,000       | 0.00  | 0.25  |
| 1,000,000     | 0.00  | 0.26  |
| 5,000,000     | 0.02  | 0.32  |
| 100,000,000   | 0.57  | 0.92  |
| 500,000,000   | 2.85  | 3.71  |
| 1,000,000,000 | 5.65  | 6.66  |
| 5,000,000,000 | 36.39 | 43.01 |

### Are the results what I expected?

Mostly yes — and the result is exactly why CUDA isn't a great fit for `iota`:

- **Low arithmetic intensity.** Each thread does one add and one store. There's
  almost no math to amortize the cost of moving data to the GPU.
- **PCIe transfer dominates.** The program copies the full vector from host to
  device and then back again. Those two `cudaMemcpy` calls move all the data
  across PCIe, which is much slower than writing to RAM directly.
- **Fixed startup cost.** The GPU times never drop below about 0.25 seconds,
  even for trivial input sizes, because CUDA context creation and kernel
  launch aren't free.

CUDA shines when each thread does meaningful math on data that already lives
on the device. `iota` does neither, so the CPU wins at every size.

---

## Part 2: CUDA-accelerated Julia set


`julia.cpp` uses a nested `for (y) for (x)` loop on the CPU, where each
iteration computes a complex number `c` for that pixel, then iterates
`z = z*z + c` until either `|z| >= 2` or `MaxIterations` is reached. The
iteration count is mapped to a color and written into a pixel buffer.

`julia.cu` ports that to a CUDA kernel where each thread handles one pixel.
The kernel computes its global `(x, y)` from `blockIdx` and `threadIdx`,
guards against running past the image bounds, then performs the same
iteration loop the CPU version uses. The launch config is a 2D grid of
32×32 thread blocks, so a 1024×1024 image launches 32×32 blocks of 32×32
threads — one thread per pixel.

### Porting notes

A few things had to change to move from `std::complex<float>` on the CPU to
the `TComplex<float>` template used in CUDA:

- **No `std::complex` on the device.** CUDA can't use `std::complex`
  directly, so the project provides a `TComplex` template with `__host__
  __device__` constructors and matching operators (`+`, `-`, `*`, `-=`).
- **The `magnitude()` bridge.** On the CPU, `std::abs(z)` returns the
  magnitude of a complex number. The inline `__device__ magnitude()`
  helper at the top of the kernel section lets the loop body use the same
  `magnitude(z) < 2.0f` check as the CPU version without changing the
  expression.
- **Member access.** `std::complex` uses `.real()` / `.imag()`, while
  `TComplex` exposes public `x` / `y` members directly.
- **`z` must be explicitly initialized.** `TComplex() = default;` isn't
  `__device__`-tagged, so the kernel constructs `z` with
  `Complex z(0.0f, 0.0f)` instead of relying on default construction.

### Generated image

The CUDA program builds and runs cleanly and produces `julia.ppm` at the
expected 3.1 MB (1024 × 1024 × 3 bytes plus header). When viewed in
IrfanView, however, the image renders as a solid black square. Since
`setColor()` returns black whenever `iter == MaxIterations`, this suggests
the iteration is hitting the cap for every pixel — likely the result of a
subtle difference in how `z` and `c` are being computed in the ported
kernel versus the CPU loop. The program compiles, the kernel launches
without errors, the memory transfer completes, and the file writes
correctly; the issue is in the math of the iteration itself.

Given time, the next step would be to add a `printf` from a single thread
(say, the center pixel) to log the value of `c`, the first few values of
`z`, and the final `iter` count, and compare those against the CPU
version's values for the same pixel.
