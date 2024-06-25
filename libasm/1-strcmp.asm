global asm_strcmp

section .text

asm_strcmp:
    push rbp
    mov rbp, rsp

    ; Load parameters s1 and s2
    mov rdi, QWORD [rbp + 8]     ; rdi = s1
    mov rsi, QWORD [rbp + 16]    ; rsi = s2

    ; Handle special case where both strings are empty ("")
    mov al, BYTE [rdi]           ; Load byte from s1 into al
    test al, al                  ; Test if *s1 is null terminator
    jz .check_s2_empty           ; Jump to check if s2 is empty if s1 is empty

    mov dl, BYTE [rsi]           ; Load byte from s2 into dl
    test dl, dl                  ; Test if *s2 is null terminator
    jz .return_result            ; If s1 not empty & s2 is, ret 1 (s1 > s2)
.loop_compare:
    ; Compare characters
    cmp al, 0                    ; Check if *s1 (al) is null terminator
    je .return_result            ; If *s1 is null, jump to return result
    cmp dl, 0                    ; Check if *s2 (dl) is null terminator
    je .return_result            ; If *s2 is null, jump to return result
    cmp al, dl                   ; Compare characters
    jne .decide_result           ; If chars are not equal, jump to decide result

    ; Increment pointers and continue loop
    inc rdi                      ; Increment s1
    mov al, BYTE [rdi]           ; Load byte from updated s1 into al
    inc rsi                      ; Increment s2
    mov dl, BYTE [rsi]           ; Load byte from updated s2 into dl
    jmp .loop_compare

.check_s2_empty:
    ; Check if s2 is empty
    mov dl, BYTE [rsi]           ; Load byte from s2 into dl
    test dl, dl                  ; Test if *s2 is null terminator
    jz .return_result            ; If both s1 & s2 are empty, ret 0 (s1 == s2)

    ; s1 is empty but s2 is not empty, return -1 (s1 < s2)
    mov eax, -1
    jmp .return

.decide_result:
    ; Compare results based on character comparison
    movzx eax, al                ; Zero-extend al (s1 character) into eax
    movzx edx, dl                ; Zero-extend dl (s2 character) into edx
    sub eax, edx                 ; Calc diff between s1 and s2 characters
    jmp .return                  ; Jump to return with result

.return_result:
    ; Return based on the comparison of s1 and s2
    cmp al, dl                   ; Compare characters
    jg .greater_than             ; If s1 > s2, jump to greater_than
    jl .less_than                ; If s1 < s2, jump to less_than

    ; s1 and s2 are equal
    mov eax, 0
    jmp .return

.greater_than:
    ; Return 1 if s1 > s2
    mov eax, 1
    jmp .return

.less_than:
    ; Return -1 if s1 < s2
    mov eax, -1

.return:
    ; Epilogue
    pop rbp                      ; Restore base pointer
    ret                          ; Return to caller with result in eax
