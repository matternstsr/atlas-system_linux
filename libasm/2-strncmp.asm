BITS 64

section .text
global asm_strncmp

asm_strncmp:
	xor eax, eax
	xor rcx, rcx
	jmp .loop
.move:
	cmp bl, 0
	je .end
	inc rcx
.loop:
	cmp rdx, rcx
	je .end
	mov bl, BYTE [rdi+rcx]
	mov r8b, BYTE [rsi+rcx]
	cmp bl, r8b
	je .move
.notsame:
	movsx eax, bl
	movsx ebx, r8b
	sub eax, ebx
.end:
	ret