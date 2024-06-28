section .text
global asm_memcpy

;; Function: asm_memcpy
;; Purpose: Copy n bytes from source buffer src to destination buffer dest
;; Arguments:
;;   rdi = dest (destination buffer)
;;   rsi = src (source buffer)
;;   rdx = n (number of bytes to copy)
;; Returns:
;;   rax = dest (pointer to the beginning of the destination buffer)
asm_memcpy:
    XOR ECX, ECX       ; Clear ECX register (ECX = 0)
    MOV EAX, EDI       ; Move destination pointer (EDI) to EAX (EAX = dest)

.loop:
    CMP ECX, EDX       ; Compare ECX (counter) with EDX (number of bytes to copy)
    JE end             ; If ECX == EDX, jump to end (copying finished)

    MOV EDX, [ESI]     ; Move 4 bytes from source pointer (ESI) to EDX
    MOV [EAX], EDX     ; Move 4 bytes from EDX to destination pointer (EAX)

    INC ESI            ; Increment source pointer (ESI)
    INC EAX            ; Increment destination pointer (EAX)
    JMP .loop          ; Jump back to .loop to continue copying

end:
    mov rax, rdi
    ret
