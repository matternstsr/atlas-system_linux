global asm_strspn
section .text

; size_t asm_strspn(const char *s, const char *accept)
asm_strspn:
    push rbp
    mov rbp, rsp
    xor rax, rax            ; Initialize counter to 0
    mov rcx, rdi            ; rcx = s (source string)
    mov rdx, rsi            ; rdx = accept (accept string)

.loop:
    movzx r8d, BYTE [rcx]   ; Load byte from s into r8
    test r8b, r8b           ; Check if end of string s ('\0')
    jz .end

    mov rsi, rdx            ; rsi = accept (reset pointer to accept string)

.check_accept:
    movzx r9d, BYTE [rsi]   ; Load byte from accept into r9
    test r9b, r9b           ; Check if end of accept string ('\0')
    jz .increment_count     ; If end of accept, increment count

    cmp r8b, r9b            ; Compare s[i] with accept[j]
    je .accept_found        ; If match found, increment and continue

    inc rsi                 ; Move to next character in accept
    jmp .check_accept       ; Continue checking

.accept_found:
    inc rax                 ; Increment count (length of matched segment)
    inc rcx                 ; Move to next character in s
    jmp .loop               ; Continue looping

.increment_count:
    inc rax                 ; Final increment to count if character matches accept

.end:
    pop rbp
    ret                     ; Return from function
