section .text

global asm_strstr

asm_strstr:
    push rbp
    mov rbp, rsp
    push rbx
    push rdi
    push rsi
    push rdx

    ; Load parameters into registers
    mov rdi, rdi ; rdi = haystack (string to search)
    mov rsi, rsi ; rsi = needle (substring to find)

    ; If haystack or needle is NULL, return NULL
    test rdi, rdi
    jz .return_null
    test rsi, rsi
    jz .return_null

    ; Get lengths of haystack and needle
    mov rcx, rdi ; rcx = haystack (use rcx to iterate through haystack)
    mov rdx, rsi ; rdx = needle (use rdx to iterate through needle)
    call strlen   ;get length of haystack
