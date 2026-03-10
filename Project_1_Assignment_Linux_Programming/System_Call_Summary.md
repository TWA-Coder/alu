# System Call Summary Table
## Project 2: System Call Monitoring Tool

This table summarizes the system calls observed during the execution of the `monitor.c` program as analyzed through `strace`.

### 1. Process-Related System Calls
| System Call | Purpose / Interpretation |
|-------------|--------------------------|
| `execve` | Executes the binary file. It replaces the current process image with the new program image (`./monitor`). This is the starting point of the execution. |
| `getpid` | Retrieves the Process ID (PID) of the currently running process. In the monitor program, this ID is obtained to print it to the user. |
| `exit_group`| Terminates all threads in the current process and exits the program, returning the exit status (0 in our case for a successful execution). |
| `brk`, `mmap`, `mprotect` | Process memory management system calls. `brk` adjusts the size of the data segment, while `mmap` is used to load shared libraries (`libc.so.6`) and allocate memory blocks. `mprotect` configures access protections on the mapped memory arrays. |
| `arch_prctl` | Sets architecture-specific process or thread state. Used during early initialization to set up thread-local storage registers. |

### 2. File-Related System Calls
| System Call | Purpose / Interpretation |
|-------------|--------------------------|
| `openat` | Opens a file relative to a directory file descriptor (`AT_FDCWD` means current directory). In our program, it is used twice on `system_monitor_log.txt`: first with `O_WRONLY|O_CREAT|O_TRUNC` to create and open it for writing, and then with `O_RDONLY` to open it for reading. |
| `write` | Writes data to a file descriptor. In standard usage, it writes strings to standard output (file descriptor `1`). During the file operation phase, it writes the log message to the log file (file descriptor `3`). |
| `read` | Reads data from a file descriptor. In the program, it pulls the bytes stored in `system_monitor_log.txt` (via file descriptor `3`) into the program's buffer variable. |
| `close` | Closes an open file descriptor, releasing the system resource mapping so that the descriptor map point (`3` in our case) can be reused in future `openat` calls, ensuring that data buffers are properly flushed. |
| `fstat` | Retrieves status information about a file descriptor. The C Standard Library (libc) uses this behind the scenes to determine appropriate buffering strategies (e.g., checking if fd `1` is a terminal or a standard file block). |
| `access` | Checks a user's permissions for a file. Often utilized by the dynamic loader (ld.so) to verify permissions on configuration files like `/etc/ld.so.preload`. |
