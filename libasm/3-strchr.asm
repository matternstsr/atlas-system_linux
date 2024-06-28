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

    test al, al          ; Check for end of string ('\0')
    jz .end              ; Jump to .end if end of string ('\0')

    inc rdi              ; Increment pointer to next character in string
    jmp .strchr_loop     ; Continue loop

.found:
    mov rax, rdi         ; Return pointer to the found character (rdi currently points to it)
    ret

.not_found:
    xor rax, rax         ; Return NULL (rax is already 0)
    ret

.end:
    cmp bl, 0x00         ; Check if character to find is '\0'
    jne .not_found       ; If not '\0', return NULL (not found)
    mov rax, rdi         ; Return pointer to the end of string ('\0')
    ret
