section .text
    global asm_strlen

asm_strlen:
    push rbp
    mov rbp, rsp
    mov rdi, QWORD [rbp + 16]    ; Load the argument (str) into rdi

    xor eax, eax                 ; Clear eax (lower 32-bit of rax) for len
    jmp .test_null

.loop:
    add eax, 1                   ; Increment len
    add rdi, 1                   ; Move to the next character in str

.test_null:
    movzx edx, byte [rdi]        ; Load the byte at [rdi] into edx (zero-extend to 32 bits)
    test dl, dl                  ; Check if the byte is null terminator ('\0')
    jne .loop                    ; If not null, continue looping

    pop rbp                      ; Restore rbp
    ret                          ; Return with eax holding the length of str
