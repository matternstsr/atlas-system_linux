global asm_strcmp
section .text

asm_strcmp:
    push rbp
    mov rbp, rsp

.compare_strings:
    ; Load bytes from s1 and s2
    mov al, [rdi]   ; Load byte from address pointed by rdi (s1)
    mov dl, [rsi]   ; Load byte from address pointed by rsi (s2)
    
    ; Compare bytes
    cmp al, dl
    jne .return_diff  ; If bytes are not equal, return the difference
    
    ; Check for end of string (null terminator)
    test al, al
    je .return_0  ; If al is 0, both strings are equal
    
    ; Move to the next characters
    inc rdi
    inc rsi
    jmp .compare_strings  ; Continue comparison

.return_diff:
    sub eax, edx   ; Calculate the difference between characters
    jmp .return

.return_0:
    xor eax, eax   ; Return 0 (strings are equal)

.return:
    pop rbp
    ret
