section .text

start:
        push rax
        push rdx
        push rcx
        push rdi
        push rsi

print_woody:
        mov rax, 1
        mov rdi, 1
        lea rsi, [rel woody]
        mov rdx, woody_len
        syscall

init:        
        mov rdx, 0xCCCCCCCCCCCCCCCC
        mov rax, 0xAAAAAAAAAAAAAAAA
        mov rcx, 0xBBBBBBBBBBBBBBBB
        add rcx, rax

decrypt:
        xor BYTE [rax], dl
        ror rdx, 1
        inc rax
        cmp rax, rcx
        jnz decrypt

entry:
        pop rsi    
        pop rdi
        pop rcx
        pop rdx
        pop rax
        jmp 0xDDDDDDDD

woody:           db '....WOODY....',0x0a,0
woody_len:       equ $ - woody

end: