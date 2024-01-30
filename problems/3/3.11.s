	.file	"3.11.c"
	.text
	.globl	xor
	.type	xor, @function
xor:
.LFB0:
	.cfi_startproc
	endbr64
	xorq	%rsi, %rdi
	xorl	%ecx, %edx
	movslq	%edx, %rdx
	leaq	(%rdi,%rdx), %rax
	ret
	.cfi_endproc
.LFE0:
	.size	xor, .-xor
	.globl	assign1
	.type	assign1, @function
assign1:
.LFB1:
	.cfi_startproc
	endbr64
	movl	$0, %eax
	ret
	.cfi_endproc
.LFE1:
	.size	assign1, .-assign1
	.globl	assign2
	.type	assign2, @function
assign2:
.LFB2:
	.cfi_startproc
	endbr64
	movq	$-15790321, %rax
	ret
	.cfi_endproc
.LFE2:
	.size	assign2, .-assign2
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
