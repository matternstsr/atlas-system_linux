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
    movzx r8, BYTE [rdi+rcx]   ; Load byte from str1[rcx] into r8
    movzx r9, BYTE [rsi+rcx]   ; Load byte from str2[rcx] into r9

    test r8b, r8b        ; Check for end of string (null terminator of str1)
    jz .END              ; If r8b is 0, strings are equal

    cmp r8b, 'A'         ; Compare r8b with 'A'
    jl .CHECK_LOWER1     ; If r8b < 'A', it's not uppercase
    cmp r8b, 'Z'         ; Compare r8b with 'Z'
    jg .CHECK_LOWER1     ; If r8b > 'Z', it's not uppercase
    add r8b, 32          ; Convert r8b to lowercase

.CHECK_LOWER1:
    cmp r9b, 'A'         ; Compare r9b with 'A'
    jl .CHECK_LOWER2     ; If r9b < 'A', it's not uppercase
    cmp r9b, 'Z'         ; Compare r9b with 'Z'
    jg .CHECK_LOWER2     ; If r9b > 'Z', it's not uppercase
    add r9b, 32          ; Convert r9b to lowercase

.CHECK_LOWER2:
    cmp r8b, r9b         ; Compare r8b and r9b (both lowercase now)
    je .NEXT_CHAR        ; If equal, compare next character

    ; Characters are different
    movsx eax, r8b       ; Move r8b (as signed integer) into eax
    movsx ebx, r9b       ; Move r9b (as signed integer) into ebx
    sub eax, ebx         ; Subtract ebx from eax (character difference)
    jmp .END             ; Exit loop

.NEXT_CHAR:
    inc rcx              ; Move to next character
    jmp .LOOP            ; Repeat loop

.END:
    ret                  ; Return from function
