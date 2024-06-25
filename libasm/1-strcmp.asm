global asm_strcmp

section .text

asm_strcmp:
    push rbp
    mov rbp, rsp

    ; Load parameters s1 and s2
    mov rdi, QWORD [rbp + 8]     ; rdi = s1
    mov rsi, QWORD [rbp + 16]    ; rsi = s2

.loop_compare:
    ; Load characters from s1 and s2
    mov al, BYTE [rdi]           ; Load byte from s1 into al
    mov dl, BYTE [rsi]           ; Load byte from s2 into dl

    ; Compare characters
    cmp al, 0                    ; Check if *s1 (al) is null terminator
    je .return_result            ; If *s1 is null, jump to return result
    cmp dl, 0                    ; Check if *s2 (dl) is null terminator
    je .return_result            ; If *s2 is null, jump to return result
    cmp al, dl                   ; Compare characters
    jne .decide_result           ; If charc are not =, jump to decide result

    ; Increment pointers and continue loop
    inc rdi                      ; Increment s1
    inc rsi                      ; Increment s2
    jmp .loop_compare

.decide_result:
    ; Compare results based on character comparison
    movzx eax, al                ; Zero-extend al (s1 character) into eax
    movzx edx, dl                ; Zero-extend dl (s2 character) into edx
    sub eax, edx                 ; Calc diff between s1 and s2 characters
    jmp .return                  ; Jump to return with result

.return_result:
    ; Return based on the comparison of s1 and s2
    mov eax, 0                   ; Default result is 0 (equal)
    cmp al, dl                   ; Compare characters
    jg .greater_than             ; If s1 > s2, jump to greater_than
    jl .less_than                ; If s1 < s2, jump to less_than

.greater_than:
    ; Return 1 if s1 > s2
    mov eax, 1                   ; Result is 1
    jmp .return                  ; Jump to return with result

.less_than:
    ; Return -1 if s1 < s2
    mov eax, -1                  ; Result is -1

.return:
    ; Epilogue
    pop rbp                      ; Restore base pointer
    ret                          ; Return to caller with result in eax
