BITS 64

global asm_strstr
section .text
    
asm_strstr:
    push rbp
    mov rbp, rsp
    push rbx

    ; Null check for haystack or needle
    test rdi, rdi
    jz .return_null
    test rsi, rsi
    jz .return_haystack

    ; Get length of needle
    mov rcx, 0
.get_length:
    cmp byte [rsi + rcx], 0
    jz .outer_loop

    inc rcx
    jmp .get_length

.outer_loop:
    ; Move to the next character in haystack
    inc rdi
    mov rcx, 0

.inner_loop:
    cmp byte [rdi + rcx], byte [rsi + rcx]
    jnz .end_of_search

    inc rcx
    cmp byte [rsi + rcx], 0
    jz .return_found

    jmp .inner_loop

.return_found:
    mov rax, rdi
    jmp .exit

.return_null:
    xor rax, rax ; Return NULL if haystack is NULL
    jmp .exit

.return_haystack:
    mov rax, rdi ; Return haystack if needle is NULL
    jmp .exit

.end_of_search:
    cmp byte [rdi], 0 ; End of haystack reached
    jz .return_null

    jmp .outer_loop

.exit:
    pop rbx
    pop rbp
    ret
