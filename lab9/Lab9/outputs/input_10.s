.data
print_fmt: .string "%ld \n"

.text

.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $24, %rsp
 movq $1, %rax
  movq %rax, -8(%rbp)
 movq $3, %rax
  movq %rax, -16(%rbp)
 movq $5, %rax
  movq %rax, -24(%rbp)
dowhile_0:
 movq -8(%rbp), %rax
 pushq %rax
 movq -16(%rbp), %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movq $0, %rax
 setle %al
 movzbq %al, %rax
 pushq %rax
 movq -24(%rbp), %rax
 pushq %rax
 movq -16(%rbp), %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movq $0, %rax
 setg %al
 movzbq %al, %rax
 movq %rax, %rcx
 popq %rax
 andq %rcx, %rax
 cmpq $0, %rax
 je else_1
 movq -8(%rbp), %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movq $0, %rax
 call printf@PLT
 jmp endif_1
else_1:
endif_1:
 movq -8(%rbp), %rax
 movq %rax, %r10
 movq $1, %rax
 cmpq %rax, %r10
 je case_2_1
 movq $2, %rax
 cmpq %rax, %r10
 je case_2_2
 movq $3, %rax
 cmpq %rax, %r10
 je case_2_3
 jmp default_2
case_2_1:
 movq $100, %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movq $0, %rax
 call printf@PLT
 jmp endswitch_2
 jmp endswitch_2
case_2_2:
 movq $200, %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movq $0, %rax
 call printf@PLT
 jmp endswitch_2
 jmp endswitch_2
case_2_3:
 movq $300, %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movq $0, %rax
 call printf@PLT
 jmp endswitch_2
 jmp endswitch_2
default_2:
 movq $999, %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movq $0, %rax
 call printf@PLT
endswitch_2:
 movq -8(%rbp), %rax
 pushq %rax
 movq $1, %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
  movq %rax, -8(%rbp)
 movq -8(%rbp), %rax
 pushq %rax
 movq $4, %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movq $0, %rax
 setle %al
 movzbq %al, %rax
 pushq %rax
 movq -8(%rbp), %rax
 pushq %rax
 movq $5, %rax
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
 jne dowhile_0
endwhile_0:
 movq $0, %rax
 jmp .end_main
.end_main:
 leave
 ret

.section .note.GNU-stack,"",@progbits
