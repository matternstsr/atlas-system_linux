BITS 64

global asm_strchr
section .text

;; char *asm_strchr(const char *s, int c);
asm_strchr:
    push rbp
    mov rbp, rsp
    push rbx

    ; Load character to find (c) into al
    movzx eax, BYTE [rsi]

    ; Loop through the string
.get_next_char:
    ; Load byte from s (rdi) into bl
    movzx ebx, BYTE [rdi]

    ; Compare current character (bl) with c (al)
    cmp bl, 0
    jz .not_found      ; If end of string ('\0'), return NULL

    cmp bl, al
    jz .return_found   ; If match found, return pointer to current character

    ; Move to the next character in the string
    inc rdi
    jmp .get_next_char ; Repeat loop

.return_found:
    ; If match found, return pointer to the found character
    mov rax, rdi
    jmp .exit

.not_found:
    ; If end of string ('\0') is reached without finding c, return NULL
    xor rax, rax        ; Set rax to 0 (NULL)

.exit:
    pop rbx
    pop rbp
    ret
