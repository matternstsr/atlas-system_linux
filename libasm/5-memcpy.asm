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
asm_memcpy:
    push rbx            ; Preserve rbx
    mov rbx, rdx        ; Store n in rbx 
    
    test rbx, rbx       ; Check if n == 0
    je .finish            ; If n == 0, nothing to copy
    
    cld                 ; Clear direction flag 

.copy_loop:
    cmp rbx, 0          ; Check if n == 0
    je .finish            ; If n == 0, exit loop
    
    mov al, [rsi]       ; Load 1 byte from src to al
    mov [rdi], al       ; Store 1 byte from al to dest
    
    inc rdi             ; Move dest pointer forward by 1 byte
    inc rsi             ; Move src pointer forward by 1 byte
    dec rbx             ; Decrement n by 1
    
    jmp .copy_loop      ; Repeat until all bytes copied

.finish:
    pop rbx             ; Restore rbx
    mov rax, rdi        ; Return dest (rax holds return value)
    ret
