section .text
global asm_strspn

; size_t asm_strspn(const char *s, const char *accept)
asm_strspn:
    push    rbp
    mov     rbp, rsp

    xor     rax, rax        ; Initialize counter to 0
    mov     r8, rdi         ; r8 = s (source string)
    mov     r9, rsi         ; r9 = accept (accept string)

    ; Loop through source string
.loop:
    movzx   edx, byte [r8]  ; Load byte from s into edx
    test    dl, dl          ; Check if end of string s ('\0')
    jz      .end            ; If end of s, exit loop

    mov     rcx, r9         ; rcx = accept (reset pointer to accept string)

.check_accept:
    ; Check each character in accept
    movzx   ebx, byte [rcx] ; Load byte from accept into ebx
    test    bl, bl          ; Check if end of accept string ('\0')
    jz      .increment_count ; If end of accept, increment count

    cmp     dl, bl          ; Compare s[i] with accept[j]
    je      .char_matched   ; If match found, go to char_matched

    inc     rcx             ; Move to next character in accept
    jmp     .check_accept   ; Continue checking accept string

.char_matched:
    inc     rax             ; Increment count (number of characters matched)
    inc     r8              ; Move to next character in s
    jmp     .loop           ; Continue looping through source string

.increment_count:
    inc     r8              ; Move to next character in s (no match found)
    jmp     .loop           ; Continue looping through source string

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