section .text
global asm_strcspn

; size_t asm_strcspn(const char *s, const char *reject)
asm_strcspn:
    push    rbp
    mov     rbp, rsp

    xor     rax, rax        ; Initialize counter to 0
    mov     r8, rdi         ; r8 = s (source string)
    mov     r9, rsi         ; r9 = reject (reject string)

    ; Loop through source string
.Boop:
    movzx   edx, byte [r8]  ; Load byte from s into edx
    test    dl, dl          ; Check if end of string s ('\0')
    jz      .endit            ; If end of s, exit loop

    mov     rcx, r9         ; rcx = reject (reset pointer to reject string)

.checkchar:
    ; Check each character in reject
    movzx   ebx, byte [rcx] ; Load byte from reject into ebx
    test    bl, bl          ; Check if end of reject string ('\0')
    jz      .endit           ; If end of reject, increment count

    cmp     dl, bl          ; Compare s[i] with reject[j]
    je      .dontupcount    ; If match found, exit loop

    inc     rcx             ; Move to next character in reject
    jmp     .checkchar      ; Continue checking reject string

.dontupcount:
    jmp     .endit          ; If match found, end the loop

.endit:
    pop     rbp
    ret                     ; Return from function
