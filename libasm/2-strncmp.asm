BITS 64

global asm_strncmp
section .text

asm_strncmp:
    push rbp
    mov rbp, rsp
	push rbx

    ; Arguments:
    ; rdi = first string (S1)
    ; rsi = second string (S2)
    ; rdx = size_t n (number of characters to compare)
    ; Loop through the strings
.asm_strncmp_loop:
    ; Load byte from S1 and S2
    movzx ebx, byte [rdi]
    movzx eax, byte [rsi]

	cmp bl, 0x00
	jz .null_found

    cmp bl, al
    jl .less_than   ; bl < al
    jg .greater_than ; al > bl

    ; Characters are equal, move to next
    inc rdi
    inc rsi
	dec rdx
	cmp rdx, 0
	jz .null_found

    jmp .asm_strncmp_loop  ; Continue loop

.null_found:
	cmp bl, al
	jl .less_than
	xor rax, rax
	jmp .exit

.less_than:
    ; S2 < S1
    mov rax, -1
    jmp .exit

.greater_than:
    ; S2 > S1
    mov rax, 1
    jmp .exit

.exit:
    pop rbp
	pop rbx
    ret