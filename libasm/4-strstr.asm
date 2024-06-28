section .text

global asm_strstr

; Function: asm_strstr
; Purpose: Find the first occurrence of substring needle in haystack
; Arguments:
;   rdi = haystack (string to search)
;   rsi = needle (substring to find)
; Returns:
;   rax = pointer to the beginning of the found substring in haystack, or NULL if not found

asm_strstr:
    push rbp
    mov rbp, rsp
    push rbx
    push rdi
    push rsi

    ; If haystack or needle is NULL, return NULL
    test rdi, rdi
    jz .return_null
    test rsi, rsi
    jz .return_null

    ; Get lengths of haystack and needle
    xor rcx, rcx  ; rcx = 0 (counter for haystack length)
    xor rdx, rdx  ; rdx = 0 (counter for needle length)

    ; Calculate length of haystack
    mov rdi, rdi  ; rdi = haystack (use rdi for iteration)
.loop_haystack_length:
    cmp byte [rdi + rcx], 0
    je .end_haystack_length
    inc rcx
    jmp .loop_haystack_length
.end_haystack_length:

    ; Calculate length of needle
    mov rsi, rsi  ; rsi = needle (use rsi for iteration)
.loop_needle_length:
    cmp byte [rsi + rdx], 0
    je .end_needle_length
    inc rdx
    jmp .loop_needle_length
.end_needle_length:

    ; If haystack length < needle length, return NULL
    cmp rcx, rdx
    jb .return_null

    ; Iterate through haystack to find needle
    mov rbx, rcx  ; rbx = length of haystack
    sub rbx, rdx  ; rbx = length of haystack - length of needle

.find_needle_loop:
    ; Compare substring starting at current position in haystack with needle
    mov rdi, rbp  ; reset rdi to original haystack pointer
    mov rsi, rsi  ; rsi = needle (reset rsi)
    mov rcx, rdx  ; rcx = length of needle
    repe cmpsb     ; Compare byte by byte

    ; Check if needle is found
    je .return_found

    ; Move to next position in haystack
    inc rbp

    ; Check if reached end of haystack
    dec rbx
    jz .return_null

    ; Continue searching
    jmp .find_needle_loop

.return_found:
    ; Calculate pointer to start of needle in haystack
    lea rax, [rbp - rdx]
    jmp .exit

.return_null:
    xor rax, rax  ; Return NULL (0)
    jmp .exit

.exit:
    pop rsi
    pop rdi
    pop rbx
    pop rbp
    ret




;Correct output - case: asm_strstr("Holberton School", " Scc")
;Correct output - case: asm_strstr("Holberton School", "SSc")
;Correct output - case: asm_strstr("", "Holberton")
;Correct output - case: asm_strstr(s1, s2), s1 being a very long string and s2 a word absent from s1
;Correct output - case: asm_strstr(s1, s2), s1 being a very long string, and s2 the same string with an extra character
