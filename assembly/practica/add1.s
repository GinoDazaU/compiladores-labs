# print(5 + 6)

.data
print_fmt: .string "%ld\n"

.text
.globl main

main:
    pushq %rbp
    movq %rsp, %rbp

    movq $5, %rax
    pushq %rax

    movq $6, %rax
    movq %rax, %rcx

    addq %rcx, %rax

    movq %rax, %rsi
    movq $0, %rax

    leaq print_fmt(%rip), %rdi
    call printf@PLT

    movq $0, %rax
    leave
    ret

.section .note.GNU-stack,"",@progbits

