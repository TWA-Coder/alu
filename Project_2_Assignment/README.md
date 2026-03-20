# Project 2 Assignment: System Calls, IPC, and Multithreading

This repository contains solutions for Project 2, focusing on essential operating system concepts including process creation, Inter-Process Communication (IPC), file I/O performance, and multithreading using Pthreads.

## Project Structure

The project is divided into four main questions:

### 1. Process Creation and IPC (Pipeline Simulation)
- **File:** `q1_pipeline.c`
- **Description:** Simulates the shell command pipeline `ps aux | grep root`. It uses `fork()` to create child processes, `pipe()` to establish communication channels, `dup2()` to redirect standard I/O, and `execvp()` to execute the commands. The parent process reads the final output and writes it to `output.txt`.

### 2. Large File Copy Performance Evaluation
- **Files:** `q2_syscalls.c`, `q2_stdio.c`, `report.md`
- **Description:** Evaluates the performance differences between using low-level system calls (`read`/`write`) and standard C library I/O (`fread`/`fwrite`) when copying large files (100MB) with a small buffer. `report.md` provides a detailed `strace` analysis and performance evaluation detailing why Standard I/O heavily outperforms system calls due to reduced context switching.

### 3. Multithreading: Prime Number Counter
- **File:** `q3_primes.c`
- **Description:** Computes the total number of prime numbers between 1 and 200,000 using multithreading. It divides the range among 16 threads (`pthread`) and uses a global mutex (`pthread_mutex_t`) to safely aggregate the local counts into a shared global variable.

### 4. Multithreading: Concurrent Keyword Search
- **Files:** `q4_search.c`, `test_q4.sh`
- **Description:** Searches for a specific keyword across multiple text files concurrently using a thread pool. A bash script (`test_q4.sh`) is provided to auto-generate dummy text files and run performance benchmarks using varying numbers of threads (2, 4, 10).

---

## Compilation and Execution

You can compile all C files using `gcc`.

### Question 1
```bash
gcc -Wall -Wextra O2 q1_pipeline.c -o q1_pipeline
./q1_pipeline
```
*Output will be saved to `output.txt`.*

### Question 2
```bash
gcc -Wall -Wextra -O2 q2_syscalls.c -o q2_syscalls
gcc -Wall -Wextra -O2 q2_stdio.c -o q2_stdio
```
*Run standard file copy tests using the generated binaries and analyze with `time` or `strace`.*

### Question 3
```bash
gcc -Wall -Wextra -O2 q3_primes.c -o q3_primes -lpthread
./q3_primes
```

### Question 4
```bash
# Automated testing and benchmarking script
chmod +x test_q4.sh
./test_q4.sh
```
*Alternatively, compile manually:*
```bash
gcc -Wall -Wextra -O2 q4_search.c -o search -lpthread
./search <keyword> <output_file> <file1> <file2> ... <num_threads>
```

---

## Technical Learnings
* **Pipes & Forks**: Building a custom pipeline involves careful file descriptor management to prevent blocking.
* **I/O Efficiency**: Buffered Standard I/O minimizes expensive context switches compared to raw syscalls.
* **Concurrency**: Using `pthread` allows parallel execution, but requires careful synchronization (e.g., mutexes) to avoid race conditions on shared resources.
