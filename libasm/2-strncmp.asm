BITS 64

section .text
global asm_strncmp

asm_strncmp:
    xor eax, eax
    xor rcx, rcx
    jmp .loop

.loop:
    cmp rcx, rdx
    je .end

    mov bl, BYTE [rdi + rcx]
    mov r8b, BYTE [rsi + rcx]
    cmp bl, r8b
    je .continue

.notsame:
    movsx eax, bl
    movsx ebx, r8b
    sub eax, ebx
    jmp .end

.continue:
    inc rcx
    jmp .loop

.end:
    ret
