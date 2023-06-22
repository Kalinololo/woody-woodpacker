section .text
        global _start

_start:
        mov rax, 1
        mov rdi, 1
        lea rsi, [rel woody]
        mov rdx, woody_len
        syscall
        xor rcx, rcx
        mov rdi, [rel key]
        mov rax, [rel text]
        mov rsi, [rel tsize]
        jmp decrypt

decrypt:
        cmp rdi, 0
        je reset_key
        mov al, BYTE [rdi]
        xor BYTE [rax + rcx], al
        inc rcx
        inc rdi
        cmp rcx, rsi
        jb decrypt
        mov rax, [rel text]
        jmp rax

reset_key:
        mov rdi, [rel key]
        jmp decrypt

end:


        woody           db '....WOODY....',0x0a,0
	woody_len       equ $ - woody
        key             dq 0x9999999999999999
        text            dq 0xAAAAAAAAAAAAAAAA
        tsize           dq 0xBBBBBBBBBBBBBBBB