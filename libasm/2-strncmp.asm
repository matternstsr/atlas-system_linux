section .text
global asm_strchr

asm_strchr:
    test rdi, rdi        ; Check if s (rdi) is NULL
    jz .not_found        ; If s is NULL, return NULL

    mov bl, byte [rsi]   ; Move character to find (c) into bl

.strchr_loop:
    mov al, byte [rdi]   ; Load byte from memory address in rdi into al
    cmp al, bl           ; Compare al with bl (character to find)
    je .found            ; Jump if equal to found

    cmp al, 0x00         ; Compare al with '\0' (end of string)
    je .not_found        ; Jump if end of string ('\0')

    inc rdi              ; Increment pointer to next character in string
    jmp .strchr_loop     ; Continue loop

.found:
    mov rax, rdi         ; Return pointer to the found character (rdi currently points to it)
    ret

.not_found:
    xor rax, rax         ; Return NULL (rax is already 0)
    ret
