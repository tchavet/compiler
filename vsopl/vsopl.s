	.text
	.file	"vsopl/vsopl.ll"
	.globl	method.IO.print
	.align	16, 0x90
	.type	method.IO.print,@function
method.IO.print:                        # @method.IO.print
	.cfi_startproc
# BB#0:
	pushl	%esi
.Ltmp0:
	.cfi_def_cfa_offset 8
	subl	$8, %esp
.Ltmp1:
	.cfi_def_cfa_offset 16
.Ltmp2:
	.cfi_offset %esi, -8
	movl	%ecx, %esi
	movl	%edx, (%esp)
	calll	puts
	movl	%esi, %eax
	addl	$8, %esp
	popl	%esi
	retl
.Ltmp3:
	.size	method.IO.print, .Ltmp3-method.IO.print
	.cfi_endproc

	.globl	method.IO.printInt32
	.align	16, 0x90
	.type	method.IO.printInt32,@function
method.IO.printInt32:                   # @method.IO.printInt32
	.cfi_startproc
# BB#0:
	movl	%ecx, %eax
	retl
.Ltmp4:
	.size	method.IO.printInt32, .Ltmp4-method.IO.printInt32
	.cfi_endproc

	.globl	method.IO.printBool
	.align	16, 0x90
	.type	method.IO.printBool,@function
method.IO.printBool:                    # @method.IO.printBool
	.cfi_startproc
# BB#0:
	subl	$12, %esp
.Ltmp5:
	.cfi_def_cfa_offset 16
	testb	$1, %dl
	je	.LBB2_2
# BB#1:                                 # %if_true
	movl	$true.str, %edx
	jmp	.LBB2_3
.LBB2_2:                                # %if_false
	movl	$false.str, %edx
.LBB2_3:                                # %if_end
	calll	method.IO.print
	addl	$12, %esp
	retl
.Ltmp6:
	.size	method.IO.printBool, .Ltmp6-method.IO.printBool
	.cfi_endproc

	.globl	method.IO.inputInt32
	.align	16, 0x90
	.type	method.IO.inputInt32,@function
method.IO.inputInt32:                   # @method.IO.inputInt32
	.cfi_startproc
# BB#0:
	subl	$12, %esp
.Ltmp7:
	.cfi_def_cfa_offset 16
	leal	8(%esp), %eax
	movl	%eax, 4(%esp)
	movl	$.L.iostr, (%esp)
	calll	__isoc99_scanf
	addl	$12, %esp
	retl
.Ltmp8:
	.size	method.IO.inputInt32, .Ltmp8-method.IO.inputInt32
	.cfi_endproc

	.globl	method.IO.inputBool
	.align	16, 0x90
	.type	method.IO.inputBool,@function
method.IO.inputBool:                    # @method.IO.inputBool
	.cfi_startproc
# BB#0:
	subl	$12, %esp
.Ltmp9:
	.cfi_def_cfa_offset 16
	movl	8(%esp), %eax
	movl	%eax, 4(%esp)
	movl	$.L.iostr2, (%esp)
	calll	__isoc99_scanf
	movl	8(%esp), %eax
	movl	%eax, (%esp)
	movl	$true.str, 4(%esp)
	calll	strcmp
	testl	%eax, %eax
	je	.LBB4_2
# BB#1:                                 # %io.if_true
	movb	$1, %al
	addl	$12, %esp
	retl
.LBB4_2:                                # %io.if_false
	xorl	%eax, %eax
	addl	$12, %esp
	retl
.Ltmp10:
	.size	method.IO.inputBool, .Ltmp10-method.IO.inputBool
	.cfi_endproc

	.globl	method.IO.inputLine
	.align	16, 0x90
	.type	method.IO.inputLine,@function
method.IO.inputLine:                    # @method.IO.inputLine
	.cfi_startproc
# BB#0:
	pushl	%esi
.Ltmp11:
	.cfi_def_cfa_offset 8
	subl	$24, %esp
.Ltmp12:
	.cfi_def_cfa_offset 32
.Ltmp13:
	.cfi_offset %esi, -8
	leal	16(%esp), %esi
	movl	%esi, 4(%esp)
	movl	$.L.iostr2, (%esp)
	calll	__isoc99_scanf
	movl	%esi, %eax
	addl	$24, %esp
	popl	%esi
	retl
.Ltmp14:
	.size	method.IO.inputLine, .Ltmp14-method.IO.inputLine
	.cfi_endproc

	.type	true.str,@object        # @true.str
	.section	.rodata,"a",@progbits
	.globl	true.str
true.str:
	.asciz	"true\n"
	.size	true.str, 6

	.type	false.str,@object       # @false.str
	.globl	false.str
false.str:
	.asciz	"false\n"
	.size	false.str, 7

	.type	.L.iostr,@object        # @.iostr
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.iostr:
	.asciz	"%d"
	.size	.L.iostr, 3

	.type	.L.iostr2,@object       # @.iostr2
.L.iostr2:
	.asciz	"%s"
	.size	.L.iostr2, 3


	.section	".note.GNU-stack","",@progbits
