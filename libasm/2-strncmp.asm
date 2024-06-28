global asm_strncmp

section .text

.asm_strncmp:
    push rbp
    mov rbp, rsp
    push rbx
    jmp .start

.next:
    movzx ebx, byte [rdi]     ; Load byte from [rdi] (first string) into ebx
    movzx eax, byte [rsi]     ; Load byte from [rsi] (second string) into eax
    cmp bl, 0x00               ; Compare byte from [rdi] to null terminator
    jz .isnull                 ; Jump to .isnull if end of first string reached
    cmp bl, al                 ; Compare bytes from [rdi] and [rsi]
    jg .ispos                  ; Jump to .ispos if byte from [rdi] > byte from [rsi]
    jl .isnega                 ; Jump to .isnega if byte from [rdi] < byte from [rsi]
    inc rdi                    ; Increment pointer of first string
    inc rsi                    ; Increment pointer of second string
    dec rdx                    ; Decrement length counter
    jnz .next                  ; Jump to .next if length counter is not zero

.isnull:
    cmp bl, al                 ; Compare bytes from [rdi] and [rsi] again at end
    jl .isnega                 ; Jump to .isnega if byte from [rdi] < byte from [rsi]
    xor rax, rax               ; Set rax to 0 (strings are equal)
    jmp .end                   ; Jump to .end

.isnega:
    mov rax, -1                ; Set rax to -1 (first string is less than second)
    jmp .end                   ; Jump to .end

.ispos:
    mov rax, 1                 ; Set rax to 1 (first string is greater than second)

.end:
    pop rbx                    ; Restore rbx
    pop rbp                    ; Restore rbp
    ret                        ; Return from function

.start:
    test rdx, rdx              ; Test if rdx (length) is zero
    jz .isnull                 ; Jump to .isnull if length is zero
    jmp .next                  ; Otherwise, start comparison loop
