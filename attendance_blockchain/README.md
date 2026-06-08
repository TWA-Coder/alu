# Blockchain Attendance System

This is a C-based attendance system that uses a blockchain data structure to ensure attendance records are immutable and cryptographically verifiable.

## Features
- **Student Registry Validation**: Loads valid students from `students.txt` at startup.
- **Cryptographic Hashing**: Uses SHA-256 to hash block contents and link blocks.
- **Digital Signatures**: Uses ECDSA to authenticate attendance marking.
- **Tamper Detection**: Validates the entire chain to detect any forged data, broken links, or invalid signatures.
- **CLI Interface**: Easy-to-use menu to mark attendance, view records, and demonstrate tampering.

## Prerequisites
- A C Compiler (e.g., GCC via MinGW/MSYS2 on Windows, or MSVC)
- **OpenSSL Library**: Required for SHA-256 and ECDSA functions.

## Compilation Instructions (Windows with MSYS2/MinGW)

1. Open your MSYS2 MinGW 64-bit terminal.
2. Install GCC and OpenSSL if you haven't already:
   ```bash
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-openssl
   ```
3. Compile the application linking the OpenSSL libraries:
   ```bash
   gcc main.c registry.c blockchain.c crypto.c -o attendance_app.exe -lssl -lcrypto
   ```

## Running the Application

1. Ensure `students.txt` is in the same directory as the executable.
2. Run the executable:
   ```bash
   ./attendance_app.exe
   ```
3. Follow the on-screen menu to test the system. Use IDs like `ALU001`, `ALU002` from `students.txt`.
