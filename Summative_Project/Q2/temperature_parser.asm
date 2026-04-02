; temperature_parser.asm
; 32-bit x86 Assembly Program using NASM syntax for Linux (syscalls via int 0x80)
; Parses a file named "temperature_data.txt", counts total lines and valid lines.
; Handles varied line-endings (\n and \r\n).

section .data
    filename db "temperature_data.txt", 0
    err_msg db "Error: Could not open temperature_data.txt", 10
    err_len equ $ - err_msg

    str_total db "Total readings: ", 0
    len_total equ $ - str_total
    
    str_valid db "Valid readings: ", 0
    len_valid equ $ - str_valid

    newline db 10

section .bss
    fd resd 1               ; file descriptor
    buffer resb 1024        ; 1KB generic buffer for reading chunks
    
    num_buffer resb 16      ; Temp string buffer for Integer-to-ASCII string conversions
    
    total_lines resd 1      ; memory holding total count
    valid_lines resd 1      ; memory holding valid count
    has_chars resd 1        ; boolean toggle tracking if the current line holds valid char data

section .text
    global _start

_start:
    ; 1. Initialize variables
    mov dword [total_lines], 0
    mov dword [valid_lines], 0
    mov dword [has_chars], 0

    ; 2. Open the file in Read-Only mode
    mov eax, 5              ; syscall 5: sys_open
    mov ebx, filename       ; pointer to string "temperature_data.txt"
    mov ecx, 0              ; O_RDONLY (0)
    int 0x80                ; Execute system call
    
    ; Check if file opened successfully
    test eax, eax
    js open_failure         ; if the Sign Flag is set (eax is negative), jump to error handler
    
    mov [fd], eax           ; Save the file descriptor returned by the system

    ; 3. Buffered Reading Loop
read_loop:
    mov eax, 3              ; syscall 3: sys_read
    mov ebx, [fd]           ; file descriptor to read from
    mov ecx, buffer         ; write destination in `.bss` memory
    mov edx, 1024           ; specify max bytes to read mapping buffer size
    int 0x80

    ; Evaluate read status
    test eax, eax
    jz end_of_file          ; If eax is 0, we reached EOF. Exit loop.
    js read_loop            ; If negative (should mathematically handle with graceful fail, repeating here for simplicity safety)
    
    ; eax now holds actual number of bytes read
    mov ecx, eax            ; ecx becomes our loop counter (length of chunk)
    mov esi, buffer         ; esi points to the starting address of buffer data

process_chunk:
    mov al, byte [esi]      ; load exactly one character into 8-bit register AL

    ; Checking for CRLF / LF line endings
    cmp al, 10              ; compare character against '\n' (Line Feed, ASCII 10)
    je handle_newline       
    cmp al, 13              ; compare character against '\r' (Carriage Return, ASCII 13)
    je iterate_next_char    ; skip processing it entirely to neutrally handle CR + LF together

    ; Since it isn't \n or \r, it's considered valid data
    mov dword [has_chars], 1 
    jmp iterate_next_char

handle_newline:
    ; We arrived at the end of a line tracking block
    inc dword [total_lines] ; Increment overall line tracking unconditionally

    ; Check if the line we just finished actually contained real characters
    cmp dword [has_chars], 0
    je reset_flag           ; If 0, it was purely empty line. Skip to reset flag.
    
    inc dword [valid_lines] ; Line wasn't empty, so it's a valid reading!

reset_flag:
    mov dword [has_chars], 0 ; Clear flag for the upcoming new line

iterate_next_char:
    inc esi                 ; Move to next byte pointer
    dec ecx                 ; Decrement counter of untouched chunk bytes
    jnz process_chunk       ; Keep looping internal characters until ecx == 0
    
    jmp read_loop           ; Ask the system to dump the next file chunk into buffer

    ; 4. End of File handling
end_of_file:
    ; Edge case scenario handling:
    ; What if the file abruptly ended without a trailing newline character (\n)?
    ; If `has_chars` is 1, a final line exists that hasn't been committed yet!
    cmp dword [has_chars], 0
    je close_file
    
    inc dword [total_lines]
    inc dword [valid_lines]

close_file:
    ; 5. Close the file descriptor
    mov eax, 6              ; syscall 6: sys_close
    mov ebx, [fd]           ; file descriptor variable
    int 0x80

    ; 6. Print Total Readings
    ; Print "Total readings: "
    mov eax, 4              ; syscall 4: sys_write
    mov ebx, 1              ; stdout (1)
    mov ecx, str_total      ; string pointer
    mov edx, len_total      ; string length
    int 0x80
    
    ; Convert total_lines to ascii, then print
    mov eax, [total_lines]
    call print_integer
    
    ; Print "Valid readings: "
    mov eax, 4
    mov ebx, 1
    mov ecx, str_valid
    mov edx, len_valid
    int 0x80
    
    ; Convert valid_lines to ascii, then print
    mov eax, [valid_lines]
    call print_integer

    ; 7. Terminate Default
    jmp graceful_exit

    ; ---------------------------
    ; Error handler logic block
open_failure:
    mov eax, 4              ; write
    mov ebx, 1              ; stdout
    mov ecx, err_msg        ; file missed err string 
    mov edx, err_len      
    int 0x80

graceful_exit:
    mov eax, 1              ; sys_exit syscall
    xor ebx, ebx            ; Set return code 0
    int 0x80

    ; ---------------------------
    ; Subroutine: print_integer
    ; Converts int loaded in eax to ASCII string and executes sys_write
    ; Modifies: eax, ebx, ecx, edx, edi
print_integer:
    mov edi, num_buffer + 15 ; Point edi to the end of our small allocated string space
    mov byte [edi], 10       ; Embed local newline char '\n' at very end
    
    mov ebx, 10              ; Set divisor to 10 for modulo extraction iteration

.convert_loop:
    xor edx, edx             ; Clear remainders mapping register 
    div ebx                  ; divides (edx:eax) by ebx (10). Result in eax, Remainder in edx
    add dl, '0'              ; ASCII encoding offset ('0' == 0x30)
    dec edi                  ; Drop one index backwards onto buffer stack
    mov [edi], dl            ; Write stringified character
    
    test eax, eax            ; Validate if base numeral quotient is finally 0 
    jnz .convert_loop        ; Loop otherwise

    ; Print converted string representation
    mov eax, 4               ; sys_write
    mov ebx, 1               ; stdout
    mov ecx, edi             ; Pointer pointing directly to the beginning of populated chars
    
    ; Calculate exact printed length iteratively generated backwards (buffer_end - cur_ptr)
    mov edx, num_buffer + 16 
    sub edx, edi             ; difference serves as length parameter
    
    int 0x80
    
    ret                      ; Return to main flow execution
