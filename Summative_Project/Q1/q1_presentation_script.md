# Presentation Script: Q1 - Reverse Engineering Internal ELF Binaries

## Introduction
Hello everyone. Thank you for joining me. Today, I am excited to walk you through the first component of my Systems Programming summative project. This phase focuses on reverse engineering internal ELF binaries. 

For those who might be new to the term, ELF stands for Executable and Linkable Format. It's the standard file format for executables, object code, shared libraries, and even core dumps in Linux. Our main objective for this phase was to take a compiled C codebase—specifically an ELF executable—and deeply analyze its internal workings, primarily focusing on understanding its execution flow, memory allocations, and how static versus dynamic binary components interact during runtime. 

In this presentation, I'll briefly outline the key concepts involved, the tools I used, and guide you through an example of what I uncovered regarding the program's control flow and memory behavior. 

## Key Concepts and Tools
To really understand an ELF binary without looking directly at its C source code, we rely on a process called Reverse Engineering. Reverse engineering involves taking the compiled machine code and systematically analyzing it to reconstruct the original logic. 

To accomplish this, I relied on three primary utilities: `objdump`, `strace`, and `gdb`.
- **`objdump`**: I used this for static analysis. By disassembling the binary using `objdump`, I could read the raw assembly instructions and map out the binary’s control flow, identifying loops, conditional branches, and function calls. 
- **`strace`**: This tool is an absolute powerhouse. It allows us to perform dynamic analysis by tracing the system calls and signals as the program runs. It let me see precisely how the program interacts with the Linux kernel in real-time.
- **`gdb`** (The GNU Debugger): Finally, I used `gdb` to step through the program's execution instruction by instruction, inspecting register values and memory addresses at runtime. 

A critical concept we observed is the difference between static and dynamic linking. With dynamic linking, the binary relies on shared libraries like `libc.so` at runtime. Exploring the binary with `strace` revealed how the operating system resolves these shared dependencies automatically via the ELF's dynamic section. 

## Code Specifics and Examples
Let’s get into the specifics of what the target application does. The provided binary evaluates mathematical parameters through a structural array. A central part of this evaluation requires dynamic memory allocation, which is where `malloc` comes in. 

Through my static and dynamic analysis, I discovered that the binary actively allocates memory on the heap. Using `gdb`, I could watch the heap space expand as `malloc` requests were fulfilled. For instance, breaking execution right after the reallocation showed the precise addresses being served up by the OS. 

Furthermore, I traced control flow logic involving tight mathematical loops and branches. The underlying assembly highlighted operations where array indices were evaluated against environmental constraints using conditional jumps (like `jne` or `je` in x86). If a certain threshold is met, the program jumps to an evaluation routine; if not, it continues looping. 

Once executed directly, the program prints out the array evaluation results, but these aren't just predefined strings. They are generated based on the values pulled directly from dynamically modified memory components on the Heap. The exact behaviors, including memory boundaries and control shifts, matched entirely with the static analysis mappings I documented in my project report. Stripping the binary using the `strip` command removed all debug symbols, simulating a real-world scenario where a binary needs to be evaluated opaquely. This proved the robustness of utilizing tools like `strace` and `gdb` to glean insights without symbol tables.

## Conclusion
In conclusion, reverse engineering this ELF binary provided an incredibly clear lens into how high-level C code translates to low-level assembly and interacts with the Linux operating system. We successfully mapped out memory allocations, tracked control flow loops, and proved our understanding through dynamic and static tracing. 

These analytical skills are vital, whether for malware analysis, optimizing legacy code, or simply deepening one's understanding of system architecture. 

Thank you for your time. I'll now transition to the next project or open the floor for questions.
