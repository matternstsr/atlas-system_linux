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

    ; Convert edx (s[i]) to lowercase if it's an uppercase letter
    cmp     dl, 'A'
    jb      .check_accept   ; If character is not a letter, skip conversion
    cmp     dl, 'Z'
    ja      .check_accept   ; If character is not a letter, skip conversion
    add     dl, 32          ; Convert uppercase letter to lowercase

.check_accept:
    ; Check each character in accept
    movzx   ebx, byte [rcx] ; Load byte from accept into ebx
    test    bl, bl          ; Check if end of accept string ('\0')
    jz      .increment_count ; If end of accept, increment count

    ; Convert ebx (accept[j]) to lowercase if it's an uppercase letter
    cmp     bl, 'A'
    jb      .cmp_characters ; If character is not a letter, skip conversion
    cmp     bl, 'Z'
    ja      .cmp_characters ; If character is not a letter, skip conversion
    add     bl, 32          ; Convert uppercase letter to lowercase

.cmp_characters:
    cmp     dl, bl          ; Compare s[i] with accept[j]
    je      .accept_found   ; If match found, increment and continue

    inc     rcx             ; Move to next character in accept
    jmp     .check_accept   ; Continue checking accept string

.accept_found:
    inc     rax             ; Increment count (length of matched segment)
    inc     r8              ; Move to next character in s
    jmp     .loop           ; Continue looping through source string

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