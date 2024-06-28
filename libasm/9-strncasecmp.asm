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

.loop:
    cmp rdx, 0              ; Check if n is 0
    je .end

    movzx r8d, BYTE [rdi]   ; Load byte from s1 into r8
    movzx r9d, BYTE [rsi]   ; Load byte from s2 into r9

    cmp r8b, 0              ; Check end of s1
    je .calc_result
    cmp r9b, 0              ; Check end of s2
    je .calc_result

    ; Convert r8 to lowercase if it's an uppercase letter
    cmp r8b, 65
    jl .check_lower1
    cmp r8b, 90
    jg .check_lower1
    add r8b, 32

.check_lower1:
    ; Convert r9 to lowercase if it's an uppercase letter
    cmp r9b, 65
    jl .compare
    cmp r9b, 90
    jg .compare
    add r9b, 32

.compare:
    ; Compare characters after conversion
    cmp r8b, r9b
    jne .calc_result         ; If not equal, calculate result

    ; Characters are equal, move to next
    inc rdi
    inc rsi
    dec rdx
    jmp .loop

.calc_result:
    ; Calculate result based on current characters
    xor eax, eax
    sub r8b, r9b
    movsx rax, r8b
    jmp .end

.end:
    pop rbp
    ret                     ; Return from function
