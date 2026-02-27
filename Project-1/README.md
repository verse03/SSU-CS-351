# Project 1 Analysis and Reporting -Bryan Dang

## 1. Which program is fastest? Is it always the fastest?

Based on my tests, malloc.out was usually the fastest overall, especially for larger runs. For example, with OPT="-O2 -g2" and 1,000,000 blocks, malloc.out finished in 2.63 s, which was slightly faster than the other versions.

However, it was not always the fastest. In some smaller tests, alloca.out performed just as well or slightly better. So the fastest program depends on the build settings and the workload size.


## 2. Which program is slowest? Is it always the slowest?

list.out and new.out were usually the slowest in my tests, especially in the unoptimized build. For example, with the default -g build and 100,000 blocks, list.out took 0.14 s and new.out took 0.13 s, while malloc.out took only 0.04 s.

Still, the slowest program was not always the same. In larger optimized runs, alloca.out was sometimes slightly slower than the others. So there was a pattern, but not one program that was always the slowest in every test.



## 3. Was there a trend in program execution time based on the size of data in each Node? If so, what, and why?

Yes. As the amount of data stored in each Node increased, the execution time also increased.

For example, with OPT="-O2 -g2" and 100,000 blocks:

- MIN_BYTES=10 MAX_BYTES=10
  - runtimes were about 0.01 to 0.02 s
- MIN_BYTES=1000 MAX_BYTES=1000
  - runtimes increased to about 0.28 to 0.30 s

This makes sense because each node contains more bytes that must be allocated, initialized, and processed by the hash computation. More data per node means more memory traffic and more work for the CPU.



## 4. Was there a trend in program execution time based on the length of the block chain?

Yes. As the block chain length increased, execution time also increased.

Using the optimized build with larger node data:

- at 10,000 blocks, runtimes were about 0.03 to 0.04 s
- at 100,000 blocks, runtimes were about 0.28 to 0.30 s
- at 1,000,000 blocks, runtimes were about 2.63 to 2.85 s

This trend is expected because a longer linked list means more nodes must be created and more bytes must be hashed. Increasing the number of blocks increases the total amount of work.



## 5. Consider heap breaks, what's noticeable? Does increasing the stack size affect the heap? Speculate on any similarities and differences in programs?

The most noticeable thing is that alloca.out had almost no heap breaks, while the other programs had many.

### Heap break results

For 10,000 blocks:

- alloca.out = 3
- list.out = 119
- malloc.out = 117
- new.out = 119

For 100,000 blocks:

- alloca.out = 3
- list.out = 1159
- malloc.out = 1142
- new.out = 1159

This shows that alloca.out mainly uses the stack, so it does not need to grow the heap very much. In contrast, list.out, malloc.out, and new.out rely on heap allocation, so the number of heap breaks increases a lot as the number of blocks increases.

Increasing the stack size does not directly increase the heap. The stack and heap are different memory regions. Increasing the stack size only allowed alloca.out to safely use more stack memory. The heap behavior stayed mostly tied to the programs that actually allocate from the heap.

A similarity is that all versions still do the same overall job: build the list and hash the data. A major difference is where memory is allocated. alloca.out uses the stack, while the others rely much more on the heap.
