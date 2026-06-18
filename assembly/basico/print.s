# print(4)
# print(5)

.data
print_fmt: .string "%ld\n"

.text
.globl main

main:
    pushq %rbp
    movq %rsp, %rbp

    movq $4, %rax
    movq %rax, %rsi
    movq $0, %rax

    leaq print_fmt(%rip), %rdi
    call printf@PLT

    movq $5, %rax
    movq %rax, %rsi
    movq $0, %rax

    leaq print_fmt(%rip), %rdi
    call printf@PLT

    leave 
    ret

.section .note.GNU-stack,"",@progbits
