BITS 64

section .text
global asm_strcasecmp

; Function: asm_strcasecmp
; Purpose: Compare two strings ignoring case
; Arguments:
;   rdi = s1 (string 1)
;   rsi = s2 (string 2)
; Returns:
;   rax = 0 if strings are equal ignoring case
;   rax > 0 if s1 > s2 ignoring case
;   rax < 0 if s1 < s2 ignoring case
;----------------------------------------

asm_strcasecmp:
    push rbp
    mov rbp, rsp
    push rbx
    push rdi
    push rsi

    ; Start comparing the strings
.loop:
    ; Load bytes from each string
    lodsb       ; Load byte from rdi into al and increment rdi
    xor ah, ah  ; Clear ah (for zero extension)

    lodsb       ; Load byte from rsi into al and increment rsi
    xor bh, bh  ; Clear bh (for zero extension)

    ; Convert characters to lowercase (assuming ASCII)
    cmp al, 65  ; 'A'
    jl .tolower_s1
    cmp al, 90  ; 'Z'
    jg .check_next_s1
    add al, 32  ; Convert to lowercase
.check_next_s1:

    cmp bl, 65  ; 'A'
    jl .tolower_s2
    cmp bl, 90  ; 'Z'
    jg .check_next_s2
    add bl, 32  ; Convert to lowercase
.check_next_s2:

    ; Compare characters
    cmp al, bl
    jne .done   ; If not equal, finish

    ; Check if end of strings
    test al, al
    jz .check_end_s2  ; s1 ended, check if s2 also ends

    ; Load next byte from s2 and check
    lodsb
    xor bl, bl
    test bl, bl
    jz .done    ; s1 and s2 are equal, s2 ended

    jmp .loop   ; Both strings continue, compare next characters

; Determine if one string is greater
.done:
    test al, al
    jnz .shorter_str  ; s1 not ended, check if s2 is shorter
    test bl, bl
    jz .longer_str    ; s1 ended, s2 is longer

.shorter_str:
    mov rax, -1       ; s1 < s2
    jmp .cleanup

.longer_str:
    mov rax, 1        ; s1 > s2
    jmp .cleanup

.cleanup:
    pop rsi
    pop rdi
    pop rbx
    pop rbp
    ret

; Convert character to lowercase
.tolower_s1:
    cmp al, 65  ; 'A'
    jl .check_next_s1
    cmp al, 90  ; 'Z'
    jg .check_next_s1
    add al, 32  ; Convert to lowercase
    jmp .check_next_s1

.tolower_s2:
    cmp bl, 65  ; 'A'
    jl .check_next_s2
    cmp bl, 90  ; 'Z'
    jg .check_next_s2
    add bl, 32  ; Convert to lowercase
    jmp .check_next_s2

