section .text

global asm_strstr

; Function: asm_strstr
; Purpose: Find the first occurrence of substring needle in haystack
; Arguments:
;   rdi = haystack (string to search)
;   rsi = needle (substring to find)
; Returns:
;   rax = pointer to the beginning of the found substring in haystack, or NULL if not found
;----------------------------------------
    ; Prologue: Save necessary registers and set up stack frame
    ; Check if either rdi (haystack) or rsi (needle) is NULL, return NULL if so
    ; Get lengths of rdi (haystack) and rsi (needle)
    ; Calculate initial positions and lengths for comparison
    ; Iterate through rdi to find rsi
        ; Compare substring starting at current position in rdi with rsi
        ; If found, calculate and return the pointer to the start of rsi in rdi
    ; If not found, return NULL
    ; Epilogue: Restore registers and return


asm_strstr:
    push rbp
    mov rbp, rsp
    push rbx
    push rdi
    push rsi
    push rdx

    ; Load parameters into registers
    mov rdi, rdi ; rdi = haystack (string to search)
    mov rsi, rsi ; rsi = needle (substring to find)

    ; If haystack or needle is NULL, return NULL
    test rdi, rdi
    jz .return_null
    test rsi, rsi
    jz .return_found_empty_needle  ; Jump to handle empty needle case

    ; Get lengths of haystack and needle
    mov rcx, rdi ; rcx = haystack (use rcx to iterate through haystack)
    mov rdx, rsi ; rdx = needle (use rdx to iterate through needle)

    ; Calculate lengths of haystack and needle
    mov r8, rax  ; r8 = length of haystack
    mov r9, rcx  ; r9 = length of needle

    ; If haystack length < needle length, return NULL
    cmp r8, r9
    jb .return_null

    ; Iterate through haystack to find needle
    mov rbx, r8  ; rbx = length of haystack - length of needle + 1
    sub rbx, r9
    inc rbx

.find_needle_loop:
    ; Compare substring starting at current position in haystack with needle
    mov r10, r9  ; r10 = length of needle
    mov rsi, rdi ; rsi = current position in haystack
    mov rcx, rdx ; rcx = pointer to needle
    repe cmpsb   ; Compare byte by byte

    ; Check if needle is found
    je .return_found

    ; Move to next position in haystack
    inc rdi

    ; Check if reached end of haystack
    dec rbx
    jz .return_null

    ; Continue searching
    jmp .find_needle_loop

.return_found:
    ; Calculate pointer to start of needle in haystack
    mov rax, rdi
    sub rax, r9
    jmp .exit

.return_found_empty_needle:
    ; Handle case where needle is empty ("")
    mov rax, rdi
    jmp .exit

.return_null:
    xor rax, rax  ; Return NULL (0)
    jmp .exit

.exit:
    pop rdx
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
