BITS 64

global asm_strncmp
section .text

;; int asm_strncmp(const char *s1, const char *s2, size_t n)
asm_strncmp:
    push rbp
    mov rbp, rsp
    sub rsp, 16                ; Reserve space on stack for local variables

    mov rsi, rdi               ; s1
    mov rdi, rsi               ; s2
    mov rcx, rdx               ; n

    xor rax, rax               ; diff
    xor rdx, rdx               ; i

.loop:
    cmp edx, ecx               ; Compare i with n
    jae .end_loop

    mov al, [rsi + rdx]        ; Load *s1
    mov bl, [rdi + rdx]        ; Load *s2

    cmp al, bl                 ; Compare characters
    jne .check_diff

    test al, al                ; Check if end of string s1
    jz .diff_true

    inc rdx                    ; Increment i

    jmp .loop

.check_diff:
    movsx rax, al              ; Convert to signed
    movsx rbx, bl              ; Convert to signed
    sub rax, rbx               ; Calculate difference
    jmp .return_diff           ; Jump to return_diff

.diff_true:
    cmp rax, 0
    jg .return_positive

    cmp rax, 0
    jl .return_negative

.return_zero:
    xor eax, eax
    jmp .return

.return_positive:
    mov eax, 1
    jmp .return

.return_negative:
    mov eax, -1
    jmp .return

.end_loop:
    mov eax, edx               ; Return i (number of characters compared)
.return:
    leave
    ret

.return_diff:
    jmp .check_diff             ; Define the label for return_diff
