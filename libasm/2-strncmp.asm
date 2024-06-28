BITS 64

global asm_strncmp

section .text

asm_strncmp:
    ; Function prologue
    push rbp         ; Save base pointer
    mov rbp, rsp     ; Set up base pointer
    push rbx         ; Save rbx register which will be used in the function

next:
    ; Load characters from s1 (rdi) and s2 (rsi) into ebx (s1) and eax (s2)
    movzx ebx, byte [rdi]
    movzx eax, byte [rsi]
    
    ; Check for null termination of s1 (rdi)
    cmp bl, 0x00
    jz isnull        ; If s1 is null terminated, handle accordingly
    
    ; Compare characters in s1 (bl) and s2 (al)
    cmp bl, al
    jg ispos         ; If s1 > s2, jump to ispos
    jl isnega        ; If s1 < s2, jump to isnega
    
    ; Increment pointers to s1 (rdi) and s2 (rsi), decrement counter (rdx)
    inc rdi
    inc rsi
    dec rdx
    cmp rdx, 0
    ; Check if more characters need to be compared
    jnz isnull         ; If rdx is not zero, continue comparing
    jmp next; Loop it
    
isnull:
    ; Check for null termination of s2 (rsi)
    cmp bl, al
    jl isnega       ; If s1 < s2 and s1 is null terminated, jump to isnega
    
    ; Strings are equal up to n characters
    xor rax, rax    ; Set return value to 0 (equal)
    jmp endit         ; Jump to function epilogue

isnega:
    ; s1 < s2
    mov rax, -1     ; Set return value to -1
    jmp endit         ; Jump to function epilogue

ispos:
    ; s1 > s2
    mov rax, 1      ; Set return value to 1
    jmp endit         ; Jump to function epilogue

endit:
    ; Function epilogue
    pop rbx         ; Restore rbx register
    pop rbp         ; Restore base pointer
    ret             ; exit out
