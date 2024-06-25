global asm_strcmp
section .text

asm_strcmp:
    ; Prologue
    push rbp
    mov rbp, rsp

    ; Initialize s1 and s2 pointers
    mov QWORD [rbp - 8], rdi     ; rdi = s1
    mov QWORD [rbp - 16], rsi    ; rsi = s2

    ; Comparison loop
.loop_compare:
    ; Load characters from s1 and s2
    mov rax, QWORD [rbp - 8]    ; Load s1 pointer into rax
    movzx eax, BYTE [rax]       ; Load s1 character into eax
    test al, al                 ; Check if *s1 (al) is null
    je .return_result           ; If *s1 is null, jump to return result

    mov rax, QWORD [rbp - 16]   ; Load s2 pointer into rax
    movzx edx, BYTE [rax]       ; Load s2 character into edx
    test dl, dl                 ; Check if *s2 (dl) is null
    je .return_result           ; If *s2 is null, jump to return result

    cmp al, dl                  ; Compare s1 and s2 characters
    jne .decide_result          ; If characters are not equal, decide result

    ; Increment pointers and continue loop
    mov rax, QWORD [rbp - 8]    ; Load s1 pointer into rax
    add QWORD [rbp - 8], 1      ; Increment s1 pointer
    mov rax, QWORD [rbp - 16]   ; Load s2 pointer into rax
    add QWORD [rbp - 16], 1     ; Increment s2 pointer
    jmp .loop_compare           ; Continue comparison loop

.decide_result:
    ; Determine result based on character comparison
    movzx eax, al               ; Zero-extend al (s1 character) into eax
    movzx edx, dl               ; Zero-extend dl (s2 character) into edx
    sub eax, edx                ; Calculate difference between characters
    jmp .return                 ; Jump to return with result

.return_result:
    ; Return based on comparison of s1 and s2 lengths
    cmp al, dl                  ; Compare s1 and s2 characters
    jg .greater_than            ; If s1 > s2, jump to greater_than
    jl .less_than               ; If s1 < s2, jump to less_than

    ; If lengths are equal and characters are equal
    mov eax, 0                  ; Return 0 (equal)
    jmp .return                 ; Jump to return

.greater_than:
    ; Return 1 if s1 > s2
    mov eax, 1

.less_than:
    ; Return -1 if s1 < s2
    mov eax, -1

.return:
    ; Epilogue
    pop rbp                     ; Restore base pointer
    ret                         ; Return to caller with result in eax
