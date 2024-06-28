section .text
global asm_putc

;; size_t asm_putc(int c)
asm_putc:
    push rbp              ; Function prologue
    mov rbp, rsp

    push rdi              ; Save rdi (character to print)

    ; Set up syscall to write character to stdout
    mov rdi, 1            ; file descriptor 1 (stdout)
    mov rsi, rsp          ; pointer to the character to print
    mov BYTE [rsi], dil   ; move the character to the lower byte of rsi
    mov rdx, 1            ; number of bytes to write (just 1 byte)
    mov rax, 1            ; syscall number for write

    syscall               ; invoke syscall to write to stdout

    ; Cleanup and return
    pop rdi               ; Restore rdi
    pop rbp               ; Function epilogue
    ret