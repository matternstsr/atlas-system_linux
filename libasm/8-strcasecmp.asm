BITS 64

section .text
global asm_strcasecmp

;; Prototype: int asm_strcasecmp(const char *s1, const char *s2)
asm_strcasecmp:
    push rbp            ; Save base pointer
    mov rbp, rsp        ; Set up base pointer

    mov rsi, rdi        ; Move s1 (first argument) to rsi
    mov rdi, rdx        ; Move s2 (second argument) to rdi

.loop:
    mov al, [rsi]       ; Load byte from s1 into al
    mov dl, [rdi]       ; Load byte from s2 into dl

    ; Convert characters to lowercase if they are uppercase
    cmp al, 'A'
    jl .check_dl
    cmp al, 'Z'
    jg .check_dl
    add al, 32          ; Convert to lowercase (al = al + 32)

.check_dl:
    cmp dl, 'A'
    jl .compare_chars
    cmp dl, 'Z'
    jg .compare_chars
    add dl, 32          ; Convert to lowercase (dl = dl + 32)

.compare_chars:
    cmp al, dl          ; Compare characters
    jne .end_loop       ; Jump to end if characters are not equal
    cmp al, 0           ; Check if end of string (null terminator)
    je .strings_equal   ; If both strings are null-terminated, they are equal

    inc rsi             ; Move to next character in s1
    inc rdi             ; Move to next character in s2
    jmp .loop           ; Repeat loop

.strings_equal:
    xor eax, eax        ; Return 0 (strings are equal)
    jmp .cleanup

.end_loop:
    movsx eax, al       ; Move al to eax with sign extension
    sub eax, edx        ; Subtract dl from eax
    jmp .cleanup

.cleanup:
    pop rbp             ; Restore base pointer
    ret                 ; Return value in eax
