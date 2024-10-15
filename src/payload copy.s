section .text

start:
        push rax
        push rdx
        push rcx
        push rdi
        push rsi

init:
        call print_woody
        mov rdx, 0xCCCCCCCCCCCCCCCC
        mov rax, 0xAAAAAAAAAAAAAAAA
        mov rcx, 0xBBBBBBBBBBBBBBBB
        add rcx, rax

reset:
        mov rsi, 0

decrypt:
        mov bl, [rdx + rsi]
        xor BYTE [rax], bl
        inc rsi
        inc rax
        cmp rax, rcx
        je entry
        cmp rsi, 8
        je reset
        jmp decrypt

entry:
        call print_woody
        pop rsi    
        pop rdi
        pop rcx
        pop rdx
        pop rax
        jmp 0xDDDDDDDD

print_woody:
        mov rax, 1
        mov rdi, 1
        lea rsi, [rel woody]
        mov rdx, woody_len
        syscall
        ret

woody:           db '....WOODY....',0x0a,0
woody_len:       equ $ - woody

end: