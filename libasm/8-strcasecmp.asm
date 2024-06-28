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
    mov rbp, rsp  ; Setup stack frame

    xor rax, rax  ; Clear rax (result register)
    push rbx      ; Preserve rbx
    push rcx      ; Preserve rcx

.loop:
    ; Load bytes from each string
    movzx ebx, BYTE [rdi]  ; Load byte from rdi into ebx
    movzx ecx, BYTE [rsi]  ; Load byte from rsi into ecx

    ; Convert characters to lowercase (assuming ASCII)
    cmp bl, 65  ; 'A'
    jl .check_next_s1  ; If less than 'A', skip lowercase conversion
    cmp bl, 90  ; 'Z'
    jg .tolower_s1_done  ; If greater than 'Z', skip lowercase conversion
    add bx, 32  ; Convert to lowercase
.check_next_s1:

    cmp cl, 65  ; 'A'
    jl .check_next_s2  ; If less than 'A', skip lowercase conversion
    cmp cl, 90  ; 'Z'
    jg .tolower_s2_done  ; If greater than 'Z', skip lowercase conversion
    add cx, 32  ; Convert to lowercase
.check_next_s2:

    ; Compare characters
    cmp bl, cl
    jne .done   ; If not equal, finish

    ; Check if end of strings
    test bl, bl
    jz .check_end_s2  ; s1 ended, check if s2 also ends

    ; Increment pointers
    inc rdi
    inc rsi
    jmp .loop   ; Continue comparing

.check_end_s2:
    test cl, cl
    jz .done    ; s1 ended, s2 also ended

.done:
    sub rbx, rcx  ; Calculate difference in lengths
    mov rax, rbx  ; Move result to rax

    pop rcx  ; Restore rcx
    pop rbx  ; Restore rbx
    pop rbp  ; Restore rbp
    ret      ; Return

; Convert character to lowercase
.tolower_s1_done:
    ret

.tolower_s2_done:
    ret
