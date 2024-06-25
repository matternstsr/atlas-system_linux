global asm_strcmp
section .text

asm_strcmp:
    ; Prologue
    push rbp
    mov rbp, rsp

    ; Load parameters s1 and s2
    mov rdi, QWORD [rbp + 8]     ; rdi = s1
    mov rsi, QWORD [rbp + 16]    ; rsi = s2

    ; Initialize variables
    mov QWORD [rbp - 8], 0       ; i (iteration counter)
    mov DWORD [rbp - 12], 0      ; diff (comparison result)

    ; Start of comparison loop
    jmp .loop_compare

.loop_compare:
    ; Load characters from s1 and s2
    mov al, BYTE [rdi]           ; Load s1 character into al
    mov dl, BYTE [rsi]           ; Load s2 character into dl

    ; Check for null terminators or end of string
    test al, al                  ; Check if *s1 (al) is null terminator
    je .return_result            ; If *s1 is null, jump to return result
    test dl, dl                  ; Check if *s2 (dl) is null terminator
    je .return_result            ; If *s2 is null, jump to return result

    ; Compare characters
    cmp al, dl                   ; Compare s1 and s2 characters
    jne .decide_result           ; If not equal, decide result

    ; Increment pointers and iteration counter
    inc rdi                      ; Increment s1 pointer
    inc rsi                      ; Increment s2 pointer
    inc QWORD [rbp - 8]          ; Increment iteration counter
    jmp .loop_compare            ; Continue comparison loop

.decide_result:
    ; Determine result based on character comparison
    movzx eax, al                ; Zero-extend al (s1 character) into eax
    movzx edx, dl                ; Zero-extend dl (s2 character) into edx
    sub eax, edx                 ; Calculate difference between characters
    mov DWORD [rbp - 12], eax    ; Store result in diff
    jmp .return                  ; Jump to return with result

.return_result:
    ; Return based on comparison of s1 and s2 lengths
    cmp DWORD [rbp - 12], 0      ; Compare diff
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
