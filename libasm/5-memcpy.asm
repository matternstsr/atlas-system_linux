section .text
global asm_memcpy

asm_memcpy:
    push rbx            ; Preserve rbx
    mov rbx, rdx        ; Store n in rbx (we will modify rdx)
    
    test rbx, rbx       ; Check if n == 0
    je .finish            ; If n == 0, nothing to copy
    
    cld                 ; Clear direction flag (ensure we copy forward)

.copy_loop:
    cmp rbx, 0          ; Check if n == 0
    je .finish            ; If n == 0, exit loop
    
    mov rax, [rsi]      ; Load 8 bytes from src to rax
    mov [rdi], rax      ; Store 8 bytes from rax to dest
    
    add rdi, 8          ; Move dest pointer forward by 8 bytes
    add rsi, 8          ; Move src pointer forward by 8 bytes
    sub rbx, 8          ; Decrement n by 8
    
    jmp .copy_loop      ; Repeat until all bytes copied

.finish:
    pop rbx             ; Restore rbx
    mov rax, rdi        ; Return dest (rax holds return value)
    ret
