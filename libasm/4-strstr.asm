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
    push rdx

    ; If haystack or needle is NULL, return NULL
    test rdi, rdi
    jz .return_null
    test rsi, rsi
    jz .return_null

    ; Get lengths of haystack and needle
    xor rcx, rcx  ; rcx = 0 (used for haystack length calculation)
    xor rdx, rdx  ; rdx = 0 (used for needle length calculation)
    
    ; Calculate length of haystack
    mov rax, rdi
    .calculate_haystack_length:
        cmp byte [rax + rcx], 0  ; Check for null terminator
        je .haystack_length_done
        inc rcx
        jmp .calculate_haystack_length
    .haystack_length_done:
    
    ; Calculate length of needle
    mov rax, rsi
    .calculate_needle_length:
        cmp byte [rax + rdx], 0  ; Check for null terminator
        je .needle_length_done
        inc rdx
        jmp .calculate_needle_length
    .needle_length_done:

    ; If needle length is 0, return haystack (because an empty needle is always found at the beginning)
    test rdx, rdx
    jz .return_haystack

    ; If haystack length < needle length, return NULL
    cmp rcx, rdx
    jb .return_null

    ; Iterate through haystack to find needle
    mov rbx, rcx  ; rbx = length of haystack - length of needle + 1

.find_needle_loop:
    ; Compare substring starting at current position in haystack with needle
    mov r10, rdx  ; r10 = length of needle
    mov rsi, rdi  ; rsi = current position in haystack
    mov rcx, rsi  ; rcx = current position in haystack (for repe cmpsb)
    repe cmpsb    ; Compare byte by byte

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
    sub rax, rdx
    jmp .exit

.return_null:
    xor rax, rax  ; Return NULL (0)
    jmp .exit

.return_haystack:
    mov rax, rdi  ; Return haystack pointer
    jmp .exit

.exit:
    pop rdx
    pop rsi
    pop rdi
    pop rbx
    pop rbp
    ret
