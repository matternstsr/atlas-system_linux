BITS 64

global asm_strlen

section .text
	;; size_t asm_strlen(const char *str)
	;; {
	;;         size_t len = 0;
	;;         for (; *str; str++)
	;;                 len++;
	;;         return (len);
	;; }

asm_strlen:
	push rbp
	mov rbp, rsp
	mov QWORD [rbp - 24], rdi
	mov QWORD [rbp - 8], 0
	jmp .testing_str
.length_of_str:
	add QWORD [rbp - 8], 1
	add QWORD [rbp - 24], 1
.testing_str:
	mov rax, QWORD [rbp - 24]
	movzx eax, BYTE [rax]
	test al, al
	jne .length_of_str
	mov rax, QWORD [rbp - 8]
	pop rbp
	ret
