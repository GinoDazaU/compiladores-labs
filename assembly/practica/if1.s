# x = 5
# y = 10
# if x < y:
#     print(x+3)
# else:
#     print(y+2)

.data 
print_fmt: .string "%ld\n"

.text
.globl main

main:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp

    # x = 5
    movq $5, %rax
    movq %rax, -8(%rbp)

    # y = 10
    movq $2, %rax
    movq %rax, -16(%rbp)

    # x < y
    movq -8(%rbp), %rax
    pushq %rax
    movq -16(%rbp), %rax
    movq %rax, %rcx
    popq %rax
    cmpq %rcx, %rax
    movq $0, %rax
    setl %al
    movzbq %al, %rax # resultado en rax

    # if x < y
    cmpq $0, %rax
    je label_else

label_if:
    movq -8(%rbp), %rax
    pushq %rax
    movq $3, %rax
    movq %rax, %rcx
    popq %rax
    addq %rcx, %rax

    movq %rax, %rsi
    movq $0, %rax

    leaq print_fmt(%rip), %rdi
    call printf@PLT

    jmp label_endif
    

label_else:
    movq -16(%rbp), %rax
    pushq %rax
    movq $2, %rax
    movq %rax, %rcx
    popq %rax
    addq %rcx, %rax

    movq %rax, %rsi
    movq $0, %rax

    leaq print_fmt(%rip), %rdi
    call printf@PLT

    jmp label_endif

label_endif:
    movq $0, %rax
    leave
    ret

.section .note.GNU-stack,"",@progbits
