section .text

start:
        push rax
        push rdx
        push rcx
        push rdi
        push rsi

init:
        call print_woody
        lea rdx, [rel key]
        mov rax, 0xAAAAAAAAAAAAAAAA
        mov rcx, 0xBBBBBBBBBBBBBBBB
        add rcx, rax

reset:
        xor r8, r8

decrypt:
        mov al, BYTE [rax]
        call print_woody
        mov al, BYTE [rdx + r8]
        xor BYTE [rax], al
        inc r8
        inc rax
        cmp rax, rcx
        je entry
        cmp r8, key_size
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
        push rax
        push rdi
        push rsi
        push rdx
        mov rax, 1
        mov rdi, 1
        lea rsi, [rel woody]
        mov rdx, woody_len
        syscall
        pop rdx
        pop rsi
        pop rdi
        pop rax
        ret

woody:           db '....WOODY....',0x0a,0
woody_len:       equ $ - woody

key:            db 'CCCCCCCC',0
key_size:       equ $ - key