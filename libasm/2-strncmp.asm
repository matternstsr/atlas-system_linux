;int asm_strncmp(const char *s1, const char *s2, size_t n);

global asm_strncmp
section .text

asm_strncmp:
    push rbp
    mov rbp, rsp

    ; Arguments:
    ; rdi = first string (S1)
    ; rsi = second string (S2)
    ; rdx = size_t n (number of characters to compare)

    xor eax, eax   ; Initialize eax (return value) to 0

    ; Loop through the strings
.asm_strncmp_loop:
    ; Load byte from S1 and S2
    mov al, byte [rdi]
    mov dl, byte [rsi]

    ; Compare bytes or check termination conditions
    cmp al, 0x00       ; Check if end of S1 ('\0')
    je .end_of_s1
    cmp dl, 0x00       ; Check if end of S2 ('\0')
    je .end_of_s2

    ; Compare bytes
    cmp al, dl
    jl .less_than   ; al < dl
    jg .greater_than ; al > dl

    ; Characters are equal, move to next
    inc rdi
    inc rsi
    dec rdx
    cmp rdx, 0    ; Check if reached n
    jz .equal

    jmp .asm_strncmp_loop

.less_than:
    ; S1 < S2
    xor eax, eax
    mov eax, -1
    jmp .exit

.greater_than:
    ; S1 > S2
    xor eax, eax
    mov eax, 1
    jmp .exit

.equal:
    ; Strings are equal up to n characters
    xor eax, eax
    jmp .exit

.end_of_s1:
    ; End of S1 reached
    cmp dl, 0       ; Check if also end of S2
    je .equal       ; Both are null, strings are equal
    mov eax, -1     ; S1 is null, S2 is not null
    jmp .exit

.end_of_s2:
    ; End of S2 reached
    mov eax, 1      ; S1 is not null, S2 is null
    jmp .exit

.exit:
    pop rbp
    ret
