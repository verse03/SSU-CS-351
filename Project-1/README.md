# Project 1 Analysis and Reporting -Bryan Dang

## Experimental Results


## Baseline Configuration

`OPT="-O2 -g2"` `MIN_BYTES=3` `MAX_BYTES=100` `NUM_BLOCKS=100000`

### Runtime Results

| Program | Time (s) | Memory (KB) |
|---|---:|---:|
| alloca | 0.02 | 12112 |
| list | 0.03 | 14984 |
| malloc | 0.02 | 12104 |
| new | 0.03 | 14984 |



## Optimization Comparison (No Optimization)

`OPT="-g"` `MIN_BYTES=3` `MAX_BYTES=100` `NUM_BLOCKS=100000`

| Program | Time (s) | Memory (KB) |
|---|---:|---:|
| alloca | 0.05 | 23048 |
| list | 0.14 | 14976 |
| malloc | 0.04 | 12100 |
| new | 0.13 | 14976 |



## Payload Size Experiment

`OPT="-O2 -g2"`  
`NUM_BLOCKS=100000`

### Payload = 10 bytes

`MIN_BYTES=10` `MAX_BYTES=10`

| Program | Time (s) | Memory (KB) |
|---|---:|---:|
| alloca | 0.01 | 8648 |
| list | 0.02 | 10944 |
| malloc | 0.01 | 8640 |
| new | 0.02 | 10944 |

### Payload = 1000 bytes

`MIN_BYTES=1000` `MAX_BYTES=1000`

| Program | Time (s) | Memory (KB) |
|---|---:|---:|
| alloca | 0.29 | 153804 |
| list | 0.30 | 156096 |
| malloc | 0.28 | 153792 |
| new | 0.28 | 156112 |



## Chain Length Experiment

`OPT="-O2 -g2"`  
`MIN_BYTES=1000`  
`MAX_BYTES=1000`

### 10,000 Nodes

| Program | Time (s) | Memory (KB) |
|---|---:|---:|
| alloca | 0.04 | 18432 |
| list | 0.04 | 18444 |
| malloc | 0.03 | 18432 |
| new | 0.03 | 18488 |

### 100,000 Nodes

| Program | Time (s) | Memory (KB) |
|---|---:|---:|
| alloca | 0.30 | 153792 |
| list | 0.29 | 156104 |
| malloc | 0.28 | 153800 |
| new | 0.29 | 156100 |

### 1,000,000 Nodes

| Program | Time (s) | Memory (KB) |
|---|---:|---:|
| alloca | 2.85 | 1506240 |
| list | 2.79 | 1529856 |
| malloc | 2.63 | 1506240 |
| new | 2.73 | 1529864 |


## Heap Break Experiment

### 10,000 Nodes

| Program | Heap Breaks |
|---|---:|
| alloca | 3 |
| list | 119 |
| malloc | 117 |
| new | 119 |

### 100,000 Nodes

| Program | Heap Breaks |
|---|---:|
| alloca | 3 |
| list | 1159 |
| malloc | 1142 |
| new | 1159 |



## 1. Which program is fastest? Is it always the fastest?

Based on my tests, malloc.out was usually the fastest overall, especially for larger runs. For example, with OPT="-O2 -g2" and 1,000,000 blocks, malloc.out finished in 2.63 s, which was slightly faster than the other versions. However, it was not always the fastest. In some smaller tests, alloca.out performed just as well or slightly better. So the fastest program depends on the build settings and the workload size.


## 2. Which program is slowest? Is it always the slowest?

list.out and new.out were usually the slowest in my tests, especially in the unoptimized build. For example, with the default -g build and 100,000 blocks, list.out took 0.14 s and new.out took 0.13 s, while malloc.out took only 0.04s. Still, the slowest program was not always the same. In larger optimized runs, alloca.out was sometimes slightly slower than the others. So there was a pattern, but not one program that was always the slowest in every test.



## 3. Was there a trend in program execution time based on the size of data in each Node? If so, what, and why?

Yes, there was a clear trend. As the amount of data in each node increased, the runtime also increased. With MIN_BYTES=10 and MAX_BYTES=10, runtimes were only about 0.01 s to 0.02 s. When the payload increased to MIN_BYTES = 1000 and MAX_BYTES = 1000, runtimes increased to about 0.28s to 0.30s. This happened because larger payloads mean more bytes must be allocated, initialized, and hashed for every node. When the payload is small, the allocation strategy matters more because allocation overhead is a larger part of the total work. When the payload becomes large, the cost of hashing and moving the larger data dominates the runtime, so the differences between the implementations become smaller.


## 4. Was there a trend in program execution time based on the length of the block chain?

Yes. As the block chain length increased, execution time also increased.

Using the optimized build with larger node data:

- at 10,000 blocks, runtimes were about 0.03 to 0.04s
- at 100,000 blocks, runtimes were about 0.28 to 0.30s
- at 1,000,000 blocks, runtimes were about 2.63 to 2.85s

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

This shows that alloca.out mainly uses the stack, so it does not need to grow the heap very much. In contrast, list.out, malloc.out, and new.out rely on heap allocation, so the number of heap breaks increases a lot as the number of blocks increases. Increasing the stack size does not directly increase the heap. The stack and heap are different memory regions. Increasing the stack size only allowed alloca.out to safely use more stack memory. The heap behavior stayed mostly tied to the programs that actually allocate from the heap. A similarity is that all versions still do the same overall job: build the list and hash the data. A major difference is where memory is allocated. alloca.out uses the stack, while the others rely much more on the heap.


## 6. Considering either the malloc.cpp or alloca.cpp versions of the program, generate a diagram showing two nodes. Include in the diagram the relationship of the head, tail, and Node next pointers, show the size in bytes and structure of a Node that allocated six bytes of data, include the bytes pointer, and indicate using an arrow which byte in the allocated memory it points to.

Node Diagram

              head                          tail
               |                             |
               v                             v

        +-------------------+        +-------------------+
        |      Node A       |        |      Node B       |
        |-------------------|        |-------------------|
        | next ----------+  |------->| next = nullptr    |
        | size = 6       |  |        | size = 6          |
        | padding        |  |        | padding           |
        | bytes ---------|--|----+   | bytes ---------|--|----+
        +-------------------+    |   +-------------------+    |
                                 |                            |
                                 v                            v
                           +----+----+----+----+----+----+  +----+----+----+----+----+----+
                           | b0 | b1 | b2 | b3 | b4 | b5 |  | b0 | b1 | b2 | b3 | b4 | b5 |
                           +----+----+----+----+----+----+  +----+----+----+----+----+----+
                             ^
                             |
                    bytes pointer points to first byte


## 7. There's an overhead to allocating memory, initializing it, and eventually processing (in our case, hashing it). For each program, were any of these tasks the same? Which ones were different

The hashing work is the same in all four programs because they all process the same data in the list. The main difference is how memory is allocated. alloca uses the stack, while list, malloc, and new use the heap. This changes the allocation overhead, which is why their runtimes and heap break counts differ.


## 8. As the size of data in a Node increases, does the significance of allocating the node increase or decrease?

It decreases. When the data stored in each node is small, the cost of allocating memory is a larger part of the total runtime, so the allocation method matters more. As the payload gets bigger, more of the work comes from storing and hashing the data itself. That makes the allocation overhead a smaller part of the total work, so the programs perform more similarly.