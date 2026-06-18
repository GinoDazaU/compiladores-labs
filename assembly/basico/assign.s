# x = 5;
# y = 10;
# z = 20;
# print(x+y);
# print(x-z);

.data
print_fmt: .string "%ld\n"

.text
.globl main

main:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp

    movq $5, %rax
    movq %rax, -8(%rbp)

    movq $10, %rax
    movq %rax, -16(%rbp)

    movq $20, %rax
    movq %rax, -24(%rbp)
    
    movq -8(%rbp), %rax
    pushq %rax
    movq -16(%rbp), %rax
    movq %rax, %rcx
    popq %rax
    addq %rcx, %rax

    movq %rax, %rsi
    movq $0, %rax

    leaq print_fmt(%rip), %rdi
    call printf@PLT

    movq -8(%rbp), %rax
    pushq %rax

    movq -24(%rbp), %rax
    movq %rax, %rcx

    popq %rax

    subq %rcx, %rax

    movq %rax, %rsi
    movq $0, %rax

    leaq print_fmt(%rip), %rdi
    call printf@PLT

    movq $0, %rax
    leave
    ret

.section .note.GNU-stack,"",@progbits
