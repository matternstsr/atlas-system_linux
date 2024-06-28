global asm_strncmp

section .text

.asm_strncmp:
    push rbp
    mov rbp, rsp
    push rbx
    jmp .start

.start:
    test rdx, rdx
    jz .isnull
    jmp .next

.isnull:
    cmp bl, al
    jl .isnega
    xor rax, rax
    jmp .end

.isnega:
    mov rax, -1
    jmp .end

.ispos:
    mov rax, 1

.next:
    movzx ebx, byte [rdi]
    movzx eax, byte [rsi]
    cmp bl, 0x00
    jz .isnull
    cmp bl, al
    jg .ispos
    jl .isnega
    inc rdi
    inc rsi
    dec rdx
    jnz .next

.end:
    pop rbx
    pop rbp
    ret
