global asm_strcmp

section .text

asm_strcmp:
    ; Prologue
    push rbp
    mov rbp, rsp

    ; Load parameters s1 and s2
    mov rdi, QWORD [rbp + 8]     ; rdi = s1
    mov rsi, QWORD [rbp + 16]    ; rsi = s2

.loop_compare:    ; Comparison loop
    ; Load characters from s1 and s2
    mov al, BYTE [rdi]           ; Load s1 character into al
    mov dl, BYTE [rsi]           ; Load s2 character into dl

    ; Check for null terminators
    cmp al, 0
    je .return_result            ; If *s1 (al) is null, return result
    cmp dl, 0
    je .return_result            ; If *s2 (dl) is null, return result

    ; Compare characters
    cmp al, dl                   ; Compare s1 and s2 characters
    jne .decide_result           ; If not equal, decide result

    ; Increment pointers and continue loop
    inc rdi                      ; Increment s1 pointer
    inc rsi                      ; Increment s2 pointer
    jmp .loop_compare            ; Continue comparison loop

.decide_result:
    ; Determine result based on character comparison
    movzx eax, al                ; Zero-extend al (s1 character) into eax
    movzx edx, dl                ; Zero-extend dl (s2 character) into edx
    sub eax, edx                 ; Calculate difference between characters
    jmp .return                  ; Jump to return with result

.return_result:
    ; Return based on comparison of s1 and s2 lengths
    cmp al, dl                   ; Compare s1 and s2 characters
    jg .greater_than             ; If s1 > s2, jump to greater_than
    jl .less_than                ; If s1 < s2, jump to less_than

    ; If lengths are equal and characters are equal
    mov eax, 0                   ; Return 0 (equal)
    jmp .return                  ; Jump to return

.greater_than:
    ; Return 1 if s1 > s2
    mov eax, 1

.less_than:
    ; Return -1 if s1 < s2
    mov eax, -1

.return:
    ; Epilogue
    pop rbp                      ; Restore base pointer
    ret                          ; Return to caller with result in eax
