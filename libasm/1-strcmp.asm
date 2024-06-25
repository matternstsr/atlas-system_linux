global asm_strcmp

section .text

asm_strcmp:
    ; Prologue
    push rbp     ; Save the current base pointer onto the stack
    mov rbp, rsp ; Set up a new base pointer

    ; Arguments passed in registers:
    ; rdi = s1 (first string)
    ; rsi = s2 (second string)

    xor rcx, rcx        ; Initialize rcx to zero (counter for the loop)

.loop_compare:
    ; Load characters from s1 and s2
    mov al, [rdi + rcx] ; Load byte from s1 into al
    mov dl, [rsi + rcx] ; Load byte from s2 into dl

    ; Compare characters
    cmp al, dl
    jne .end_loop       ; If characters are not equal, exit loop

    ; Check if end of string (null terminator)
    test al, al         ; Check if al (current char of s1) is zero
    jz .end_loop        ; If zero, end of both strings reached

    ; Increment counter and loop
    inc rcx             ; rcx is the register that is being incremented
    jmp .loop_compare   ; "goto" begining (loop_compare)

.end_loop:
    ; Calculate result based on comparison
    mov rax, 0
    mov al, [rdi + rcx] ; Load next character from s1
    sub al, [rsi + rcx] ; Subtract character from s2
    ret

    ; Epilogue
    pop rbp             ; used to remove (pop) the topmost value
