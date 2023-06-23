section .text

_start:
        mov rax, 1
        mov rdi, 1
        lea rsi, [rel woody]
        mov rdx, woody_len
        syscall
        mov rdx, [rel key]
        mov rax, [rel text]
        mov rcx, [rel tsize]
        add rcx, rax
        jmp decrypt

decrypt:
        cmp rdi, 0
        je reset_key
        xor BYTE [rax], dl
        ror rdx, 8
        inc rax
        cmp rax, rcx
        jnz decrypt
        mov rax, [rel text]
        jmp rax

reset_key:
        mov rdi, [rel key]
        jmp decrypt

datas:
key:             dq 0xCCCCCCCCCCCCCCCC
text:            dq 0xAAAAAAAAAAAAAAAA
tsize:           dq 0xBBBBBBBBBBBBBBBB
woody:           db '....WOODY....',0x0a,0
woody_len:       equ $ - woody

end: