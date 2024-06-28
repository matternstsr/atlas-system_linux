section .text
global asm_puts

;; Prototype: size_t asm_puts(const char *str)
asm_puts:
    push rbp            ; Function prologue
    mov rbp, rsp

    ; Get the length of the string using asm_strlen
    mov rdi, rdi        ; Pass the argument (str) to asm_strlen
    call asm_strlen     ; Call asm_strlen to get the length
    mov rdx, rax        ; Store the length in rdx for syscall

    ; Set up syscall to write the string to stdout
    mov rax, 1          ; syscall number for write
    mov rdi, 1          ; file descriptor 1 (stdout)
    mov rsi, rdi        ; pointer to the string (already in rdi)

    ; Loop through the string to handle special characters
    mov rcx, rdx        ; Use rcx as loop counter
    mov rdx, 0          ; Clear rdx for byte count

.loop:
    lodsb               ; Load byte from rsi into al, increment rsi
    test al, al         ; Check for end of string
    jz .end             ; Jump to end if null terminator

    ; Handle special characters
    cmp al, '\t'        ; Check for tab character
    je .print_tab
    cmp al, '\n'        ; Check for newline character
    je .print_newline

    ; Normal character: store in buffer to print in one syscall
    stosb               ; Store byte from al into rdi, increment rdi
    inc rdx             ; Increment byte count
    jmp .continue

.print_tab:
    mov word [rdi], 0x0920  ; Place tab character followed by a space in rdi
    add rdi, 2          ; Increment rdi by 2 to skip past the stored characters
    add rdx, 2          ; Add 2 to the byte count
    jmp .continue

.print_newline:
    mov word [rdi], 0x0A20 ; Place newline character followed by a space in rdi
    add rdi, 2          ; Increment rdi by 2 to skip past the stored characters
    add rdx, 2          ; Add 2 to the byte count
    jmp .continue

.continue:
    loop .loop          ; Loop to process next character
.end:
    ; Perform syscall to write the entire processed string at once
    syscall             ; Invoke syscall to write characters to stdout

    ; Cleanup and return total bytes written
    pop rbp             ; Function epilogue
    ret                 ; Return number of bytes written in rax

extern asm_strlen
