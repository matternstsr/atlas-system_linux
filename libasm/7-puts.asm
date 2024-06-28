section .text
global asm_puts

;; Prototype: size_t asm_puts(const char *str)
asm_puts:
    push rbp
    mov rbp, rsp
    push rdx
    push rsi

    ; Get the length of the string using asm_strlen
    mov rsi, rdi        ; Pass the argument (str) to asm_strlen
    call asm_strlen     ; Call asm_strlen to get the length

    ; Set up syscall to write the string to stdout
    mov rax, 1          ; syscall number for write
    mov rdx, rax        ; pointer to the string (already in rdi)
    mov rdi, 1          ; file descriptor 1 (stdout)
    syscall             ; invoke syscall to write to stdout

    ; Cleanup and return
    pop rbp             ; Function epilogue
    ret                 ; Return number of bytes written in rax

extern asm_strlen
