BITS 64

global asm_puts
extern asm_strlen

section .text
    ;; Prototype: size_t asm_puts(const char *str)

asm_puts:
    push rbp            ; Save current base pointer (function prologue)
    mov rbp, rsp        ; Set up new base pointer

    push rsi            ; Save rsi (callee-saved register)
    push rdx            ; Save rdx (callee-saved register)

    ; Get the length of the string using asm_strlen
    mov rsi, rdi        ; Pass the argument (str) to asm_strlen
    call asm_strlen     ; Call asm_strlen to get the length

    ; Set up syscall to write the string to stdout
    mov rdi, 1          ; file descriptor 1 (stdout)
    mov rdx, rax        ; length of the string
    mov rax, 1          ; syscall number for write
    syscall             ; invoke syscall to write to stdout

    ; Cleanup and return
    pop rdx             ; Restore rdx
    pop rsi             ; Restore rsi
    pop rbp             ; Restore previous base pointer
    ret                 ; Return number of bytes written in rax
