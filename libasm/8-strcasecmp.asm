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

.BOOP:
    movzx r8d, BYTE [rdi]   ; Load byte from S1[rcx] into r8
    movzx r9d, BYTE [rsi]   ; Load byte from S2[rcx] into r9

	cmp r8b, 0x00
	jnz .convert
	cmp r9b, 0x00
	jnz .CHECK_LOWER1
    jmp .calc

.convert:
    ; Convert r8 to lowercase if it's an uppercase letter
    cmp r8b, 'A'
    jl .CHECK_LOWER1
    cmp r8b, 'Z'
    jg .CHECK_LOWER1
    add r8w, 32

.CHECK_LOWER1:
    ; Convert r9 to lowercase if it's an uppercase letter
    cmp r9b, 'A'
    jl .COMPARE
    cmp r9b, 'Z'
    jg .COMPARE
    add r9w, 32

.COMPARE:
    ; Compare characters after conversion
    cmp r8b, r9b
    je .next_chars     ; If equal, check end condition

.calc:
    ; Characters are different
	xor rax, rax
	sub r8, r9           ; 64 byte
	mov rax, r8          ; 64 byte
    jmp .END             ; Exit loop

.next_chars:
    inc rdi
	inc rsi
    jmp .BOOP

.END:
	pop rcx
	pop rbx
	pop rbp
    ret                  ; Return from function