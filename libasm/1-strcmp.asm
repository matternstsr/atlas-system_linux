global asm_strcmp
section .text

asm_strcmp:
    ; Prologue
    push rbp
    mov rbp, rsp

    ; Initialize s1 and s2 pointers
    mov rdi, QWORD [rbp + 8]     ; rdi = s1
    mov rsi, QWORD [rbp + 16]    ; rsi = s2

    ; Comparison loop
.loop_compare:
    ; Load characters from s1 and s2
    mov al, BYTE [rdi]           ; Load s1 character into al
    mov dl, BYTE [rsi]           ; Load s2 character into dl

    ; Compare characters
    cmp al, dl                   ; Compare s1 and s2 characters
    jne .decide_result           ; If not equal, decide result

    ; Check for null terminators
    test al, al                  ; Check if *s1 (al) is null
    jz .check_s2_null            ; If *s1 is null, jump to check s2 null

    test dl, dl                  ; Check if *s2 (dl) is null
    jz .return_equal             ; If *s2 is null, jump to return equal

    ; Increment pointers and continue loop
    inc rdi                      ; Increment s1 pointer
    inc rsi                      ; Increment s2 pointer
    jmp .loop_compare            ; Continue comparison loop

.check_s2_null:
    ; Check if s2 is null and s1 is not
    test dl, dl                  ; Check if *s2 (dl) is null
    jz .return_equal             ; If *s2 is null, jump to return equal

    ; Return result for s1 < s2
    mov eax, -1
    jmp .return                  ; Jump to return

.return_equal:
    ; Return result for s1 == s2
    xor eax, eax                 ; eax = 0 (equal)
    jmp .return                  ; Jump to return

.decide_result:
    ; Determine result based on character comparison
    movsx eax, al                ; Sign-extend al (s1 character) into eax
    movsx edx, dl                ; Sign-extend dl (s2 character) into edx
    sub eax, edx                 ; Calculate difference between characters
    jmp .return                  ; Jump to return

.return:
    ; Epilogue
    pop rbp                      ; Restore base pointer
    ret                          ; Return to caller with result in eax
