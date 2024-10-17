section .text

start:
        push rax
        push rdx
        push rcx
        push rdi
        push rsi

init:
        lea rdi, [rel key]
        lea rdx, [rel 0xAAAAAAAA] ; patch later to text section address
        mov rax, 0xBBBBBBBBBBBBBBBB ; patch later to text section end address

reset:
        xor r8, r8

decrypt:
        mov al, BYTE [rdi + r8]
        xor [rdx], al
        call print_woody
        inc r8
        inc rdx
        cmp rdx, rax
        je entry
        cmp r8, 8
        je reset
        jmp decrypt

entry:
        pop rsi    
        pop rdi
        pop rcx
        pop rdx
        pop rax
        jmp 0xDDDDDDDD ; patch later to base entrypoint

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

key:            db 'CCCCCCCC',0 ; patch later to key value
key_size:       equ $ - key