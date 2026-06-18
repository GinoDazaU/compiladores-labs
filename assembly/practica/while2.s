# x = 0
# i = 1
# n = 10
# while(i < n)
#     x = x + i
#     print(i)
# print(x)

.data
print_fmt: .string "%ld\n"

.text
.globl main

main:
    pushq %rbp
    movq %rsp, %rbp
    subq $24, %rsp

    movq $0, %rax
    movq %rax, -8(%rbp)

    movq $1, %rax
    movq %rax, -16(%rbp)

    movq $10, %rax
    movq %rax, -24(%rbp)

label_while_cond:
    movq -16(%rbp), %rax
    pushq %rax
    movq -24(%rbp), %rax
    movq %rax, %rcx
    popq %rax
    
    cmpq %rcx, %rax
    movq $0, %rax
    setl %al
    movzbq %al, %rax

    cmpq $0, %rax
    jmpe label_while_end

label_while_body:
    
