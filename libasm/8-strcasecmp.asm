section .text
global asm_strcasecmp

; Arguments:
;   rdi = pointer to first string (S1)
;   rsi = pointer to second string (S2)
;   rdx = size_t n (number of characters to compare)
;   Loop through the strings

;; int asm_strcasecmp(const char *s1, const char *s2);

asm_strcasecmp:
    xor eax, eax        ; Clear eax (return value)
    xor rcx, rcx        ; Clear rcx (index)
    jmp .LOOP           ; Jump to start of comparison loop

.INCR:
    cmp bl, 0           ; Check if bl (current char in str1) is '\0' (end of string)
    je .END             ; If yes, jump to end of function

    inc rcx             ; Increment index rcx

.LOOP:
    mov bl, BYTE [rdi+rcx]  ; Load byte from str1[rcx] into bl
    mov dl, BYTE [rsi+rcx]  ; Load byte from str2[rcx] into dl

    cmp bl, 'A'         ; Compare bl with 'A'
    jl .NO_UPS1         ; Jump if bl < 'A' (not an uppercase letter)
    cmp bl, 'Z'         ; Compare bl with 'Z'
    jg .NO_UPS1         ; Jump if bl > 'Z' (not an uppercase letter)
    add bl, 32          ; Convert bl to lowercase

.NO_UPS1:
    cmp dl, 'A'         ; Compare dl with 'A'
    jl .NO_UPS2         ; Jump if dl < 'A' (not an uppercase letter)
    cmp dl, 'Z'         ; Compare dl with 'Z'
    jg .NO_UPS2         ; Jump if dl > 'Z' (not an uppercase letter)
    add dl, 32          ; Convert dl to lowercase

.NO_UPS2:
    cmp bl, dl          ; Compare bl and dl (both lowercase now)
    je .INCR            ; If equal, jump to increment index

.DIFF:
    movsx eax, bl       ; Move bl (as integer) into eax
    movsx ebx, dl       ; Move dl (as integer) into ebx
    sub eax, ebx        ; Subtract ebx from eax (character difference)

.END:
    ret                 ; Return from function
