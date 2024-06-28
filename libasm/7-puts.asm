BITS 64

global asm_puts
extern asm_strlen

section .text
    ;; Prototype: size_t asm_puts(const char *str)

asm_puts:
    push rbp
    mov rbp, rsp
    push rsi
    push rdx

    ; Get the length of the string using asm_strlen
    mov rsi, rdi        ; Pass the argument (str) to asm_strlen
    call asm_strlen     ; Call asm_strlen to get the length

    ; Set up syscall to write the string to stdout
    mov rdi, 1          ; file descriptor 1 (stdout)
    mov rdx, rax        ; length of the string
    mov rax, 1          ; syscall number for write
    SYSCALL             ; invoke syscall to write to stdout

    ; Cleanup and return
    pop rbp             ; Function epilogue
    pop rdx
    ret                 ; Return number of bytes written in rax
