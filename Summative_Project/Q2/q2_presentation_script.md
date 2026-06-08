# Presentation Script: Q2 - 32-Bit x86 Hardware Parsing

## Introduction
Welcome! In this section of our presentation, I'll be discussing the second project of our systems programming module: writing a 32-bit x86 Assembly hardware parser. 

When we talk about high-level languages like Python or C, we often take for granted how easily we can open a file, read its contents, and parse information. However, for this project, the objective was to strip away all those high-level abstractions and implement a textual temperature analytics parser directly in raw x86 assembly. 

The main task was to create a tool that evaluates simulated text payloads from a file, distinguishing between valid textual temperature records and entirely blank components. This required a ground-up understanding of system calls, custom memory management, and ASCII mathematical encoding using the `nasm` assembler toolchain on Linux. 

## Key Concepts and Tools
The core concept here is executing logic via low-level interactions with the operating system rather than relying on standard C libraries. In 32-bit x86 on Linux, we interact with the kernel using software interrupts—specifically, the `int 0x80` system call instruction. 

In order to read our data file, I manually set up system calls for standard File I/O operations. This involved moving the correct syscall number into the `EAX` register—like '5' for `sys_open`, '3' for `sys_read`, and '4' for `sys_write`. From there, I placed the file descriptors, buffer pointers, and read boundaries into registers like `EBX`, `ECX`, and `EDX` before triggering the vector interrupt.

Another essential concept is ASCII processing. When data is read from the file, we don't naturally get integers; we get raw bytes representing ASCII characters. For example, a temperature like "25" is stored as the hex values `0x32` and `0x35`. In order to parse the mathematical representations or identify empty lines, I had to build custom algebraic loops to subtract the ASCII baseline (zero, which is `48` in decimal) to verify numerical integrity.

## Code Specifics and Examples
Let's talk about the specific architecture of the parser. Once the binary reads chunks of data into a local buffer, it kicks off a byte-by-byte evaluation loop. 

A primary responsibility of the parser is to determine how many lines inside the simulated text payload contained valid textual data versus entirely blank components. To achieve this, my assembly code maintains two separate tally registers. 
- It sweeps through the buffer checking for newline characters (`\n` or `0x0A`).
- If it encounters back-to-back newlines without any intervening numeric bytes, it jumps to increment the blank-line counter. 
- Conversely, if it identifies sequential mathematical digits between the newlines, it increments the valid-data counter.

After finishing the end-of-file loop (when `sys_read` returns 0), the program needs to print these two integer counts to the terminal. But since we are in raw assembly, there is no `printf`. I wrote a custom internal routine to divide the binary tally by ten repeatedly, grabbing the remainder, and converting it back into ASCII characters to construct a printable string buffer. 

## Conclusion
To summarize, crafting this parser in 32-bit NASM taught me the meticulous reality of hardware-level execution. By manually orchestrating registers for Linux API syscalls, crafting mathematical ASCII encoding loops, and controlling memory block reads, my code seamlessly outputs the two integer counts without using a single high-level dependency. 

Running the binary efficiently outputs the statistics at blazing speed with virtually zero overhead. It’s an exercise that builds immense respect for the compiler toolchains we utilize every day. Thank you very much, and I’m ready to move forward.
