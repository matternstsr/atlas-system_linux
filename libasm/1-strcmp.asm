global asm_strcmp
section .text
asm_strcmp:
    push rbp
    mov rbp, rsp
    mov QWORD [rbp - 8], rdi   ; s1
    mov QWORD [rbp - 16], rsi  ; s2

    ; Handle case where both strings are empty
    mov rax, QWORD [rbp - 8]
    test rax, rax
    je .s1_empty
    mov rax, QWORD [rbp - 16]
    test rax, rax
    je .s2_empty

    ; Compare strings character by character
.test_loop:
    mov rax, QWORD [rbp - 8]
    movzx eax, BYTE [rax]
    test al, al
    je .strings_equal

    mov rax, QWORD [rbp - 16]
    movzx eax, BYTE [rax]
    test al, al
    je .strings_equal

    mov rax, QWORD [rbp - 8]
    movzx edx, BYTE [rax]
    mov rax, QWORD [rbp - 16]
    movzx eax, BYTE [rax]
    cmp dl, al
    jne .strings_not_equal

    add QWORD [rbp - 8], 1
    add QWORD [rbp - 16], 1
    jmp .test_loop

.strings_equal:
    mov eax, 0
    jmp .return

.strings_not_equal:
    mov rax, QWORD [rbp - 8]
    movzx edx, BYTE [rax]
    mov rax, QWORD [rbp - 16]
    movzx eax, BYTE [rax]
    cmp dl, al
    jl .s1_less_than_s2
    jg .s1_greater_than_s2

.s1_less_than_s2:
    mov eax, -1
    jmp .return

.s1_greater_than_s2:
    mov eax, 1
    jmp .return

.s1_empty:
    mov rax, QWORD [rbp - 16]
    test rax, rax
    je .strings_equal
    mov eax, -1
    jmp .return

.s2_empty:
    mov eax, 1
    jmp .return

.return:
    pop rbp
    ret
