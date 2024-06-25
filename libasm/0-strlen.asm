section .text
    global asm_strlen          ; Define asm_strlen as a global symbol

asm_strlen:
    xor rax, rax               ; Clear rax (counter for string length)
    test rdi, rdi              ; Check if str is NULL
    jz .end                    ; If str is NULL, return 0
    
.loop:
    lodsb                      ; Load byte at [DS:RSI] into AL, increment RSI
    test al, al                ; Check if AL (current byte) is zero
    jz .end                    ; If zero, end of string, exit loop
    inc rax                    ; Increment length counter
    jmp .loop                  ; Repeat until null terminator found
    
.end:
    dec rax                    ; Adjust length by decrementing by 1 (since we counted null terminator)
    ret                        ; Return with rax holding string length
