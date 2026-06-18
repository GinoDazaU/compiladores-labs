# i = 0
# n = 4
# while i < n:
#     print(i)
#     i = i + 1
# print(100)
.data
print_fmt: .string "%ld\n"

.text
.globl main

main:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp

    # i = 0
    movq $0, %rax
    movq %rax, -8(%rbp)

    # n = 4
    movq $4, %rax
    movq %rax, -16(%rbp)

label_while_cond:
    # i < n -> %rax
    movq -8(%rbp), %rax
    pushq %rax
    movq -16(%rbp), %rax
    movq %rax, %rcx
    popq %rax
    cmpq %rcx, %rax
    movq $0, %rax
    setl %al
    movzbq %al, %rax

    # while i < n:
    cmpq $0, %rax
    je label_while_end

label_while_body:
    movq -8(%rbp), %rax
    movq %rax, %rsi
    movq $0, %rax
    leaq print_fmt(%rip), %rdi
    call printf@PLT
    
    movq -8(%rbp), %rax
    pushq %rax
    movq $1, %rax
    movq %rax, %rcx
    popq %rax
    addq %rcx, %rax
    movq %rax, -8(%rbp)

    jmp label_while_cond

label_while_end:
    movq $100, %rax
    movq %rax, %rsi
    movq $0, %rax
    leaq print_fmt(%rip), %rdi
    call printf@PLT

    movq $0, %rax
    leave
    ret

.section .note.GNU-stack,"",@progbits
