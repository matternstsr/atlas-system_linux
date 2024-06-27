global asm_strncmp
section .text

asm_strncmp:
    push rbp
    mov rbp, rsp

    ; Arguments:
    ; rdi = first string (S1)
    ; rsi = second string (S2)
    xor eax, eax   ; Initialize eax (return value) to 0

.asm_strncmp_loop:
    ; Load byte from S1 and S2
    mov al, byte [rdi]
    mov dl, byte [rsi]

    ; Check if both characters are null
    test al, al
    jz .end_of_s1
    test dl, dl
    jz .end_of_s2

    ; Compare bytes
    cmp al, dl
    jl .end_of_s1
    jg .end_of_s2

    ; Characters are equal, move to next
    inc rdi
    inc rsi
    jmp .asm_strncmp_loop

.end_of_s1:
    ; End of S1 reached
    test dl, dl
    jz .equal_and_end  ; Both are null, strings are equal
    mov eax, -1       ; S1 is null, S2 is not null
    jmp .exit

.end_of_s2:
    ; End of S2 reached
    mov eax, 1        ; S1 is not null, S2 is null
    jmp .exit

.equal_and_end:
    ; Both strings are null
    xor eax, eax      ; Return 0 (strings are equal)

.exit:
    pop rbp
    ret
