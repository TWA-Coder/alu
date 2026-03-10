# Signal Handling and Demonstration Document

## 1. How the Program Registers Signal Handlers
The C program (`monitor_service.c`) registers signal handlers using the `sigaction()` system call. Unlike the older `signal()` function, `sigaction` provides a more robust and portable way to manage signals in POSIX-compliant operating systems like Linux. 

In the `main` function:
1. We define a `struct sigaction` and set its `sa_handler` attribute to our custom callback function called `handle_signal(int sig)`.
2. We initialize the signal mask using `sigemptyset()` to ensure no other signals are blocked while the handler is executing.
3. We add the `SA_RESTART` flag so that if a slow system call is interrupted by our signal, it will automatically restart instead of throwing an `EINTR` error.
4. We bind specific signals (`SIGINT`, `SIGUSR1`, and `SIGTERM`) to this structure using repeated `sigaction()` calls.

When the operating system sends one of these signals to the process, execution jumps from the infinite loop to `handle_signal`, processes the instructions, and normally returns (unless `exit()` is called).

## 2. What Happens When Each Signal is Received

Inside `handle_signal(int sig)`, the code evaluates which signal was caught:

*   **`SIGINT` (Interrupt):** Standardly sent when a user presses `Ctrl+C` in the foreground terminal. The program catches it, prints *"Monitor Service shutting down safely"* to stdout, and calls `exit(0)` to cleanly terminate itself.
*   **`SIGUSR1` (User Signal 1):** A custom signal defined by POSIX for application use. When trapped (usually via the `kill` command), it prints *"System status requested by administrator."* Because `exit()` is not called here, the background service continues monitoring after the printf without terminating.
*   **`SIGTERM` (Termination):** The default signal sent by the `kill` command to politely request a program to stop. The program catches it, prints *"Emergency shutdown signal received."*, and safely ends the process execution via `exit(1)`.

## 3. Compilation Command Used
To compile the C code, standard GCC was used:
```bash
gcc monitor_service.c -o monitor_service
```

## 4. Terminal Demonstration Log Output
Below is a simulated terminal session acting as a demonstration snippet showing what happens when the admin sends the desired signals.

**Terminal 1 (Running the Application):**
```bash
$ ./monitor_service
Starting Monitor Service (PID: 7421)...
Waiting for signals. Try sending SIGINT (Ctrl+C), SIGUSR1, or SIGTERM.

[Monitor Service] System running normally...
[Monitor Service] System running normally...

System status requested by administrator.
[Monitor Service] System running normally...
[Monitor Service] System running normally...

Emergency shutdown signal received.
$
```

**Terminal 2 (Admin sending `kill` signals from another prompt):**
```bash
# Sending SIGUSR1 to request status report
$ kill -SIGUSR1 7421

# Waiting 10 seconds, then sending SIGTERM to kill it
$ kill -SIGTERM 7421
```

*(Note: Had we utilized `Ctrl+C` in Terminal 1, the script would have caught SIGINT instead of SIGTERM).*
