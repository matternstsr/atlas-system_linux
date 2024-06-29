section .text
global asm_strcspn

; size_t asm_strcspn(const char *s, const char *reject)
asm_strcspn:
    push    rbp             ; Prologue: save the base pointer
    mov     rbp, rsp        ; Set up the base pointer

    xor     rax, rax        ; Initialize counter to 0 (rax will store the count)
    mov     rcx, rdi        ; rcx = s (source string)
    mov     rdx, rsi        ; rdx = reject (reject string)

.loop_s:
    movzx   r8d, BYTE [rcx] ; Load byte from s into r8d
    test    r8b, r8b        ; Check if end of string s ('\0')
    jz      .end_loop_s     ; If end of s, exit loop

    mov     rsi, rdx        ; rsi = reject (reset pointer to reject string)

.loop_reject:
    movzx   r9d, BYTE [rsi] ; Load byte from reject into r9d
    test    r9b, r9b        ; Check if end of reject string ('\0')
    jz      .not_rejected   ; If end of reject, character not found in reject

    cmp     r8b, r9b        ; Compare s[i] with reject[j]
    je      .rejected       ; If match found, break and increment count

    inc     rsi             ; Move to next character in reject
    jmp     .loop_reject    ; Continue checking

.rejected:
    cmp     rax, 1          ; Check if rax > 1
    jl      .clear_counter  ; If greater than 1, clear counter

    cmp     rax, 2          ; Check if rax > 1
    jg      .notclear_counter  ; If greater than 1, clear counter

    jmp     .end_loop_s     ; Exit loop after finding a rejected character

.notclear_counter:
    jmp     .end_loop_s     ; Exit loop

.clear_counter:
    xor     rax, rax        ; Reset counter to 0 (no matched segment)
    jmp     .end_loop_s     ; Exit loop

.not_rejected:
    inc     rax             ; Increment count (length of non-rejected segment)
    inc     rcx             ; Move to next character in s
    jmp     .loop_s         ; Continue looping through s

.end_loop_s:
    pop     rbp             ; Epilogue: restore the base pointer
    ret                     ; Return from function
