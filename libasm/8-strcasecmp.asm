section .text
global asm_strcasecmp

; Arguments:
;   rdi = pointer to first string (S1)
;   rsi = pointer to second string (S2)
;   rdx = size_t n (number of characters to compare)

asm_strcasecmp:
    xor eax, eax        ; Clear eax (return value)
    xor rcx, rcx        ; Clear rcx (index)

.LOOP:
    movzx r8, BYTE [rdi+rcx]   ; Load byte from S1[rcx] into r8
    movzx r9, BYTE [rsi+rcx]   ; Load byte from S2[rcx] into r9

    ; Convert r8 to lowercase if it's an uppercase letter
    cmp r8b, 'A'
    jl .CHECK_LOWER1
    cmp r8b, 'Z'
    jg .CHECK_LOWER1
    add r8b, 32

.CHECK_LOWER1:
    ; Convert r9 to lowercase if it's an uppercase letter
    cmp r9b, 'A'
    jl .COMPARE
    cmp r9b, 'Z'
    jg .COMPARE
    add r9b, 32

.COMPARE:
    ; Compare characters after conversion
    cmp r8b, r9b
    je .CHECK_END        ; If equal, check end condition

    ; Characters are different
    movsx eax, r8b       ; Move r8b (as signed integer) into eax
    movsx ebx, r9b       ; Move r9b (as signed integer) into ebx
    sub eax, ebx         ; Subtract ebx from eax (character difference)
    jmp .END             ; Exit loop

.CHECK_END:
    ; Check for end of strings
    test r8b, r8b
    jz .END              ; If r8b is 0, strings are equal
    test r9b, r9b
    jz .END              ; If r9b is 0, strings are equal

    ; Move to next character
    inc rcx
    jmp .LOOP

.END:
    ret                  ; Return from function
