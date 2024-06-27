global asm_strcmp
section .text

asm_strcmp:
    push rbp
    mov rbp, rsp

    ; Arguments:
    ; rdi = first string (S1)
    ; rsi = second string (S2)

    xor eax, eax   ; Initialize eax (return value) to 0

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
    jne .not_equal

    ; Characters are equal, move to next
    inc rdi
    inc rsi
    mov eax, 1    ; Return 1 (characters are equal)
    jmp .exit

.not_equal:
    ; Characters are not equal
    sub al, dl
    movsx eax, al  ; Sign-extend difference to 32-bit
    jmp .exit

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
