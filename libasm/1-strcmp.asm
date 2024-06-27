global asm_strcmp
section .text

asm_strcmp:
    push rbp
    mov rbp, rsp

    ; Arguments:
    ; rdi = first string (S1)
    ; rsi = second string (S2)

    .compare_loop:
        ; Load bytes from each string
        mov al, byte [rdi]
        mov dl, byte [rsi]

        ; Compare bytes
        cmp al, dl
        jne .different_chars

        ; Check if end of string (null terminator)
        test al, al
        jz .strings_equal

        ; Move to next characters
        inc rdi
        inc rsi

        ; Repeat loop
        jmp .compare_loop

    .different_chars:
        ; Calculate difference
        sub al, dl
        movzx eax, al  ; Zero-extend result to 32-bit

        ; Exit function
        pop rbp
        ret

    .strings_equal:
        ; Strings are equal
        xor eax, eax   ; Return 0

        ; Exit function
        pop rbp
        ret
