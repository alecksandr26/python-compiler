	.text
	.file	"PythonCompiler"
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movl	$2, 4(%rsp)
	movl	$8, (%rsp)
	movl	$10, 20(%rsp)
	movl	$10, %edi
	callq	print@PLT
	movl	4(%rsp), %edi
	imull	(%rsp), %edi
	movl	%edi, 16(%rsp)
	callq	print@PLT
	movl	(%rsp), %eax
	cltd
	idivl	4(%rsp)
	movl	%eax, 12(%rsp)
	movl	%eax, %edi
	callq	print@PLT
	movl	(%rsp), %edi
	subl	4(%rsp), %edi
	movl	%edi, 8(%rsp)
	callq	print@PLT
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
