.data
print_fmt: .string "%ld\n"

.text
.globl main

label_fun_suma:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, (%rsp)

    movq %rdi, %rax
    pusq %rax
    movq %rsi, %rax
    movq %rax, %rcx
    popq %rax

    addq %rcx, %rax

    jmp label_fun_suma_end
    




main:
    pushq %rbp
    movq %rsp, %rbp

    
