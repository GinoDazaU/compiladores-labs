.data
print_fmt: .string "%ld \n"

.text

.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $8, %rsp
 movq $2, %rax
  movq %rax, -8(%rbp)
 movq -8(%rbp), %rax
 movq %rax, %r10
 movq $1, %rax
 cmpq %rax, %r10
 je case_0_1
 movq $2, %rax
 cmpq %rax, %r10
 je case_0_2
 jmp default_0
case_0_1:
 movq $100, %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movq $0, %rax
 call printf@PLT
 jmp endswitch_0
 jmp endswitch_0
case_0_2:
 movq $200, %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movq $0, %rax
 call printf@PLT
 jmp endswitch_0
 jmp endswitch_0
default_0:
 movq $999, %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movq $0, %rax
 call printf@PLT
endswitch_0:
 movq $0, %rax
 jmp .end_main
.end_main:
 leave
 ret

.section .note.GNU-stack,"",@progbits
