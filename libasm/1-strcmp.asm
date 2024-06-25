global asm_strcmp
section .text

asm_strcmp:
    push rbp
    mov rbp, rsp

    ; Function prologue
    mov rdi, QWORD [rbp + 8]   ; First argument (s1)
    mov rsi, QWORD [rbp + 16]   ; Second argument (s2)

.compare_strings:
    ; Load and compare bytes from s1 and s2
    mov al, BYTE [rdi]
    test al, al
    je .check_s1_more_s2

    mov dl, BYTE [rsi]
    test dl, dl
    je .check_s1_more_s2

    cmp al, dl
    jne .return_compare_result

    ; Increment pointers
    inc rdi
    inc rsi
    jmp .compare_strings

.check_s1_more_s2:
    ; Check if s1 > s2
    mov al, BYTE [rdi]
    test al, al
    jz .return_0 ; s1 is empty, so equal if s2 is also empty

    mov eax, -1 ; Return -1 if s1 > s2
    jmp .return

.return_compare_result:
    ; Compare result in al (-1 if s1 < s2, 1 if s1 > s2)
    cmp al, dl
    setg al ; al = 1 if s1 > s2
    setl al ; al = -1 if s1 < s2

.return_0:
    mov eax, 1 ; Return 1 if s1 == s2
    jmp .return

.return:
    pop rbp
    ret
