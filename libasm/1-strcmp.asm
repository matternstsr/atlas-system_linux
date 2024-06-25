global asm_strcmp
section .text

asm_strcmp:
	push rbp
	mov rbp, rsp
	mov QWORD [rbp - 8], rdi
	mov QWORD [rbp - 16], rsi
	jmp .test_s1_v_s2
.strings_to_cmp:
	add QWORD [rbp - 8], 1
	add QWORD [rbp - 16], 1
.test_s1_v_s2:
	mov rax, QWORD [rbp - 8]
	movzx eax, BYTE [rax]
	test al, al
	je .test_s1_more_s2
	mov rax, QWORD [rbp - 16]
	movzx eax, BYTE [rax]
	test al, al
	je .test_s1_more_s2
	mov rax, QWORD [rbp - 8]
	movzx edx, BYTE [rax]
	mov rax, QWORD [rbp - 16]
	movzx eax, BYTE [rax]
	cmp dl, al
	je .strings_to_cmp
.test_s1_more_s2:
	mov rax, QWORD [rbp - 8]
	movzx edx, BYTE [rax]
	mov rax, QWORD [rbp - 16]
	movzx eax, BYTE [rax]
	cmp dl, al
	jle .test_s1_less_s2
	mov eax, 1
	jmp .return
.test_s1_less_s2:
	mov rax, QWORD [rbp - 8]
	movzx edx, BYTE [rax]
	mov rax, QWORD [rbp - 16]
	movzx eax, BYTE [rax]
	cmp dl, al
	jge .return_0
	mov eax, -1
	jmp .return
.return_0:
	mov eax, 0
.return:
	pop rbp
	ret
