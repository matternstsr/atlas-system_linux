BITS 64
global asm_strcasecmp
section .text

; Arguments:
;   rdi = pointer to first string (S1)
;   rsi = pointer to second string (S2)

asm_strcasecmp:
    xor eax, eax        ; Clear eax (return value)
    xor rcx, rcx        ; Clear rcx (index)

.LOOP:
    movzx r8, BYTE [rdi+rcx]   ; Load byte from S1[rcx] into r8
    movzx r9, BYTE [rsi+rcx]   ; Load byte from S2[rcx] into r9

    ; Check for end of strings
    test r8b, r8b
    jz .END              ; If r8b is 0, strings are equal
    test r9b, r9b
    jz .END              ; If r9b is 0, strings are equal

    ; Convert r8 to lowercase if it's an uppercase letter
    cmp r8b, 'A'
    jl .COMPARE
    cmp r8b, 'Z'
    jg .COMPARE
    add r8b, 32

    ; Convert r9 to lowercase if it's an uppercase letter
    cmp r9b, 'A'
    jl .COMPARE
    cmp r9b, 'Z'
    jg .COMPARE
    add r9b, 32

.COMPARE:
    ; Compare characters after conversion
    cmp r8b, r9b
    jne .DIFFERENT       ; If characters are different, exit

    ; Move to next character
    inc rcx              ; Move to next character index
    jmp .LOOP            ; Repeat loop

.DIFFERENT:
    ; Calculate difference and return
    movsx eax, r8b       ; Move r8b (as signed integer) into eax
    movsx ebx, r9b       ; Move r9b (as signed integer) into ebx
    sub eax, ebx         ; Subtract ebx from eax (character difference)
    ret                  ; Exit function

.END:
    ; Return zero if strings are equal in length
    xor eax, eax
    ret
