global asm_strcmp
section .text

asm_strcmp:
    ; Prologue
    push rbp
    mov rbp, rsp

    ; Load parameters s1, s2, and n
    mov rdi, QWORD [rbp + 8]     ; rdi = s1
    mov rsi, QWORD [rbp + 16]    ; rsi = s2
    mov rdx, QWORD [rbp + 24]    ; rdx = n

    ; Initialize counters and result storage
    xor rcx, rcx                 ; i = 0 (counter)
    mov eax, 0                   ; diff = 0 (result)

    ; Comparison loop
.loop_compare:
    ; Check if i < n (compare up to n characters)
    cmp rcx, rdx
    jge .return_result           ; If i >= n, jump to return result

    ; Load characters from s1 and s2
    mov al, BYTE [rdi]           ; Load s1 character into al
    mov dl, BYTE [rsi]           ; Load s2 character into dl

    ; Check for null terminators
    test al, al
    je .return_result            ; If *s1 (al) is null, jump to return result
    test dl, dl
    je .return_result            ; If *s2 (dl) is null, jump to return result

    ; Compare characters
    cmp al, dl                   ; Compare s1 and s2 characters
    jne .decide_result           ; If not equal, decide result

    ; Increment pointers and counters
    inc rdi                      ; Increment s1 pointer
    inc rsi                      ; Increment s2 pointer
    inc rcx                      ; Increment counter
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
