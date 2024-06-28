section .text
global asm_putc

;; Function: asm_putc
;; Purpose: Print a single character to standard output and return the number of bytes written
;; Arguments:
;;   rdi = c (character to be printed, passed in the lower 8 bits of rdi)
;; Returns:
;;   rax = number of bytes written (always 1 in this case)

asm_putc:
    ; Prepare arguments for write call
    mov rax, 1          ; syscall number for write
    mov rdi, 1          ; file descriptor 1 (stdout)
    mov rsi, rsp        ; pointer to the character to print (in this case, in the lower 8 bits of rdi)
    mov BYTE [rsp], dil ; move character to the lower byte of rsi
    mov rdx, 1          ; number of bytes to write (just 1 byte)
    
.LOOP:
    syscall             
    cmp eax
