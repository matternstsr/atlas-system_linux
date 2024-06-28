global asm_strncmp

section .text
asm_strncmp:
    push rbp
    mov rbp, rsp
    push rbx

    ; Start of function
    cmp rdx, 0        ; Compare n with 0
    jz .isnull        ; Jump to .isnull if n is 0
    jmp .next         ; Otherwise, jump to .next

.isnull:
    cmp byte [rdi], 0 ; Compare byte in *s1 with 0
    jl .isnega        ; Jump to .isnega if *s1 < 0
    xor rax, rax      ; Set return value to 0 (equal strings)
    jmp .end          ; Jump to .end

.isnega:
    mov rax, -1       ; Set return value to -1 (*s1 < *s2)
    jmp .end          ; Jump to .end

.ispos:
    mov rax, 1        ; Set return value to 1 (*s1 > *s2)

.next:
    movzx ebx, byte [rdi] ; Load byte from *s1 into bl
    movzx eax, byte [rsi] ; Load byte from *s2 into al
    cmp bl, 0x00       ; Compare byte in bl with 0
    jz .isnull         ; Jump to .isnull if bl is 0
    cmp bl, al         ; Compare bl and al
    jg .ispos          ; Jump to .ispos if bl > al
    jl .isnega         ; Jump to .isnega if bl < al
    inc rdi            ; Increment s1 pointer
    inc rsi            ; Increment s2 pointer
    dec rdx            ; Decrement n
    jnz .next          ; Jump to .next if n != 0

.end:
    pop rbx
    pop rbp
    ret                ; Return

