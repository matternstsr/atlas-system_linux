BITS 64

section .text
global asm_memcpy

;; Function: asm_memcpy
;; Purpose: Copy n bytes from source buffer src to destination buffer dest
;; Arguments:
;;   rdi = dest (destination buffer)
;;   rsi = src (source buffer)
;;   rdx = n (number of bytes to copy)
;; Returns:
;;   rax = dest (pointer to the beginning of the destination buffer)
memcpy:
    xor rcx, rcx        ; Clear rcx (counter)
    jmp .LOOP           ; Jump to start the copying loop

.INCR:
    inc rcx             ; Increment rcx (counter)

.LOOP:
    cmp rcx, rdx        ; Compare rcx (counter) with rdx (number of bytes)
    je .END             ; If rcx == rdx, jump to end (copying finished)

    mov al, BYTE [rsi+rcx]      ; Load 1 byte from src+rcx to al
    mov BYTE [rdi+rcx], al      ; Store 1 byte from al to dest+rcx
    jmp .INCR           ; Jump to increment counter and continue copying

.END:
    mov rax, rdi        ; Move dest to rax (return value)
    ret                 ; Return from function
