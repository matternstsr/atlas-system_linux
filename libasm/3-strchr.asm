global asm_strchr
section .text

asm_strchr:
    push rbp
    mov rbp, rsp
    push rbx

    ; Load parameters into registers
    mov rdi, rdi ; rdi = s (string)
    mov rsi, rsi ; rsi = c (character)

    ; If s or c is NULL, return NULL (0)
    test rdi, rdi
    jz .return_null

    ; Load the first byte of the string
.get_next_char:
    movzx rbx, byte [rdi]
    cmp rbx, 0
    jz .return_null ; If end of string, return NULL
    
    ; Compare current character with c
    cmp bl, sil
    jz .return_found ; If match, return pointer to current character

    ; Move to next character in string
    inc rdi
    jmp .get_next_char

.return_null:
    xor rax, rax ; Return 0 (NULL) if s or c is NULL
    test rsi, 0
    jz .return_found

.return_found:
    mov rax, rdi ; Return pointer to current character in s

.exit:
    pop rbx
    pop rbp
    ret
