global asm_strcmp
section .text

asm_strcmp:
    push rbp
    mov rbp, rsp

    ; Arguments:
    ; rdi = first string (S1)
    ; rsi = second string (S2)

    xor eax, eax  ; Initialize eax (return value) to 0

    .compare_loop:
        mov al, byte [rdi]  ; Load byte from S1 into al
        mov dl, byte [rsi]  ; Load byte from S2 into dl

        cmp al, dl          ; Compare bytes
        jne .different_chars ; Jump if different

        test al, al         ; Check for end of S1 ('\0')
        jz .strings_equal   ; If end of S1, strings are equal

        inc rdi             ; Move to next character in S1
        inc rsi             ; Move to next character in S2
        jmp .compare_loop   ; Repeat loop

    .different_chars:
        sub al, dl          ; Calculate difference
        movsx eax, al       ; Sign-extend difference to 32-bit

    .strings_equal:
        pop rbp             ; Restore rbp
        ret                 ; Return with result in eax
