BITS 64  ; Set the mode to 64-bit

global asm_strstr
section .text

;; Function: asm_strstr
;; Purpose: Find the first occurrence of substring needle in haystack
;; Arguments:
;;   rdi = haystack (string to search)
;;   rsi = needle (substring to find)
;; Returns:
;;   rax = pointer to the beginning of the found substring in haystack, or NULL if not found
;;----------------------------------------

asm_strstr:
    push rbp        ; Prologue: Save current base pointer
    mov rbp, rsp    ; Set up base pointer

.get_first_char:
    movzx ebx, BYTE [rdi]  ; Load first byte of haystack into ebx
    movzx eax, BYTE [rsi]  ; Load first byte of needle into eax
    cmp al, 0x00           ; Check if needle is empty (null terminator)
    je .return_found       ; If needle is empty, return immediately

.outter_loop:
    cmp bl, 0x00           ; Check if end of haystack (null terminator)
    jz .outter_null_found  ; If end of haystack, go to handling null found

    cmp bl, al             ; Compare current character in haystack with first character of needle
    je .inner_loop_setup   ; If match, set up inner loop

    inc rdi                ; Move to next character in haystack
    movzx ebx, BYTE [rdi]  ; Load next byte of haystack
    jmp .outter_loop       ; Continue outer loop

.inner_loop_setup:
    mov r8, rdi            ; Save current position in haystack (rdi) to r8
    mov r9, rsi            ; Save pointer to needle (rsi) to r9
    inc r8                 ; Move to next character in haystack
    inc r9                 ; Move to next character in needle
    movzx r10d, BYTE [r8]  ; Load next byte of haystack into r10d
    movzx r11d, BYTE [r9]  ; Load next byte of needle into r11d
    cmp r10b, r11b         ; Compare bytes
    je .run_inner_loop     ; If equal, run inner loop
    cmp r11b, 0x00         ; Check if end of needle (null terminator)
    jz .return_found       ; If end of needle, needle is found

    jmp .back_to_outter    ; Continue outer loop

.run_inner_loop:
    cmp r10b, 0x00         ; Check if end of haystack (null terminator)
    jz .inner_null_found   ; If end of haystack, go to handling inner null found
    cmp r11b, 0x00         ; Check if end of needle (null terminator)
    jz .return_found       ; If end of needle, needle is found

    cmp r10b, r11b         ; Compare bytes
    jne .back_to_outter    ; If not equal, go back to outer loop

    inc r8                 ; Move to next character in haystack
    inc r9                 ; Move to next character in needle
    movzx r10d, BYTE [r8]  ; Load next byte of haystack into r10d
    movzx r11d, BYTE [r9]  ; Load next byte of needle into r11d
    jmp .run_inner_loop    ; Continue inner loop

.back_to_outter:
    inc rdi                ; Move to next character in haystack
    movzx ebx, BYTE [rdi]  ; Load next byte of haystack
    jmp .outter_loop       ; Continue outer loop

.outter_null_found:
    cmp al, 0x00           ; Check if needle is empty (null terminator)
    je .return_found       ; If needle is empty, needle is found
    jmp .return_0          ; Otherwise, return NULL

.inner_null_found:
    cmp r11b, 0x00         ; Check if needle is empty (null terminator)
    je .return_found       ; If needle is empty, needle is found
    jmp .return_0          ; Otherwise, return NULL

.return_0:
    xor rax, rax           ; Set rax to 0 (return NULL)
    jmp .exit              ; Jump to exit

.return_found:
    xor rax, rax           ; Set rax to 0
    mov rax, rdi           ; Move haystack pointer (rdi) to rax (return pointer to found needle)
    jmp .exit              ; Jump to exit

.exit:
    pop rbp                ; Epilogue: Restore base pointer
    ret                    ; Return from function
