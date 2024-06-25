global asm_strcmp
section .text
asm_strcmp:
    push rbp
    mov rbp, rsp
    mov rdi, rdi

    ; Load pointers to strings
    mov rsi, rsi
    mov rdx, rdx

    ; Loop through strings
.loop:
    ; Load characters
    mov al, BYTE [rsi]
    mov bl, BYTE [rdx]

    ; Compare characters
    cmp al, bl
    jne .compare_result

    ; Check for end of strings
    test al, al
    jz .compare_result

    ; Move to next characters
    add rsi, 1
    add rdx, 1
    jmp .loop

.compare_result:
    ; Compare the characters
    movzx eax, al
    sub eax, ebx

    ; Prepare return value
    test eax, eax
    jne .return

    ; Check if we reached end of both strings
    test al, al
    jz .return_0

    ; If not, determine which string is longer
    test bl, bl
    jz .return_1

    mov eax, 1
    jmp .return

.return_0:
    mov eax, 0
.return:
    pop rbp
    ret
