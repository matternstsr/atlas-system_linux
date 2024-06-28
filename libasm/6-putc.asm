section .text
global asm_putc

;; Function: asm_putc
;; Purpose: Print a single character to standard output and return the number of bytes written
;; Arguments:
;;   rdi = c (character to be printed, passed in the lower 8 bits of rdi)
;; Returns:
;;   rax = number of bytes written (always 1 in this case)

asm_putc:
    ; Move character from rdi to rsi
    mov rsi, rdi

    ; Prepare arguments for write call
    mov rax, 1           ; syscall number for write
    mov rdi, 1           ; file descriptor 1 (stdout)
    lea rdx, [rsi]       ; pointer to the character to print
    mov BYTE [rdx], dil  ; move the character to the lower byte of rdx
    mov rdx, 1           ; number of bytes to write (just 1 byte)

    syscall              ; invoke syscall to write to stdout

    ; Return 1 byte written (always 1 for a single character)
    mov rax, 1
    ret
