global asm_strchr
section .text

asm_strchr:
    ; Arguments:
    ; rdi = const char *s (pointer to the string)
    ; rsi = int c (character to search for)

    ; Move the character to find (c) into cl (lower 8 bits of rcx)
    mov cl, byte [rsi]

    ; Handle edge case: empty string
    test rdi, rdi
    jz .not_found  ; If s is NULL, return NULL

    ; Loop through the string pointed by rdi
    xor rax, rax  ; Initialize rax to 0 (NULL)

.strchr_loop:
    ; Load byte from memory address in rdi into al
    mov al, byte [rdi]

    ; Compare al with cl (character to find)
    cmp al, cl
    je .found  ; Jump if equal to found

    ; Check for end of string ('\0')
    test al, al
    jz .not_found  ; Jump if end of string ('\0')

    ; Increment pointer to next character in string
    inc rdi

    ; Continue loop
    jmp .strchr_loop

.found:
    ; Return pointer to the found character (rdi currently points to it)
    mov rax, rdi
    ret

.not_found:
    ; Return NULL (rax is already 0)
    ret
