section .text

init:
        call rip

rip:
        pop rdx
        sub rdx, rip - init
        sub rdx, [rel addr]
        mov rsi, rdx
        mov rdx, [rel size]
        lea rdi, [rel key]
        xor rcx, rcx

reset:
        xor r8, r8

decrypt:
        mov al, BYTE [rdi + r8]
        xor [rsi + rcx], al
        inc r8
        inc rcx
        cmp rcx, rdx
        je entry
        cmp r8, 8
        je reset
        jmp decrypt

entry:
        call print_woody
        jmp rsi

print_woody:
        push rsi
        mov rax, 1
        mov rdi, 1
        lea rsi, [rel woody]
        mov rdx, woody_len
        syscall
        pop rsi
        ret

woody:           db '....WOODY....',0x0a,0
woody_len:       equ $ - woody

addr:            dq 0xA
size:            dq 0xB

key:             db 'CCCCCCCC',0
key_size:        equ $ - key