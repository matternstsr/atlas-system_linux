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
    je .found_null
    cmp dl, 0       ; Check if end of S2 ('\0')
    je .found_null
    cmp eax, edx    ; Check if n characters have been compared
    je .same        ; If n is zero, strings are equal up to n characters

    ; Compare bytes
    cmp al, dl
    jl .less_than   ; al < dl
    jg .greater_than ; al > dl

    ; Characters are equal, move to next
    inc rdi
    inc rsi
    inc eax         ; Increment counter for remaining characters
    jmp .asm_strncmp_loop  ; Continue loop

.found_null:
    ; Check if both strings are terminated simultaneously
    cmp eax, edx
    jne .not_same    ; If n characters haven't been compared fully, continue checking
    xor eax, eax     ; Otherwise, strings are equal up to n characters
    jmp .exit

.not_same:
    ; Compare the last characters
    cmp al, dl
    jl .less_than
    jg .greater_than
    xor eax, eax     ; Strings are equal up to current position
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
    ; Strings are equal up to n characters or both terminated
    xor eax, eax

.exit:
    pop rbp
    ret
