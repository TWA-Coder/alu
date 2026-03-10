# Performance Analysis: Pure Python vs C Extension

## Benchmark Scenario
The benchmark focuses on a heavy numeric computation: calculating the sum of squares from 1 to 50,000,000 (`50M`). The exact same mathematical logic is implemented in two ways:
1. **Pure Python** (`benchmark.py` using a standard `for` loop).
2. **C Extension Module** (`fast_math_ext.c` built using `setup.py` and the CPython API).

## Typical Expected Performance Differences
When compiled and run on a modern CPU, the benchmarking output generally looks like this:

- **Pure Python Time:** ~2.0000 - 3.5000 seconds
- **C Extension Time:** ~0.0400 - 0.0900 seconds
- **Speedup:** The **C extension runs about 35x to 50x faster** than the pure Python implementation.

## Why is the C Extension Faster? (Performance Explanation)

The significant performance gap is due to fundamental differences in how Python and C handle instructions and memory.

### 1. The Interpreter vs. Native Machine Code
**Python** is an interpreted language. When Python executes the `for` loop, it doesn't just do math. It executes a sequence of bytecode instructions (`SETUP_LOOP`, `LOAD_FAST`, `BINARY_MULTIPLY`, `INPLACE_ADD`, etc.). For every iteration, the Python interpreter has to decode the instruction and route it. 
**C**, on the other hand, is compiled ahead of time into native machine code by a compiler (like GCC or MSVC). The loop directly maps to CPU-level hardware instructions, entirely bypassing any interpreter middleman.

### 2. Dynamic Typing vs. Static Typing
In **Python**, variables don't have fixed types—objects have types. During the line `total += i * i`, Python must dynamically check if `i` is an integer/float and what operator overload is mapped to multiplication, every single time it iterates. 
In **C**, variables are statically typed (`long long` and `double`). The compiler knows exactly how much memory they consume and what math operations to perform at compile time.

### 3. Object Allocation Overhead (Reference Counting)
In **Python**, numbers are objects. A Python `int` object encapsulates more than just the numeric value; it includes a reference count and type information. Every addition essentially forces Python to discard the old object and allocate/create a brand-new integer object in memory. Repeated 50 million times, memory allocation acts as a severe bottleneck.
In **C**, primitive types (`long long`, `double`) are managed in hardware registries or directly on the stack. No heap allocation, garbage collection, or reference counting occurs during the calculation.

### 4. Compiler Optimizations
A **C compiler** is highly advanced. Upon inspecting the C loop, it can perform Loop Unrolling or vectorize the math using SIMD (Single Instruction, Multiple Data) CPU features, effectively calculating multiple squares concurrently. **Python's** interpreter executes logic strictly sequentially and lacks the capability to optimize the loop in this manner.

---

### How to Reproduce
1. In your terminal, navigate to the `Project 1_Assignment_Linux_Programming` folder.
2. Compile the extension by running: `python setup.py build_ext --inplace`
3. Run the benchmark: `python benchmark.py`
