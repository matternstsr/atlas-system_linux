section .text

extern asm_putc

global asm_puti

asm_puti:
    ; Function implementation here
    push rbp                ; Function prologue
    mov rbp, rsp
    
    sub rsp, 16             ; Allocate space for local variables
    
    mov qword [rbp-8], rdi  ; Store argument n in local variable space
    
    xor rax, rax            ; rax will store the number of bytes written
    xor rcx, rcx            ; rcx will be used as a counter
    
    ; Check if n is negative
    cmp qword [rbp-8], 0
    jge .check_positive     ; Jump if n >= 0
    
    ; Print the minus sign for negative numbers
    mov rdi, '-'            ; Prepare the character '-'
    call asm_putc           ; Call asm_putc to print the '-'
    inc rax                 ; Increment rax for the minus sign
    
    ; Convert n to positive
    mov rax, qword [rbp-8]
    neg rax                 ; Negate rax to make it positive
    mov qword [rbp-8], rax  ; Store the positive value back
    
.check_positive:
    ; Convert n to string and print each character
    mov rax, qword [rbp-8]  ; Load n into rax
    mov rbx, 10             ; Divider (base 10)
    mov rcx, 0              ; Initialize counter to zero
    
    ; Edge case: if n is zero, handle it separately
    test rax, rax
    jz .handle_zero
    
.convert_loop:
    xor rdx, rdx            ; Clear rdx for division
    div rbx                 ; Divide rax by 10, quotient in rax, remainder in rdx
    
    add dl, '0'             ; Convert remainder to ASCII digit
    mov byte [rsp + rcx], dl  ; Store the digit in the temporary buffer (use byte instead of qword)
    inc rcx                 ; Increment counter
    
    test rax, rax           ; Check if quotient is zero
    jnz .convert_loop       ; If not zero, continue conversion
    
.handle_zero:
    ; If original number was zero, directly print '0'
    cmp rcx, 0
    jz .print_zero
    
    ; Print characters in reverse order
.print_loop:
    dec rcx                 ; Move back one character in the buffer
    mov rdi, byte [rsp + rcx]  ; Load character to print into rdi (use byte instead of qword)
    call asm_putc           ; Print the character
    inc rax                 ; Increment byte count
    
    cmp rcx, 0              ; Check if all characters printed
    jg .print_loop          ; If more to print, continue
    
.print_zero:
    ; Print newline character
    mov rdi, 0x0A           ; ASCII code for newline
    call asm_putc           ; Call asm_putc to print the newline
    inc rax                 ; Increment byte count for newline
    
    add rsp, 16             ; Restore stack pointer
    pop rbp                 ; Function epilogue
    ret                     ; Return
