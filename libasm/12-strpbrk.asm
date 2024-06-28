BITS 64

global asm_strpbrk
section .text

; char *asm_strpbrk(const char *s, const char *accept)
asm_strpbrk:
    push rbp
    mov rbp, rsp
    xor rax, rax            ; Initialize return pointer to NULL
    mov rcx, rdi            ; rcx = s (source string)
    mov rdx, rsi            ; rdx = accept (accept string)

.loop:
    movzx r8d, BYTE [rcx]   ; Load byte from s into r8
    test r8b, r8b           ; Check if end of string s ('\0')
    jz .end                 ; If end of s, exit loop

    mov rsi, rdx            ; rsi = accept (reset pointer to accept string)
.check_accept:
    movzx r9d, BYTE [rsi]   ; Load byte from accept into r9
    test r9b, r9b           ; Check if end of accept string ('\0')
    jz .next_char           ; If end of accept, move to next character in s

    cmp r8b, r9b            ; Compare s[i] with accept[j]
    je .found_match         ; If match found, set return pointer and exit

    inc rsi                 ; Move to next character in accept
    jmp .check_accept       ; Continue checking accept characters

.next_char:
    inc rcx                 ; Move to next character in s
    jmp .loop               ; Continue looping

.found_match:
    mov rax, rcx            ; Set return value to pointer where match was found
    pop rbp
    ret                     ; Return from function

.end:
    pop rbp
    ret                     ; Return NULL (end of s reached without match)
