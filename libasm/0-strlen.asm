section .text
    global asm_strlen

asm_strlen:
    push rbp
    mov rbp, rsp
    mov rdi, QWORD [rbp + 16]    ; Load the argument (str) into rdi

    xor rax, rax                 ; Clear rax (use rax for length)
    test rdi, rdi                ; Check if str pointer is NULL
    jz .end                      ; If NULL, return 0

.loop:
    lodsb                        ; Load byte at [DS:RSI] into AL, increment RSI
    test al, al                  ; Check if AL (current byte) is zero (null terminator)
    jz .end                      ; If zero, end of string, exit loop
    inc rax                      ; Increment length counter
    jmp .loop                    ; Repeat until null terminator found

.end:
    pop rbp                      ; Restore rbp
    ret                          ; Return with rax holding the length of str
