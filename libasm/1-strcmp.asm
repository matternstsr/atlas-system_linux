global asm_strcmp
section .text

asm_strcmp:
    push rbp
    mov QWORD PTR [rbp - 8], rdi
    mov QWORD PTR [rbp - 16], rsi
    jmp .compare_strings

.compare_strings:
    ; Load and compare bytes from s1 and s2
    mov rax, QWORD PTR [rbp - 8]
    movzx eax, BYTE PTR [rax]
    test al, al
    je .check_s1_more_s2

    mov rax, QWORD PTR [rbp - 16]
    movzx eax, BYTE PTR [rax]
    test al, al
    je .check_s1_more_s2

    mov rax, QWORD PTR [rbp - 8]
    movzx edx, BYTE PTR [rax]

    mov rax, QWORD PTR [rbp - 16]
    movzx eax, BYTE PTR [rax]

    cmp dl, al
    je .compare_strings

.check_s1_more_s2:
    ; Check if s1 > s2
    mov rax, QWORD PTR [rbp - 8]
    movzx edx, BYTE PTR [rax]

    mov rax, QWORD PTR [rbp - 16]
    movzx eax, BYTE PTR [rax]

    cmp dl, al
    jle .check_s1_less_s2

    mov eax, 1 ; Return 1 if s1 > s2
    jmp .return

.check_s1_less_s2:
    ; Check if s1 < s2
    mov rax, QWORD PTR [rbp - 8]
    movzx edx, BYTE PTR [rax]

    mov rax, QWORD PTR [rbp - 16]
    movzx eax, BYTE PTR [rax]

    cmp dl, al
    jge .return_0

    mov eax, -1 ; Return -1 if s1 < s2
    jmp .return

.return_0:
    mov eax, 0 ; Return 0 if s1 == s2

.return:
    pop rbp
    ret
