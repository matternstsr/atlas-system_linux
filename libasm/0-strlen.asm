global asm_strlen

section .text

asm_strlen:
    xor rax, rax          ; Clear rax (set rax to zero, which will hold the length)
.loop_start:
    movzx rcx, BYTE [rdi] ; Load the byte at the address stored in rdi into rcx
    test rcx, rcx         ; Test if rcx (current byte) is zero
    jz .end               ; If rcx is zero, jump to .end (end of string)

    inc rdi               ; Move to the next character (increment rdi)
    inc rax               ; Increment the length counter
    jmp .loop_start       ; Jump back to .loop_start to continue the loop

.end:
    ret                    ; Return with the length of the string in rax
