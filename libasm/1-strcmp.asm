global asm_strncmp
section .text

asm_strcmp:
    push rbp
    mov rbp, rsp

    ; Parameters:
    ; rdi = s1 (first string)
    ; rsi = s2 (second string)
    ; rdx = n (maximum number of characters to compare)

    mov rax, 0        ; i = 0
    mov r8, 0         ; diff = 0

.loop:
    cmp rax, rdx      ; Check if i >= n
    jge .end          ; If i >= n, end loop

    mov al, [rdi]     ; Load byte from s1
    mov dl, [rsi]     ; Load byte from s2
    test al, al       ; Check if end of s1 ('\0')
    je .end           ; If s1 end, end loop
    test dl, dl       ; Check if end of s2 ('\0')
    je .end           ; If s2 end, end loop

    sub al, dl        ; Calculate difference
    movsx r9d, al     ; Sign-extend to 32-bit

    test r8d, r8d     ; Check if diff == 0
    jnz .diff_set     ; If diff != 0, skip setting diff

    mov r8d, r9d      ; Set diff = difference

.diff_set:
    add rdi, 1        ; Increment s1 pointer
    add rsi, 1        ; Increment s2 pointer
    add rax, 1        ; Increment i
    jmp .loop         ; Continue loop

.end:
    test r8d, r8d     ; Check diff
    jg .return_1      ; diff > 0
    jl .return_neg1   ; diff < 0
    xor eax, eax      ; diff == 0
    jmp .return       ; Return result

.return_1:
    mov eax, 1        ; Return 1
    jmp .return

.return_neg1:
    mov eax, -1       ; Return -1

.return:
    pop rbp
    ret
