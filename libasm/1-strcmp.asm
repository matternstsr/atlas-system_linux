global asm_strcmp
section .text

asm_strcmp:
    push rbp
    mov rbp, rsp

.compare_strings:
    ; Load and compare bytes from s1 and s2
    mov al, [rdi]
    mov dl, [rsi]
    
    ; If characters differ, return difference
    cmp al, dl
    jne .return_diff
    
    ; If both characters are null, strings are equal
    test al, al
    je .return_0
    
    ; Increment pointers and continue comparison
    inc rdi
    inc rsi
    jmp .compare_strings

.return_diff:
    sub eax, edx    ; Return difference between characters
    jmp .return

.return_0:
    xor eax, eax    ; Return 0 (strings are equal)
    
.return:
    pop rbp
    ret
