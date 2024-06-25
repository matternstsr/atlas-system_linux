global asm_strcmp
section .text

asm_strcmp:
    push rbp
    mov rbp, rsp
    mov rdi, QWORD [rbp + 16]  ; s1
    mov rsi, QWORD [rbp + 24]  ; s2

    ; Check if both strings are empty
    test rdi, rdi
    jz .check_s2_empty
    test rsi, rsi
    jz .s1_not_empty_s2_empty

    ; Compare strings character by character
.compare_loop:
    mov al, BYTE [rdi]
    cmp al, BYTE [rsi]
    jne .strings_not_equal
    test al, al
    jz .strings_equal

    inc rdi
    inc rsi
    jmp .compare_loop

.strings_equal:
    mov eax, 0
    jmp .return

.strings_not_equal:
    movzx eax, BYTE [rdi]
    movzx edx, BYTE [rsi]
    sub eax, edx
    jmp .return

.check_s2_empty:
    test rsi, rsi
    jz .strings_equal
    mov eax, -1
    jmp .return

.s1_not_empty_s2_empty:
    mov eax, 1

.return:
    pop rbp
    ret
