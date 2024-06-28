BITS 64

global asm_strcasecmp
section .text
; Arguments:
;   rdi = pointer to first string (S1)
;   rsi = pointer to second string (S2)
;   rdx = size_t n (number of characters to compare)

asm_strcasecmp:
	push rbp
	mov rbp, rsp
	xor rax, rax
	push rbx
	push rcx

    xor eax, eax        ; Clear eax (return value)
    xor rcx, rcx        ; Clear rcx (index)

.BOOP:
    movzx r8, BYTE [rdi]   ; Load byte from S1[rcx] into r8
    movzx r9, BYTE [rsi]   ; Load byte from S2[rcx] into r9

    ; Convert r8 to lowercase if it's an uppercase letter
    cmp r8b, '65'
    jl .CHECK_LOWER1
    cmp r8b, '90'
    jg .CHECK_LOWER1
    add r8b, 32

.CHECK_LOWER1:
    ; Convert r9 to lowercase if it's an uppercase letter
    cmp r9b, '65'
    jl .COMPARE
    cmp r9b, '90'
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
    jmp .BOOP

.END:
    ret                  ; Return from function
