# Summative Project Portfolio - System Internals & Linux Programming

This repository houses solutions for a comprehensive multi-phase systems programming assessment.

## Project Structure Overview
* **`/Q1` (ELF Analysis)**: C codebase and dynamic/static binary reverse engineering report.
* **`/Q2` (x86 Assembly)**: 32-bit x86 NASM parser extracting textual temperature analytics locally.
* **`/Q3` (Python C Extension)**: High-speed vibration array analyzer bypassing python inefficiencies.
* **`/Q4` (POSIX Multithreading)**: Automated luggage coordination simulation with synchronized limits.
* **`/Q5` (TCP Client-Server)**: Fully distributed concurrent Digital Library authentication/reservation architecture.

---

## Component Details & Execution Guides

### [Question 1] Reverse Engineering Internal ELF Binaries
**Core Concepts**: `malloc` allocations, Control Flow logic (loops, branches), `objdump/strace/gdb` manipulation.
* **Requirements**: Linux environment (Ubuntu WSL recommended).
* **Compilation**:
  ```bash
  cd Q1
  gcc -Wall -O0 -fno-inline -o program program.c
  strip program
  ```
* **Execution**: `./program` (Displays structural array outputs evaluating mathematical parameters).
* **Expected Output**: The program prints out an array evaluation result directly pointing to dynamically modified memory components on the HEAP. The exact behavior aligns tightly with the static analysis mappings outlined inside `q1_report.txt`.

### [Question 2] 32-Bit x86 Hardware Parsing
**Core Concepts**: Raw Linux API syscall manipulation (`int 0x80`), custom memory IO, mathematical ASCII encoding loops.
* **Requirements**: `nasm` assembler toolchain (`sudo apt install nasm gcc-multilib`).
* **Compilation**:
  ```bash
  cd Q2
  nasm -f elf32 temperature_parser.asm -o parsed.o
  ld -m elf_i386 parsed.o -o parser
  ```
* **Execution**: Create a dummy `temperature_data.txt` in the active terminal context, then execute `./parser`.
* **Expected Output**: Two integer counts evaluating precisely how many lines inside the simulated text payload contained valid textual data versus entirely blank components.

### [Question 3] Python C-API Extension (Vibration Diagnostics)
**Core Concepts**: Extending Python dynamically utilizing custom C algorithms scaling execution parameters perfectly. Includes mathematical Welford one-pass statistical stabilization strategies.
* **Compilation**:
  ```bash
  cd Q3
  python3 setup.py build_ext --inplace
  ```
* **Execution**: `python3 test_vibration.py`
* **Expected Output**: 
  - Standard payload triggers return mathematically perfect analytic calculations evaluated at O(1) C-Level structural limits.
  - Built-in crash evaluations (`test_edge_cases()`) deliberately fire custom `ValueError` handling confirming strict exception boundaries.

### [Question 4] POSIX Multithreading (Airport Luggage Coordination)
**Core Concepts**: Pure `pthread_create` multithreading heavily protected by `pthread_mutex_t` limits and integrated `pthread_cond_t` boundaries avoiding race conditions.
* **Compilation**:
  ```bash
  cd Q4
  gcc -Wall -pthread -o baggage baggage_handler.c
  ```
* **Execution**: `./baggage`
* **Expected Output**: Terminal log dynamically rolling as threads sleep/resume depending on array capacity states. Aircraft loading safely blocks behind maximum bounds. Monitoring Thread seamlessly outputs statistical tracking payloads identically matching strings generated inside `q4_sample_console_output.txt`.

### [Question 5] TCP Multi-Node Concurrency (Digital Library System)
**Core Concepts**: `libsys/socket` instantiations, `accept()/recv()` blocking manipulation tied closely to unique client-spawning thread limits protecting Central Shared Resources.
* **Compilation**: Note two separate terminal bodies must be initialized.
  ```bash
  cd Q5
  gcc -Wall -pthread -o library_server server.c
  gcc -Wall -o library_client client.c
  ```
* **Execution**: 
  1. Boot Central Terminal: `./library_server` -> Initializes TCP `127.0.0.1:8080`.
  2. Boot Custom Network Terminals: `./library_client` -> Wait for Auth Request. Auth IDs hardcoded standard inside `valid_users`: `ALU123` or `ALU456`.
* **Expected Output**: Strict synchronization limits guarantee that if Client A executes `RESERVE:1`, Client B executing the identical parameter instantly receives a `RES_REJECTED` string preventing massive structural database race conditions. Valid Auth logic controls all inbound buffers. Logs align strictly to `q5_sample_logs.txt`.
