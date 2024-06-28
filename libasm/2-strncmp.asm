BITS 64

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
    cmp al, 0       ; Check if end of S1 ('\0')
    je .end_of_s1
    cmp dl, 0       ; Check if end of S2 ('\0')
    je .end_of_s2

    ; Compare bytes
    cmp al, dl
    jl .less_than   ; al < dl
    jg .greater_than ; al > dl

    ; Characters are equal, move to next
    inc rdi
    inc rsi
    dec rdx        ; Decrement counter for remaining characters
    jnz .asm_strncmp_loop  ; Continue loop if counter is not zero

    ; If we exit the loop, it means the strings are equal up to n characters
    jmp .same

.end_of_s1:
    ; End of S1 reached
    cmp dl, 0       ; Check if end of S2 ('\0')
    je .same        ; S1 and S2 are both at end (equal up to n)
    mov eax, -1     ; S1 is shorter (S1 < S2)
    jmp .exit

.end_of_s2:
    ; End of S2 reached
    cmp al, 0       ; Check if end of S1 ('\0')
    je .same        ; S1 and S2 are both at end (equal up to n)
    mov eax, 1      ; S2 is shorter (S1 > S2)
    jmp .exit

.less_than:
    ; S1 < S2
    mov eax, -1
    jmp .exit

.greater_than:
    ; S1 > S2
    mov eax, 1
    jmp .exit

.same:
    ; Strings are equal up to n characters
    xor eax, eax


.exit:
    pop rbp
    ret
