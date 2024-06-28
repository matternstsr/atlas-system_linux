global asm_strchr
section .text

asm_strchr:
    push rbp
    mov rbp, rsp
    push rbx

.get_next_char:
    movzx ebx, BYTE [rdi]   ; Load the byte from memory address in rdi into ebx (s)
    movzx eax, BYTE [rsi]   ; Load the byte from memory address in rsi into eax (c)

.run_loop:
    cmp bl, 0x00            ; Compare the byte in ebx (s) with 0x00 (end-of-string)
    jz .null_found          ; If s[i] == 0x00, jump to .null_found

    cmp bl, al              ; Compare the byte in ebx (s) with eax (c)
    jz .return_found        ; If s[i] == c, jump to .return_found

    inc rdi                 ; Increment the pointer to s (rdi) to move to the next character
    movzx ebx, BYTE [rdi]   ; Load the next byte from memory into ebx (s)
    jmp .run_loop           ; Jump back to .run_loop to continue searching

.null_found:
    cmp al, 0x00            ; Check if c == 0x00 (end-of-search character)
    jz .return_found        ; If c is end-of-search character, jump to .return_found
    xor rax, rax            ; Otherwise, set return value rax to 0 (not found)
    jmp .exit               ; Jump to .exit

.return_found:
    xor rax, rax            ; Set return value rax to 0 (not found)
    mov rdi, rax            ; Set rdi (return value convention) to 0 (not found)
    jmp .exit               ; Jump to .exit

.exit:
    pop rbx                 ; Restore rbx
    pop rbp                 ; Restore rbp
    ret                     ; Return from asm_strchr function
