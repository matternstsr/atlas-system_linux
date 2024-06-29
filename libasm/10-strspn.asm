section .text
global asm_strspn

; size_t asm_strspn(const char *s, const char *accept)
asm_strspn:
    push    rbp
    mov     rbp, rsp

    xor     rax, rax        ; Initialize counter to 0
    mov     r8, rdi         ; r8 = s (source string)
    mov     r9, rsi         ; r9 = accept (accept string)

.loop:
    movzx   edx, byte [r8]  ; Load byte from s into edx
    test    dl, dl          ; Check if end of string s ('\0')
    jz      .end

    mov     rcx, r9         ; rcx = accept (reset pointer to accept string)

.check_accept:
    movzx   ebx, byte [rcx] ; Load byte from accept into ebx
    test    bl, bl          ; Check if end of accept string ('\0')
    jz      .increment_count ; If end of accept, increment count

    cmp     dl, bl          ; Compare s[i] with accept[j]
    je      .accept_found   ; If match found, increment and continue

    inc     rcx             ; Move to next character in accept
    jmp     .check_accept   ; Continue checking

.accept_found:
    inc     rax             ; Increment count (length of matched segment)
    inc     r8              ; Move to next character in s
    jmp     .loop           ; Continue looping

.increment_count:
    inc     rax             ; Final increment to count if character matches accept

.end:
    pop     rbp
    ret                     ; Return from function


;Correct output - case: asm_strspn("Holberton", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
;inorrect output - case: asm_strspn("Holberton", "abcdefghijklmopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
;inorrect output - case: asm_strspn("Holberton", "0123456789");
;inorrect output - case: asm_strspn("Holberton School", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
;Correct output - case: asm_strspn("Holberton School", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ");
;inorrect output - case: asm_strspn("Holberton School", "");
;Correct output - case: asm_strspn("", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ");
;Correct output - case: asm_strspn(s, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ,. "); s being a very long string