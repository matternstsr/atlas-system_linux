BITS 64

global asm_strncasecmp
section .text

; int asm_strncasecmp(const char *s1, const char *s2, size_t n)
asm_strncasecmp:
    push rbp
    mov rbp, rsp
    xor eax, eax            ; Initialize return value to 0 (equal)

    ; Arguments:
    ; rdi = s1, rsi = s2, rdx = n

.BOOP:
    cmp rdx, 0              ; Check if n is 0
    je .calc

    movzx r8d, BYTE [rdi]   ; Load byte from s1 into r8
    movzx r9d, BYTE [rsi]   ; Load byte from s2 into r9

    cmp r8b, 0              ; Check end of s1
    jnz .convert
    cmp r9b, 0              ; Check end of s2
    jnz .CHECK_LOWER1


.convert:
    ; Convert r8 to lowercase if it's an uppercase letter
    cmp r8b, 'A'
    jl .CHECK_LOWER1
    cmp r8b, 'Z'
    jg .CHECK_LOWER1
    add r8w, 32

.check_lower1:
    ; Convert r9 to lowercase if it's an uppercase letter
    cmp r9b, 'A'
    jl .COMPARE
    cmp r9b, 'Z'
    jg .COMPARE
    add r9w, 32

.compare:
    ; Compare characters after conversion
    cmp r8b, r9b
    jne .calc_result         ; If not equal, calculate result

    ; Characters are equal, move to next
    inc rdi
    inc rsi
    dec rdx
    jmp .BOOP

.calc_result:
    ; Calculate result based on current characters
    xor eax, eax
	sub r8, r9           ; 64 byte
	mov rax, r8          ; 64 byte
    jmp .END

.end:
    pop rbp
    ret                     ; Return from function
