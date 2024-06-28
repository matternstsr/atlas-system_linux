BITS 64

section .text
global strcasecmp

strcasecmp:
	xor eax, eax		; set return value to 0
	xor rcx, rcx		; set an index to 0
	jmp .LOOP		; goto '.LOOP'

.INCR:
	cmp bl, 0		; id bl equal to '\0' ?
	je .END			; if yes, goto '.END'
	inc rcx			; index incrementation

.LOOP:
	mov bl, BYTE [rdi+rcx]	; put str1[rcx] into bl
	mov dl, BYTE [rsi+rcx]	; put str2[rcx] into dl
	cmp bl, 'A'		; is bl < 'A' ?
	jl .NO_UPS1		; if yes, goto '.NO_UPPER1'
	cmp bl, 'Z'		; is bl > 'Z' ?
	jg .NO_UPS1		; if yes, goto '.NO_UPPER1'
	add bl, 32		; lowercase bl

.NO_UPS1:
	cmp dl, 'A'		; is dl < 'A' ?
	jl .NO_UPS2		; if yes, goto '.NO_UPPER2'
	cmp dl, 'Z'		; is dl > 'Z' ?
	jg .NO_UPS2		; if yes, goto '.NO_UPPER2'
	add dl, 32		; lowercase dl

.NO_UPS2:
	cmp bl, dl		; is bl equal to dl ?
	je .INCR		; if yes, goto '.INCREMENT'

.DIFF:
	movsx eax, bl		; put bl char into eax int
	movsx ebx, dl		; put dl char into ebx int
	sub eax, ebx		; eax = eax - ebx

.END:
	ret			; end