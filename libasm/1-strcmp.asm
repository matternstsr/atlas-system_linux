global asm_strcmp
section .text

asm_strcmp:
    ; Initialize pointers
    mov rdi, rdi    ; rdi: s1
    mov rsi, rsi    ; rsi: s2
    
.loop_compare:
    ; Load characters from s1 and s2
    mov al, byte [rdi]
    mov dl, byte [rsi]
    
    ; Compare characters
    test al, al
    jz .check_s2_null   ; If *s1 is null, check if *s2 is also null
    test dl, dl
    jz .decide_result   ; If *s2 is null, decide result
    
    ; Compare *s1 and *s2
    cmp al, dl
    je .continue_loop   ; If characters are equal, continue loop
    
    ; Determine return value
    jl .s1_less_s2      ; *s1 < *s2
    jg .s1_greater_s2   ; *s1 > *s2
    
.continue_loop:
    ; Increment pointers
    inc rdi
    inc rsi
    jmp .loop_compare

.check_s2_null:
    ; Check if *s2 is null
    test dl, dl
    jnz .decide_result  ; *s2 is not null, decide result
    
    ; Both strings are null-terminated and equal
    xor eax, eax        ; return 0
    jmp .return

.decide_result:
    ; Decide result based on comparison
    jl .s1_less_s2      ; *s1 < *s2
    jg .s1_greater_s2   ; *s1 > *s2
    
    ; Strings are equal
    xor eax, eax        ; return 0
    jmp .return

.s1_less_s2:
    ; *s1 < *s2
    mov eax, -1         ; return -1
    jmp .return

.s1_greater_s2:
    ; *s1 > *s2
    mov eax, 1          ; return 1
    jmp .return

.return:
    ret
