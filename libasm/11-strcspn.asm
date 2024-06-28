BITS 64

global asm_strcspn
section .text

; size_t asm_strcspn(const char *s, const char *reject)
asm_strcspn:
    push rbp
    mov rbp, rsp
    xor rax, rax            ; Initialize counter to 0
    mov rcx, rdi            ; rcx = s (source string)
    mov rdx, rsi            ; rdx = reject (reject string)

.loop:
    movzx r8d, BYTE [rcx]   ; Load byte from s into r8
    test r8b, r8b           ; Check if end of string s ('\0')
    jz .end

    mov rsi, rdx            ; rsi = reject (reset pointer to reject string)
    mov r9, rax             ; r9 = current length of matched segment

.check_reject:
    movzx r10d, BYTE [rsi]  ; Load byte from reject into r10
    test r10b, r10b         ; Check if end of reject string ('\0')
    jz .not_rejected        ; If end of reject, character not found in reject

    cmp r8b, r10b           ; Compare s[i] with reject[j]
    je .rejected            ; If match found, break and increment count

    inc rsi                 ; Move to next character in reject
    jmp .check_reject       ; Continue checking

.rejected:
    xor rax, rax            ; Reset counter to 0 (no matched segment)
    jmp .end                ; Exit loop

.not_rejected:
    inc rax                 ; Increment count (length of non-rejected segment)
    inc rcx                 ; Move to next character in s
    jmp .loop               ; Continue looping

.end:
    pop rbp
    ret                     ; Return from function
