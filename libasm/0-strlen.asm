BITS 64

global asm_strlen

section .text

asm_strlen:
    push rbp                    ; Save the base pointer
    mov rbp, rsp                ; Set up the base pointer

    mov QWORD [rbp - 8], 0      ; Initialize len to 0

.loop:
    mov rax, QWORD [rbp + 16]   ; Load str into rax (first argument)
    movzx eax, byte [rax]       ; Load byte at [rax] into eax, zero-extend to 32 bits
    test al, al                 ; Check if AL (current byte) is zero (null terminator)
    jz .end                     ; If zero, exit loop

    add QWORD [rbp - 8], 1      ; Increment len
    add QWORD [rbp + 16], 1     ; Move to the next character in str
    jmp .loop                   ; Repeat loop

.end:
    mov rax, QWORD [rbp - 8]    ; Load len into rax
    pop rbp                     ; Restore rbp
    ret                         ; Return with rax holding the length of str
