global asm_strcmp
section .text

asm_strcmp:
    push rbp
    mov rbp, rsp

    ; Arguments:
    ; rdi = first string (S1)
    ; rsi = second string (S2)

    ; Initialize return value to 0
    xor eax, eax

    .compare_loop:
        ; Load bytes from S1 and S2
        mov al, byte [rdi]
        mov dl, byte [rsi]

        ; Compare bytes
        cmp al, dl
        jne .not_equal

        ; Check if end of S1 ('\0')
        test al, al
        jz .strings_equal

        ; Move to next characters
        inc rdi
        inc rsi

        ; Continue loop
        jmp .compare_loop

    .not_equal:
        ; Calculate the difference
        sub al, dl
        jmp .exit

    .strings_equal:
        ; Both strings are equal, check for termination
        test dl, dl
        jz .exit

        ; S1 is shorter than S2
        mov eax, -1
        jmp .exit

    .exit:
        pop rbp
        ret
