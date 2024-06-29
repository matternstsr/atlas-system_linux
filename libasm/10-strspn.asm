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
.Boop:
    movzx   edx, byte [r8]  ; Load byte from s into edx
    test    dl, dl          ; Check if end of string s ('\0')
    jz      .endit            ; If end of s, exit loop

    mov     rcx, r9         ; rcx = accept (reset pointer to accept string)

.checkchar:
    ; Check each character in accept
    movzx   ebx, byte [rcx] ; Load byte from accept into ebx
    test    bl, bl          ; Check if end of accept string ('\0')
    jz      .upcount ; If end of accept, increment count

    cmp     dl, bl          ; Compare s[i] with accept[j]
    je      .matched   ; If match found, go to char_matched

    inc     rcx             ; Move to next character in accept
    jmp     .checkchar   ; Continue checking accept string

.matched:
    inc     rax             ; Increment count (number of characters matched)
    inc     r8              ; Move to next character in s
    jmp     .Boop           ; Continue looping through source string

.upcount:
    jmp     .endit          ; If no match found, end the loop

.endit:
    pop     rbp
    ret                     ; Return from function



;Correct output - case: asm_strspn("Holberton", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
;Correct output - case: asm_strspn("Holberton", "abcdefghijklmopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
;Correct output - case: asm_strspn("Holberton", "0123456789");
;inorrect output - case: asm_strspn("Holberton School", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
;Correct output - case: asm_strspn("Holberton School", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ");
;Correct output - case: asm_strspn("Holberton School", "");
;Correct output - case: asm_strspn("", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ");
;Correct output - case: asm_strspn(s, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ,. "); s being a very long string