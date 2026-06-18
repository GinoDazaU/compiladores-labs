.data
print_fmt: .string "%ld \n"

.text

.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $24, %rsp
 movq $5, %rax
  movq %rax, -8(%rbp)
 movq $10, %rax
  movq %rax, -16(%rbp)
 movq $15, %rax
  movq %rax, -24(%rbp)
 movq -8(%rbp), %rax
 pushq %rax
 movq -16(%rbp), %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movq $0, %rax
 setl %al
 movzbq %al, %rax
 pushq %rax
 movq -16(%rbp), %rax
 pushq %rax
 movq -24(%rbp), %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movq $0, %rax
 setl %al
 movzbq %al, %rax
 movq %rax, %rcx
 popq %rax
 andq %rcx, %rax
 pushq %rax
 movq -24(%rbp), %rax
 pushq %rax
 movq $0, %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movq $0, %rax
 sete %al
 movzbq %al, %rax
 movq %rax, %rcx
 popq %rax
 orq %rcx, %rax
 cmpq $0, %rax
 je else_0
 movq $1, %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movq $0, %rax
 call printf@PLT
 jmp endif_0
else_0:
endif_0:
 movq $0, %rax
 jmp .end_main
.end_main:
 leave
 ret

.section .note.GNU-stack,"",@progbits
