	.file	"lex.c"
	.text
	.p2align 4
	.type	lex_current.constprop.0, @function
lex_current.constprop.0:
.LFB89:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$56, %rsp
	.cfi_def_cfa_offset 112
	movzbl	72(%rdi), %eax
	movb	%al, 15(%rsp)
	testb	%al, %al
	jne	.L7
	movq	16(%rdi), %r13
	movq	24(%rdi), %r12
	movq	%rdi, %r15
	movq	32(%rdi), %r14
.L8:
	movdqu	16(%r15), %xmm1
	movq	32(%r15), %rax
	cmpb	$0, 0(%r13,%r12)
	movq	%rax, 32(%rsp)
	movaps	%xmm1, 16(%rsp)
	je	.L1
	cmpq	$0, 8(%r15)
	je	.L1
	xorl	%ebx, %ebx
	jmp	.L9
	.p2align 4,,10
	.p2align 3
.L6:
	addq	$1, %rbx
	cmpq	8(%r15), %rbx
	jnb	.L1
.L9:
	movq	%r13, 16(%rsp)
	subq	$32, %rsp
	.cfi_def_cfa_offset 144
	movq	(%r15), %rax
	leaq	(%rbx,%rbx,2), %rbp
	movq	%r12, 56(%rsp)
	salq	$3, %rbp
	movq	%r14, 64(%rsp)
	movdqa	48(%rsp), %xmm0
	movq	8(%rax,%rbp), %rax
	movq	%r14, 16(%rsp)
	movups	%xmm0, (%rsp)
	call	*%rax
	addq	$32, %rsp
	.cfi_def_cfa_offset 112
	testq	%rax, %rax
	je	.L6
	cmpb	$0, 73(%r15)
	movq	%r13, 16(%r15)
	movq	%r12, 24(%r15)
	movq	%rax, 32(%r15)
	movq	%r13, 40(%r15)
	movq	%r12, 48(%r15)
	movq	%rax, 56(%r15)
	movl	%ebx, 64(%r15)
	movb	$1, 72(%r15)
	jne	.L7
	movq	(%r15), %rdx
	cmpb	$0, 16(%rdx,%rbp)
	je	.L7
	addq	%rax, %r12
	movq	$0, 32(%r15)
	xorl	%r14d, %r14d
	movq	%r12, 24(%r15)
	movb	$0, 72(%r15)
	jmp	.L8
.L7:
	movb	$1, 15(%rsp)
.L1:
	movzbl	15(%rsp), %eax
	addq	$56, %rsp
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE89:
	.size	lex_current.constprop.0, .-lex_current.constprop.0
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC0:
	.string	"Lex initialization failed due to an missing token definition. TokenId '%d' is unset"
	.text
	.p2align 4
	.globl	lex_init
	.type	lex_init, @function
lex_init:
.LFB55:
	.cfi_startproc
	endbr64
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	pxor	%xmm0, %xmm0
	movq	%rdi, %r10
	movq	%rsi, %r11
	subq	$560, %rsp
	.cfi_def_cfa_offset 576
	movq	%fs:40, %rax
	movq	%rax, 552(%rsp)
	xorl	%eax, %eax
	movaps	%xmm0, (%rsp)
	movq	$0, 16(%rsp)
	testq	%rdx, %rdx
	je	.L14
	movq	%rsi, %rdi
	xorl	%r8d, %r8d
	movq	%rsp, %r9
	jmp	.L18
	.p2align 4,,10
	.p2align 3
.L22:
	addq	$1, %r8
	addq	$24, %rdi
	cmpq	%rdx, %r8
	je	.L14
.L18:
	movq	(%rdi), %rax
	movq	8(%rdi), %rsi
	xorq	(%r9), %rax
	xorq	8(%r9), %rsi
	orq	%rsi, %rax
	jne	.L22
	movq	16(%r9), %rax
	cmpq	%rax, 16(%rdi)
	jne	.L22
	leaq	32(%rsp), %rbx
	leaq	.LC0(%rip), %rcx
	movl	$512, %edx
	xorl	%eax, %eax
	movl	$2, %esi
	movq	%rbx, %rdi
	call	__sprintf_chk@PLT
	call	__errno_location@PLT
	movq	%rbx, %rdi
	movl	$22, (%rax)
	call	perror@PLT
	movl	$1, %edi
	call	exit@PLT
	.p2align 4,,10
	.p2align 3
.L14:
	movq	$0, 72(%r10)
	pxor	%xmm0, %xmm0
	movq	%r11, (%r10)
	movq	%rdx, 8(%r10)
	movq	%rcx, 16(%r10)
	movups	%xmm0, 24(%r10)
	movups	%xmm0, 40(%r10)
	movups	%xmm0, 56(%r10)
	movq	552(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L26
	addq	$560, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	movq	%r10, %rax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
.L26:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE55:
	.size	lex_init, .-lex_init
	.p2align 4
	.globl	lex_current
	.type	lex_current, @function
lex_current:
.LFB56:
	.cfi_startproc
	endbr64
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$56, %rsp
	.cfi_def_cfa_offset 112
	movzbl	72(%rdi), %eax
	movq	%rsi, (%rsp)
	movb	%al, 15(%rsp)
	testb	%al, %al
	jne	.L28
	movq	16(%rdi), %r13
	movq	24(%rdi), %r12
	movq	%rdi, %r15
	movq	32(%rdi), %r14
.L29:
	movdqu	16(%r15), %xmm1
	movq	32(%r15), %rax
	cmpb	$0, 0(%r13,%r12)
	movq	%rax, 32(%rsp)
	movaps	%xmm1, 16(%rsp)
	je	.L32
	cmpq	$0, 8(%r15)
	je	.L39
	xorl	%ebx, %ebx
	jmp	.L33
	.p2align 4,,10
	.p2align 3
.L38:
	addq	$1, %rbx
	cmpq	8(%r15), %rbx
	jnb	.L39
.L33:
	movq	%r13, 16(%rsp)
	subq	$32, %rsp
	.cfi_def_cfa_offset 144
	movq	(%r15), %rax
	leaq	(%rbx,%rbx,2), %rbp
	movq	%r12, 56(%rsp)
	salq	$3, %rbp
	movq	%r14, 64(%rsp)
	movdqa	48(%rsp), %xmm0
	movq	8(%rax,%rbp), %rax
	movq	%r14, 16(%rsp)
	movups	%xmm0, (%rsp)
	call	*%rax
	addq	$32, %rsp
	.cfi_def_cfa_offset 112
	testq	%rax, %rax
	je	.L38
	cmpb	$0, 73(%r15)
	movq	%r13, 16(%r15)
	movq	%r12, 24(%r15)
	movq	%rax, 32(%r15)
	movq	%r13, 40(%r15)
	movq	%r12, 48(%r15)
	movq	%rax, 56(%r15)
	movl	%ebx, 64(%r15)
	movb	$1, 72(%r15)
	jne	.L28
	movq	(%r15), %rcx
	cmpb	$0, 16(%rcx,%rbp)
	je	.L28
	addq	%rax, %r12
	movq	$0, 32(%r15)
	xorl	%r14d, %r14d
	movq	%r12, 24(%r15)
	movb	$0, 72(%r15)
	jmp	.L29
	.p2align 4,,10
	.p2align 3
.L39:
	cmpq	$0, (%rsp)
	je	.L27
	movq	(%rsp), %rax
	movl	$-1, (%rax)
.L27:
	movzbl	15(%rsp), %eax
	addq	$56, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
.L28:
	.cfi_restore_state
	movq	(%rsp), %rax
	testq	%rax, %rax
	je	.L30
	movl	$1, (%rax)
.L30:
	movb	$1, 15(%rsp)
	jmp	.L27
.L32:
	movq	(%rsp), %rax
	testq	%rax, %rax
	je	.L27
	movl	$0, (%rax)
	jmp	.L27
	.cfi_endproc
.LFE56:
	.size	lex_current, .-lex_current
	.p2align 4
	.globl	lex_consume
	.type	lex_consume, @function
lex_consume:
.LFB57:
	.cfi_startproc
	endbr64
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	%rsi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movl	%edx, %ebp
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	%rdi, %rbx
	call	lex_current.constprop.0
	testb	%al, %al
	je	.L46
	cmpl	%ebp, 64(%rbx)
	je	.L58
	xorl	%eax, %eax
.L46:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L58:
	.cfi_restore_state
	testq	%r12, %r12
	je	.L48
	movdqu	40(%rbx), %xmm0
	movups	%xmm0, (%r12)
	movdqu	56(%rbx), %xmm1
	movups	%xmm1, 16(%r12)
.L48:
	cmpb	$0, 72(%rbx)
	je	.L46
	movq	32(%rbx), %rdx
	movb	$0, 72(%rbx)
	addq	%rdx, 24(%rbx)
	movq	$0, 32(%rbx)
	jmp	.L46
	.cfi_endproc
.LFE57:
	.size	lex_consume, .-lex_consume
	.p2align 4
	.globl	lex_skipn
	.type	lex_skipn, @function
lex_skipn:
.LFB58:
	.cfi_startproc
	endbr64
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	movq	%rdx, %r14
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	movl	%esi, %r13d
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	movq	%rcx, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movq	%rdi, %rbx
	subq	$96, %rsp
	.cfi_def_cfa_offset 144
	movdqu	(%rdi), %xmm0
	movdqu	16(%rdi), %xmm1
	movq	%fs:40, %rax
	movq	%rax, 88(%rsp)
	xorl	%eax, %eax
	movdqu	32(%rdi), %xmm2
	movdqu	48(%rdi), %xmm3
	movdqu	64(%rdi), %xmm4
	movq	%rsp, %rdi
	movaps	%xmm0, (%rsp)
	movaps	%xmm1, 16(%rsp)
	movaps	%xmm2, 32(%rsp)
	movaps	%xmm3, 48(%rsp)
	movaps	%xmm4, 64(%rsp)
	call	lex_current.constprop.0
	testb	%al, %al
	je	.L62
	cmpl	64(%rsp), %r13d
	je	.L66
.L62:
	xorl	%ebp, %ebp
.L59:
	movq	88(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L67
	addq	$96, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 48
	movl	%ebp, %eax
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L66:
	.cfi_restore_state
	cmpb	$0, 72(%rsp)
	movl	%eax, %ebp
	movq	40(%rsp), %rdi
	movq	48(%rsp), %rax
	je	.L63
	movq	32(%rsp), %rdx
	addq	%rdx, 24(%rsp)
	movq	$0, 32(%rsp)
	movb	$0, 72(%rsp)
.L63:
	cmpq	%r12, 56(%rsp)
	jne	.L62
	addq	%rax, %rdi
	movq	%r12, %rdx
	movq	%r14, %rsi
	call	strncmp@PLT
	testl	%eax, %eax
	jne	.L62
	movdqa	(%rsp), %xmm5
	movdqa	16(%rsp), %xmm6
	movdqa	32(%rsp), %xmm7
	movups	%xmm5, (%rbx)
	movdqa	48(%rsp), %xmm5
	movups	%xmm6, 16(%rbx)
	movdqa	64(%rsp), %xmm6
	movups	%xmm7, 32(%rbx)
	movups	%xmm5, 48(%rbx)
	movups	%xmm6, 64(%rbx)
	jmp	.L59
.L67:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE58:
	.size	lex_skipn, .-lex_skipn
	.p2align 4
	.globl	lex_skip
	.type	lex_skip, @function
lex_skip:
.LFB59:
	.cfi_startproc
	endbr64
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	movl	%esi, %r14d
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	movq	%rdx, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movq	%rdi, %rbx
	movq	%rdx, %rdi
	subq	$96, %rsp
	.cfi_def_cfa_offset 144
	movq	%fs:40, %rax
	movq	%rax, 88(%rsp)
	xorl	%eax, %eax
	call	strlen@PLT
	movdqu	(%rbx), %xmm0
	movdqu	16(%rbx), %xmm1
	movq	%rsp, %rdi
	movdqu	32(%rbx), %xmm2
	movdqu	48(%rbx), %xmm3
	movq	%rax, %r13
	movdqu	64(%rbx), %xmm4
	movaps	%xmm0, (%rsp)
	movaps	%xmm1, 16(%rsp)
	movaps	%xmm2, 32(%rsp)
	movaps	%xmm3, 48(%rsp)
	movaps	%xmm4, 64(%rsp)
	call	lex_current.constprop.0
	testb	%al, %al
	je	.L71
	cmpl	64(%rsp), %r14d
	je	.L75
.L71:
	xorl	%ebp, %ebp
.L68:
	movq	88(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L76
	addq	$96, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 48
	movl	%ebp, %eax
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L75:
	.cfi_restore_state
	cmpb	$0, 72(%rsp)
	movl	%eax, %ebp
	movq	40(%rsp), %rdi
	movq	48(%rsp), %rax
	je	.L72
	movq	32(%rsp), %rdx
	addq	%rdx, 24(%rsp)
	movq	$0, 32(%rsp)
	movb	$0, 72(%rsp)
.L72:
	cmpq	56(%rsp), %r13
	jne	.L71
	addq	%rax, %rdi
	movq	%r13, %rdx
	movq	%r12, %rsi
	call	strncmp@PLT
	testl	%eax, %eax
	jne	.L71
	movdqa	(%rsp), %xmm5
	movdqa	16(%rsp), %xmm6
	movdqa	32(%rsp), %xmm7
	movups	%xmm5, (%rbx)
	movdqa	48(%rsp), %xmm5
	movups	%xmm6, 16(%rbx)
	movdqa	64(%rsp), %xmm6
	movups	%xmm7, 32(%rbx)
	movups	%xmm5, 48(%rbx)
	movups	%xmm6, 64(%rbx)
	jmp	.L68
.L76:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE59:
	.size	lex_skip, .-lex_skip
	.p2align 4
	.globl	lex_move
	.type	lex_move, @function
lex_move:
.LFB60:
	.cfi_startproc
	endbr64
	cmpb	$0, 72(%rdi)
	je	.L77
	movq	32(%rdi), %rax
	addq	%rax, 24(%rdi)
	movq	$0, 32(%rdi)
	movb	$0, 72(%rdi)
.L77:
	ret
	.cfi_endproc
.LFE60:
	.size	lex_move, .-lex_move
	.p2align 4
	.globl	lex_match_charsn
	.type	lex_match_charsn, @function
lex_match_charsn:
.LFB61:
	.cfi_startproc
	endbr64
	testl	%esi, %esi
	jle	.L82
	movq	16(%rsp), %rax
	movq	8(%rsp), %rdx
	movslq	%esi, %rsi
	movzbl	(%rdx,%rax), %edx
	leaq	(%rdi,%rsi), %rax
	jmp	.L81
	.p2align 4,,10
	.p2align 3
.L85:
	addq	$1, %rdi
	cmpq	%rax, %rdi
	je	.L82
.L81:
	cmpb	(%rdi), %dl
	jne	.L85
	movl	$1, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L82:
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE61:
	.size	lex_match_charsn, .-lex_match_charsn
	.p2align 4
	.globl	lex_match_chars
	.type	lex_match_chars, @function
lex_match_chars:
.LFB62:
	.cfi_startproc
	endbr64
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	call	strlen@PLT
	movq	16(%rsp), %rcx
	movq	24(%rsp), %rdx
	testl	%eax, %eax
	jle	.L89
	subl	$1, %eax
	movzbl	(%rcx,%rdx), %edx
	movq	%rbx, %rdi
	leaq	1(%rbx,%rax), %rax
	jmp	.L88
	.p2align 4,,10
	.p2align 3
.L93:
	addq	$1, %rdi
	cmpq	%rax, %rdi
	je	.L89
.L88:
	cmpb	(%rdi), %dl
	jne	.L93
	movl	$1, %eax
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L89:
	.cfi_restore_state
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE62:
	.size	lex_match_chars, .-lex_match_chars
	.p2align 4
	.globl	lex_match_keywordn
	.type	lex_match_keywordn, @function
lex_match_keywordn:
.LFB63:
	.cfi_startproc
	endbr64
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	%rsi, %rdx
	xorl	%r12d, %r12d
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	40(%rsp), %rbp
	movq	%rsi, %rbx
	movq	%rdi, %rsi
	addq	32(%rsp), %rbp
	movq	%rbp, %rdi
	call	strncmp@PLT
	testl	%eax, %eax
	jne	.L94
	call	__ctype_b_loc@PLT
	movsbq	0(%rbp,%rbx), %rbp
	movq	(%rax), %rax
	testb	$8, (%rax,%rbp,2)
	cmove	%rbx, %r12
.L94:
	movq	%r12, %rax
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE63:
	.size	lex_match_keywordn, .-lex_match_keywordn
	.p2align 4
	.globl	lex_match_keyword
	.type	lex_match_keyword, @function
lex_match_keyword:
.LFB64:
	.cfi_startproc
	endbr64
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	call	strlen@PLT
	movq	40(%rsp), %r12
	addq	32(%rsp), %r12
	movq	%rbp, %rsi
	movq	%rax, %rdx
	movq	%r12, %rdi
	movq	%rax, %rbx
	call	strncmp@PLT
	movl	%eax, %edx
	xorl	%eax, %eax
	testl	%edx, %edx
	jne	.L99
	call	__ctype_b_loc@PLT
	movsbq	(%r12,%rbx), %rbp
	movq	(%rax), %rax
	testb	$8, (%rax,%rbp,2)
	movl	$0, %eax
	cmove	%rbx, %rax
.L99:
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE64:
	.size	lex_match_keyword, .-lex_match_keyword
	.p2align 4
	.globl	lex_match_wrapped
	.type	lex_match_wrapped, @function
lex_match_wrapped:
.LFB65:
	.cfi_startproc
	endbr64
	movq	16(%rsp), %rdx
	addq	8(%rsp), %rdx
	movl	%edi, %r8d
	xorl	%eax, %eax
	cmpb	%dil, (%rdx)
	je	.L119
.L105:
	ret
	.p2align 4,,10
	.p2align 3
.L119:
	movzbl	1(%rdx), %ecx
	testb	%cl, %cl
	je	.L105
	movl	$1, %eax
	testb	%sil, %sil
	je	.L109
.L107:
	cmpb	%cl, %r8b
	je	.L120
.L111:
	addq	$1, %rax
	movzbl	(%rdx,%rax), %ecx
	testb	%cl, %cl
	jne	.L107
	xorl	%eax, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L120:
	cmpb	$92, -1(%rdx,%rax)
	je	.L111
.L108:
	addq	$1, %rax
	ret
	.p2align 4,,10
	.p2align 3
.L109:
	cmpb	%r8b, %cl
	je	.L108
	addq	$1, %rax
	movzbl	(%rdx,%rax), %ecx
	testb	%cl, %cl
	jne	.L109
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE65:
	.size	lex_match_wrapped, .-lex_match_wrapped
	.p2align 4
	.globl	lex_match_exactn
	.type	lex_match_exactn, @function
lex_match_exactn:
.LFB66:
	.cfi_startproc
	endbr64
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rax
	movq	%rsi, %rbx
	movq	24(%rsp), %rdi
	movq	%rsi, %rdx
	addq	16(%rsp), %rdi
	movq	%rax, %rsi
	call	strncmp@PLT
	testl	%eax, %eax
	movl	$0, %eax
	cmove	%rbx, %rax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE66:
	.size	lex_match_exactn, .-lex_match_exactn
	.p2align 4
	.globl	lex_match_region
	.type	lex_match_region, @function
lex_match_region:
.LFB68:
	.cfi_startproc
	endbr64
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	movq	%rdi, %r13
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movq	%rsi, %rbx
	subq	$24, %rsp
	.cfi_def_cfa_offset 80
	movl	%edx, 12(%rsp)
	movq	88(%rsp), %r15
	call	strlen@PLT
	movq	80(%rsp), %rbp
	movq	%r13, %rsi
	movq	%rax, %rdx
	movq	%rax, %r14
	leaq	0(%rbp,%r15), %rdi
	call	strncmp@PLT
	testl	%eax, %eax
	jne	.L131
	testq	%r14, %r14
	jne	.L149
.L131:
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L149:
	.cfi_restore_state
	addq	%r15, %r14
	leaq	0(%rbp,%r14), %r13
	cmpb	$0, 0(%r13)
	je	.L127
	movq	%rbx, %rdi
	call	strlen@PLT
	testq	%rax, %rax
	movq	%rax, %r12
	setne	11(%rsp)
	subq	%r14, %r13
	jmp	.L130
	.p2align 4,,10
	.p2align 3
.L128:
	addq	$1, %r14
	cmpb	$0, 0(%rbp,%r14)
	je	.L127
.L130:
	leaq	0(%r13,%r14), %rdi
	movq	%r12, %rdx
	movq	%rbx, %rsi
	call	strncmp@PLT
	testl	%eax, %eax
	jne	.L128
	cmpb	$0, 11(%rsp)
	je	.L128
	addq	%r12, %r14
.L129:
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	movq	%r14, %rax
	popq	%rbx
	.cfi_def_cfa_offset 48
	subq	%r15, %rax
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L127:
	.cfi_restore_state
	xorl	%eax, %eax
	cmpb	$0, 12(%rsp)
	jne	.L129
	addq	$24, %rsp
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE68:
	.size	lex_match_region, .-lex_match_region
	.p2align 4
	.globl	lex_tkstr_tmp
	.type	lex_tkstr_tmp, @function
lex_tkstr_tmp:
.LFB69:
	.cfi_startproc
	endbr64
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	32(%rsp), %rbx
	movl	$1024, %eax
	leaq	buf.2(%rip), %rcx
	movq	24(%rsp), %rsi
	movq	%rcx, %rdi
	cmpq	%rax, %rbx
	cmova	%rax, %rbx
	addq	16(%rsp), %rsi
	movq	%rbx, %rdx
	call	strncpy@PLT
	movb	$0, (%rax,%rbx)
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE69:
	.size	lex_tkstr_tmp, .-lex_tkstr_tmp
	.p2align 4
	.globl	lex_tkstr_dup
	.type	lex_tkstr_dup, @function
lex_tkstr_dup:
.LFB70:
	.cfi_startproc
	endbr64
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	32(%rsp), %rbx
	movq	%rbx, %rdi
	call	malloc@PLT
	movq	24(%rsp), %rsi
	movq	%rbx, %rcx
	movq	%rbx, %rdx
	addq	16(%rsp), %rsi
	movq	%rax, %rdi
	call	__strncpy_chk@PLT
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE70:
	.size	lex_tkstr_dup, .-lex_tkstr_dup
	.p2align 4
	.globl	lex_curcol
	.type	lex_curcol, @function
lex_curcol:
.LFB71:
	.cfi_startproc
	endbr64
	movq	16(%rsp), %rcx
	movl	%ecx, %eax
	subl	$1, %eax
	js	.L157
	movl	%eax, %eax
	xorl	%edx, %edx
	addq	8(%rsp), %rcx
	jmp	.L156
	.p2align 4,,10
	.p2align 3
.L159:
	subq	$1, %rax
	addq	$1, %rdx
	testl	%eax, %eax
	js	.L154
.L156:
	cmpb	$10, (%rcx,%rax)
	jne	.L159
.L154:
	movq	%rdx, %rax
	ret
.L157:
	xorl	%edx, %edx
	jmp	.L154
	.cfi_endproc
.LFE71:
	.size	lex_curcol, .-lex_curcol
	.p2align 4
	.globl	lex_curline
	.type	lex_curline, @function
lex_curline:
.LFB72:
	.cfi_startproc
	endbr64
	movq	16(%rsp), %rdx
	testq	%rdx, %rdx
	je	.L182
	leaq	-1(%rdx), %rax
	movq	8(%rsp), %rcx
	cmpq	$14, %rax
	jbe	.L183
	movq	%rdx, %rdi
	pxor	%xmm3, %xmm3
	pxor	%xmm7, %xmm7
	movq	%rcx, %rax
	andq	$-16, %rdi
	movdqa	.LC2(%rip), %xmm5
	pxor	%xmm6, %xmm6
	pxor	%xmm4, %xmm4
	movdqa	.LC1(%rip), %xmm8
	leaq	(%rdi,%rcx), %rsi
	.p2align 4,,10
	.p2align 3
.L163:
	movdqu	(%rax), %xmm0
	movdqa	%xmm7, %xmm2
	movdqa	%xmm6, %xmm9
	movdqa	%xmm6, %xmm10
	movdqa	%xmm4, %xmm11
	addq	$16, %rax
	pcmpeqb	%xmm8, %xmm0
	pcmpgtb	%xmm0, %xmm2
	movdqa	%xmm0, %xmm1
	punpcklbw	%xmm2, %xmm1
	punpckhbw	%xmm2, %xmm0
	pcmpgtw	%xmm1, %xmm9
	pcmpgtw	%xmm0, %xmm10
	movdqa	%xmm1, %xmm2
	punpcklwd	%xmm9, %xmm2
	punpckhwd	%xmm9, %xmm1
	movdqa	%xmm0, %xmm9
	pcmpgtd	%xmm2, %xmm11
	punpcklwd	%xmm10, %xmm9
	punpckhwd	%xmm10, %xmm0
	movdqa	%xmm2, %xmm10
	punpckhdq	%xmm11, %xmm10
	punpckldq	%xmm11, %xmm2
	movdqa	%xmm4, %xmm11
	pcmpgtd	%xmm1, %xmm11
	pand	%xmm5, %xmm10
	psubq	%xmm2, %xmm10
	movdqa	%xmm1, %xmm2
	punpckhdq	%xmm11, %xmm2
	punpckldq	%xmm11, %xmm1
	pand	%xmm5, %xmm2
	psubq	%xmm1, %xmm2
	movdqa	%xmm9, %xmm1
	paddq	%xmm10, %xmm2
	movdqa	%xmm4, %xmm10
	pcmpgtd	%xmm9, %xmm10
	punpckhdq	%xmm10, %xmm1
	punpckldq	%xmm10, %xmm9
	movdqa	%xmm0, %xmm10
	pand	%xmm5, %xmm1
	psubq	%xmm9, %xmm1
	movdqa	%xmm4, %xmm9
	pcmpgtd	%xmm0, %xmm9
	punpckldq	%xmm9, %xmm10
	punpckhdq	%xmm9, %xmm0
	psubq	%xmm10, %xmm1
	psubq	%xmm0, %xmm3
	paddq	%xmm2, %xmm1
	paddq	%xmm1, %xmm3
	cmpq	%rax, %rsi
	jne	.L163
	movdqa	%xmm3, %xmm0
	movl	%edi, %esi
	psrldq	$8, %xmm0
	paddq	%xmm0, %xmm3
	movq	%xmm3, %rax
	addq	$1, %rax
	cmpq	%rdx, %rdi
	je	.L185
.L162:
	cmpb	$10, (%rcx,%rdi)
	jne	.L166
	addq	$1, %rax
.L166:
	leal	1(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L160
	cmpb	$10, (%rcx,%rdi)
	je	.L186
.L167:
	leal	2(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L160
	cmpb	$10, (%rcx,%rdi)
	je	.L187
.L168:
	leal	3(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L160
	cmpb	$10, (%rcx,%rdi)
	je	.L188
.L169:
	leal	4(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L160
	cmpb	$10, (%rcx,%rdi)
	je	.L189
	leal	5(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L160
.L194:
	cmpb	$10, (%rcx,%rdi)
	jne	.L171
	addq	$1, %rax
.L171:
	leal	6(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L160
	cmpb	$10, (%rcx,%rdi)
	jne	.L172
	addq	$1, %rax
.L172:
	leal	7(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L160
	cmpb	$10, (%rcx,%rdi)
	jne	.L173
	addq	$1, %rax
.L173:
	leal	8(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L160
	cmpb	$10, (%rcx,%rdi)
	jne	.L174
	addq	$1, %rax
.L174:
	leal	9(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L160
	cmpb	$10, (%rcx,%rdi)
	je	.L190
.L175:
	leal	10(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L160
	cmpb	$10, (%rcx,%rdi)
	je	.L191
.L176:
	leal	11(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L160
	cmpb	$10, (%rcx,%rdi)
	je	.L192
.L177:
	leal	12(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L160
	cmpb	$10, (%rcx,%rdi)
	je	.L193
.L178:
	leal	13(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L160
	cmpb	$10, (%rcx,%rdi)
	jne	.L179
	addq	$1, %rax
.L179:
	addl	$14, %esi
	movslq	%esi, %rsi
	cmpq	%rdx, %rsi
	jnb	.L160
	cmpb	$10, (%rcx,%rsi)
	jne	.L160
	addq	$1, %rax
	ret
	.p2align 4,,10
	.p2align 3
.L182:
	movl	$1, %eax
.L160:
	ret
	.p2align 4,,10
	.p2align 3
.L186:
	addq	$1, %rax
	jmp	.L167
	.p2align 4,,10
	.p2align 3
.L187:
	addq	$1, %rax
	jmp	.L168
	.p2align 4,,10
	.p2align 3
.L188:
	addq	$1, %rax
	jmp	.L169
	.p2align 4,,10
	.p2align 3
.L189:
	leal	5(%rsi), %edi
	addq	$1, %rax
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jb	.L194
	ret
	.p2align 4,,10
	.p2align 3
.L190:
	addq	$1, %rax
	jmp	.L175
.L191:
	addq	$1, %rax
	jmp	.L176
.L192:
	addq	$1, %rax
	jmp	.L177
.L193:
	addq	$1, %rax
	jmp	.L178
.L183:
	xorl	%esi, %esi
	movl	$1, %eax
	xorl	%edi, %edi
	jmp	.L162
.L185:
	ret
	.cfi_endproc
.LFE72:
	.size	lex_curline, .-lex_curline
	.p2align 4
	.globl	lex_curreset
	.type	lex_curreset, @function
lex_curreset:
.LFB73:
	.cfi_startproc
	endbr64
	pxor	%xmm0, %xmm0
	movups	%xmm0, 8(%rdi)
	ret
	.cfi_endproc
.LFE73:
	.size	lex_curreset, .-lex_curreset
	.p2align 4
	.globl	lex_curpos
	.type	lex_curpos, @function
lex_curpos:
.LFB74:
	.cfi_startproc
	endbr64
	movq	16(%rsp), %rdx
	movq	8(%rsp), %rax
	testq	%rdx, %rdx
	je	.L218
	leaq	-1(%rdx), %rcx
	cmpq	$14, %rcx
	jbe	.L219
	movq	%rdx, %rdi
	pxor	%xmm3, %xmm3
	pxor	%xmm7, %xmm7
	movq	%rax, %rcx
	andq	$-16, %rdi
	movdqa	.LC2(%rip), %xmm5
	pxor	%xmm6, %xmm6
	pxor	%xmm4, %xmm4
	movdqa	.LC1(%rip), %xmm8
	leaq	(%rax,%rdi), %rsi
	.p2align 4,,10
	.p2align 3
.L199:
	movdqu	(%rcx), %xmm0
	movdqa	%xmm7, %xmm2
	movdqa	%xmm6, %xmm9
	movdqa	%xmm6, %xmm10
	movdqa	%xmm4, %xmm11
	addq	$16, %rcx
	pcmpeqb	%xmm8, %xmm0
	pcmpgtb	%xmm0, %xmm2
	movdqa	%xmm0, %xmm1
	punpcklbw	%xmm2, %xmm1
	punpckhbw	%xmm2, %xmm0
	pcmpgtw	%xmm1, %xmm9
	pcmpgtw	%xmm0, %xmm10
	movdqa	%xmm1, %xmm2
	punpcklwd	%xmm9, %xmm2
	punpckhwd	%xmm9, %xmm1
	movdqa	%xmm0, %xmm9
	pcmpgtd	%xmm2, %xmm11
	punpcklwd	%xmm10, %xmm9
	punpckhwd	%xmm10, %xmm0
	movdqa	%xmm2, %xmm10
	punpckhdq	%xmm11, %xmm10
	punpckldq	%xmm11, %xmm2
	movdqa	%xmm4, %xmm11
	pcmpgtd	%xmm1, %xmm11
	pand	%xmm5, %xmm10
	psubq	%xmm2, %xmm10
	movdqa	%xmm1, %xmm2
	punpckhdq	%xmm11, %xmm2
	punpckldq	%xmm11, %xmm1
	pand	%xmm5, %xmm2
	psubq	%xmm1, %xmm2
	movdqa	%xmm9, %xmm1
	paddq	%xmm10, %xmm2
	movdqa	%xmm4, %xmm10
	pcmpgtd	%xmm9, %xmm10
	punpckhdq	%xmm10, %xmm1
	punpckldq	%xmm10, %xmm9
	movdqa	%xmm0, %xmm10
	pand	%xmm5, %xmm1
	psubq	%xmm9, %xmm1
	movdqa	%xmm4, %xmm9
	pcmpgtd	%xmm0, %xmm9
	punpckldq	%xmm9, %xmm10
	punpckhdq	%xmm9, %xmm0
	psubq	%xmm10, %xmm1
	psubq	%xmm0, %xmm3
	paddq	%xmm2, %xmm1
	paddq	%xmm1, %xmm3
	cmpq	%rsi, %rcx
	jne	.L199
	movdqa	%xmm3, %xmm0
	movl	%edi, %esi
	psrldq	$8, %xmm0
	paddq	%xmm0, %xmm3
	movq	%xmm3, %rcx
	addq	$1, %rcx
	cmpq	%rdi, %rdx
	je	.L200
.L198:
	cmpb	$10, (%rax,%rdi)
	jne	.L201
	addq	$1, %rcx
.L201:
	leal	1(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L200
	cmpb	$10, (%rax,%rdi)
	je	.L222
.L202:
	leal	2(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L200
	cmpb	$10, (%rax,%rdi)
	je	.L223
.L203:
	leal	3(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L200
	cmpb	$10, (%rax,%rdi)
	je	.L224
.L204:
	leal	4(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L200
	cmpb	$10, (%rax,%rdi)
	je	.L225
	leal	5(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L200
.L231:
	cmpb	$10, (%rax,%rdi)
	jne	.L206
	addq	$1, %rcx
.L206:
	leal	6(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L200
	cmpb	$10, (%rax,%rdi)
	jne	.L207
	addq	$1, %rcx
.L207:
	leal	7(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L200
	cmpb	$10, (%rax,%rdi)
	jne	.L208
	addq	$1, %rcx
.L208:
	leal	8(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L200
	cmpb	$10, (%rax,%rdi)
	jne	.L209
	addq	$1, %rcx
.L209:
	leal	9(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L200
	cmpb	$10, (%rax,%rdi)
	je	.L226
.L210:
	leal	10(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L200
	cmpb	$10, (%rax,%rdi)
	je	.L227
.L211:
	leal	11(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L200
	cmpb	$10, (%rax,%rdi)
	je	.L228
.L212:
	leal	12(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L200
	cmpb	$10, (%rax,%rdi)
	je	.L229
.L213:
	leal	13(%rsi), %edi
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jnb	.L200
	cmpb	$10, (%rax,%rdi)
	jne	.L214
	addq	$1, %rcx
.L214:
	addl	$14, %esi
	movslq	%esi, %rsi
	cmpq	%rdx, %rsi
	jnb	.L200
	cmpb	$10, (%rax,%rsi)
	jne	.L200
	addq	$1, %rcx
	.p2align 4,,10
	.p2align 3
.L200:
	leal	-1(%rdx), %edi
	addq	%rdi, %rdx
	addq	%rdx, %rax
	xorl	%edx, %edx
	jmp	.L217
	.p2align 4,,10
	.p2align 3
.L230:
	leaq	1(%rdx), %rsi
	subq	$1, %rax
	cmpq	%rdi, %rdx
	movq	%rsi, %rdx
	je	.L197
.L217:
	cmpb	$10, (%rax)
	jne	.L230
.L197:
	movq	%rcx, %rax
	ret
	.p2align 4,,10
	.p2align 3
.L222:
	addq	$1, %rcx
	jmp	.L202
	.p2align 4,,10
	.p2align 3
.L223:
	addq	$1, %rcx
	jmp	.L203
	.p2align 4,,10
	.p2align 3
.L224:
	addq	$1, %rcx
	jmp	.L204
	.p2align 4,,10
	.p2align 3
.L225:
	leal	5(%rsi), %edi
	addq	$1, %rcx
	movslq	%edi, %rdi
	cmpq	%rdx, %rdi
	jb	.L231
	jmp	.L200
	.p2align 4,,10
	.p2align 3
.L226:
	addq	$1, %rcx
	jmp	.L210
.L227:
	addq	$1, %rcx
	jmp	.L211
.L228:
	addq	$1, %rcx
	jmp	.L212
.L229:
	addq	$1, %rcx
	jmp	.L213
	.p2align 4,,10
	.p2align 3
.L218:
	movl	$1, %ecx
	movq	%rcx, %rax
	ret
.L219:
	xorl	%esi, %esi
	movl	$1, %ecx
	xorl	%edi, %edi
	jmp	.L198
	.cfi_endproc
.LFE74:
	.size	lex_curpos, .-lex_curpos
	.p2align 4
	.globl	lex_curmove
	.type	lex_curmove, @function
lex_curmove:
.LFB75:
	.cfi_startproc
	endbr64
	addq	%rsi, 8(%rdi)
	ret
	.cfi_endproc
.LFE75:
	.size	lex_curmove, .-lex_curmove
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC3:
	.string	"\033[%d;%dm%.*s"
.LC4:
	.string	"\033[0m"
.LC5:
	.string	"\033[%d;%dm%s "
	.text
	.p2align 4
	.globl	lex_print_hl
	.type	lex_print_hl, @function
lex_print_hl:
.LFB76:
	.cfi_startproc
	endbr64
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pxor	%xmm0, %xmm0
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	movl	%edi, %r13d
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	leaq	colors.0(%rip), %r12
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	leaq	styles.1(%rip), %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	leaq	.LC3(%rip), %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 64
	movb	$1, 137(%rsp)
	movups	%xmm0, 88(%rsp)
	.p2align 4,,10
	.p2align 3
.L235:
	leaq	64(%rsp), %rdi
	call	lex_current.constprop.0
	testb	%al, %al
	je	.L245
.L236:
	movslq	128(%rsp), %rax
	movl	120(%rsp), %r8d
	movq	112(%rsp), %r9
	addq	104(%rsp), %r9
	imulq	$-1840700269, %rax, %rdx
	movl	%eax, %edi
	movq	%rax, %rcx
	sarl	$31, %edi
	shrq	$32, %rdx
	addl	%eax, %edx
	imulq	$715827883, %rax, %rax
	sarl	$2, %edx
	subl	%edi, %edx
	leal	0(,%rdx,8), %esi
	shrq	$32, %rax
	subl	%edx, %esi
	subl	%edi, %eax
	movl	$2, %edi
	subl	%esi, %ecx
	cltq
	movq	%rbx, %rsi
	movslq	%ecx, %rcx
	movl	0(%rbp,%rax,4), %edx
	xorl	%eax, %eax
	movl	(%r12,%rcx,4), %ecx
	call	__printf_chk@PLT
	cmpb	$0, 136(%rsp)
	je	.L235
	leaq	64(%rsp), %rdi
	movq	96(%rsp), %rax
	movb	$0, 136(%rsp)
	addq	%rax, 88(%rsp)
	movq	$0, 96(%rsp)
	call	lex_current.constprop.0
	testb	%al, %al
	jne	.L236
.L245:
	leaq	.LC4(%rip), %rdi
	call	puts@PLT
	testb	%r13b, %r13b
	je	.L233
	movq	72(%rsp), %rbx
	testq	%rbx, %rbx
	je	.L238
	movq	64(%rsp), %rbp
	movl	$1, %r14d
	movl	$31, %ecx
	xorl	%edx, %edx
	leaq	styles.1(%rip), %r15
	movl	$2863311531, %r13d
	leaq	colors.0(%rip), %r12
	jmp	.L239
	.p2align 4,,10
	.p2align 3
.L246:
	movl	%r14d, %ecx
	movl	%r14d, %r9d
	movq	%rcx, %rax
	imulq	$613566757, %rcx, %rcx
	imulq	%r13, %rax
	shrq	$32, %rcx
	shrq	$34, %rax
	movl	(%r15,%rax,4), %edx
	movl	%r14d, %eax
	movq	%rsi, %r14
	subl	%ecx, %eax
	shrl	%eax
	addl	%ecx, %eax
	shrl	$2, %eax
	leal	0(,%rax,8), %ecx
	subl	%eax, %ecx
	subl	%ecx, %r9d
	movslq	%r9d, %r9
	movl	(%r12,%r9,4), %ecx
.L239:
	movq	0(%rbp), %r8
	movl	$2, %edi
	xorl	%eax, %eax
	addq	$24, %rbp
	leaq	.LC5(%rip), %rsi
	call	__printf_chk@PLT
	leaq	1(%r14), %rsi
	cmpq	%r14, %rbx
	jne	.L246
.L238:
	leaq	.LC4(%rip), %rdi
	call	puts@PLT
.L233:
	addq	$8, %rsp
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE76:
	.size	lex_print_hl, .-lex_print_hl
	.p2align 4
	.globl	lex_builtin_rule_ws
	.type	lex_builtin_rule_ws, @function
lex_builtin_rule_ws:
.LFB77:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	40(%rsp), %rbp
	addq	32(%rsp), %rbp
	movsbq	0(%rbp), %rbx
	testb	%bl, %bl
	je	.L250
	call	__ctype_b_loc@PLT
	movq	(%rax), %rdx
	xorl	%eax, %eax
	jmp	.L249
	.p2align 4,,10
	.p2align 3
.L256:
	addq	$1, %rax
	movsbq	0(%rbp,%rax), %rbx
	testb	%bl, %bl
	je	.L247
.L249:
	testb	$32, 1(%rdx,%rbx,2)
	jne	.L256
.L247:
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L250:
	.cfi_restore_state
	xorl	%eax, %eax
	jmp	.L247
	.cfi_endproc
.LFE77:
	.size	lex_builtin_rule_ws, .-lex_builtin_rule_ws
	.p2align 4
	.globl	lex_builtin_rule_id
	.type	lex_builtin_rule_id, @function
lex_builtin_rule_id:
.LFB78:
	.cfi_startproc
	endbr64
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	24(%rsp), %rbx
	addq	16(%rsp), %rbx
	call	__ctype_b_loc@PLT
	movsbq	(%rbx), %rdx
	movq	(%rax), %rcx
	testb	$4, 1(%rcx,%rdx,2)
	jne	.L265
	cmpb	$36, %dl
	sete	%sil
	cmpb	$95, %dl
	sete	%al
	xorl	%edx, %edx
	orb	%al, %sil
	je	.L257
.L265:
	movzbl	1(%rbx), %eax
	testb	%al, %al
	je	.L264
	movl	$1, %edx
.L262:
	movsbq	%al, %rsi
	testb	$8, (%rcx,%rsi,2)
	jne	.L260
	cmpb	$36, %al
	je	.L260
	cmpb	$95, %al
	je	.L260
.L257:
	movq	%rdx, %rax
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L260:
	.cfi_restore_state
	addq	$1, %rdx
	movzbl	(%rbx,%rdx), %eax
	testb	%al, %al
	jne	.L262
.L264:
	xorl	%edx, %edx
	popq	%rbx
	.cfi_def_cfa_offset 8
	movq	%rdx, %rax
	ret
	.cfi_endproc
.LFE78:
	.size	lex_builtin_rule_id, .-lex_builtin_rule_id
	.p2align 4
	.globl	lex_builtin_rule_dqstring
	.type	lex_builtin_rule_dqstring, @function
lex_builtin_rule_dqstring:
.LFB79:
	.cfi_startproc
	endbr64
	movq	16(%rsp), %rdx
	addq	8(%rsp), %rdx
	xorl	%eax, %eax
	cmpb	$34, (%rdx)
	je	.L279
.L272:
	ret
	.p2align 4,,10
	.p2align 3
.L279:
	movzbl	1(%rdx), %ecx
	testb	%cl, %cl
	je	.L272
	movl	$1, %eax
.L275:
	cmpb	$34, %cl
	je	.L280
.L274:
	addq	$1, %rax
	movzbl	(%rdx,%rax), %ecx
	testb	%cl, %cl
	jne	.L275
	xorl	%eax, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L280:
	cmpb	$92, -1(%rdx,%rax)
	je	.L274
	addq	$1, %rax
	ret
	.cfi_endproc
.LFE79:
	.size	lex_builtin_rule_dqstring, .-lex_builtin_rule_dqstring
	.p2align 4
	.globl	lex_builtin_rule_sqstring
	.type	lex_builtin_rule_sqstring, @function
lex_builtin_rule_sqstring:
.LFB80:
	.cfi_startproc
	endbr64
	movq	16(%rsp), %rdx
	addq	8(%rsp), %rdx
	xorl	%eax, %eax
	cmpb	$39, (%rdx)
	je	.L288
.L281:
	ret
	.p2align 4,,10
	.p2align 3
.L288:
	movzbl	1(%rdx), %ecx
	testb	%cl, %cl
	je	.L281
	movl	$1, %eax
.L284:
	cmpb	$39, %cl
	je	.L289
.L283:
	addq	$1, %rax
	movzbl	(%rdx,%rax), %ecx
	testb	%cl, %cl
	jne	.L284
	xorl	%eax, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L289:
	cmpb	$92, -1(%rdx,%rax)
	je	.L283
	addq	$1, %rax
	ret
	.cfi_endproc
.LFE80:
	.size	lex_builtin_rule_sqstring, .-lex_builtin_rule_sqstring
	.p2align 4
	.globl	lex_builtin_rule_string
	.type	lex_builtin_rule_string, @function
lex_builtin_rule_string:
.LFB81:
	.cfi_startproc
	endbr64
	movq	16(%rsp), %rdx
	addq	8(%rsp), %rdx
	movzbl	(%rdx), %ecx
	cmpb	$34, %cl
	je	.L306
	xorl	%eax, %eax
	cmpb	$39, %cl
	jne	.L290
	movzbl	1(%rdx), %ecx
	testb	%cl, %cl
	je	.L290
	movl	$1, %eax
.L297:
	cmpb	$39, %cl
	je	.L307
.L296:
	addq	$1, %rax
	movzbl	(%rdx,%rax), %ecx
	testb	%cl, %cl
	jne	.L297
.L304:
	xorl	%eax, %eax
.L290:
	ret
	.p2align 4,,10
	.p2align 3
.L307:
	cmpb	$92, -1(%rdx,%rax)
	je	.L296
	addq	$1, %rax
	ret
	.p2align 4,,10
	.p2align 3
.L306:
	movzbl	1(%rdx), %ecx
	testb	%cl, %cl
	je	.L304
	movl	$1, %eax
	cmpb	$34, %cl
	je	.L308
.L293:
	addq	$1, %rax
	movzbl	(%rdx,%rax), %ecx
	testb	%cl, %cl
	je	.L304
	cmpb	$34, %cl
	jne	.L293
.L308:
	cmpb	$92, -1(%rdx,%rax)
	je	.L293
	addq	$1, %rax
	ret
	.cfi_endproc
.LFE81:
	.size	lex_builtin_rule_string, .-lex_builtin_rule_string
	.section	.rodata.str1.1
.LC6:
	.string	"\n"
.LC7:
	.string	"#"
	.text
	.p2align 4
	.globl	lex_builtin_rule_pylike_comment
	.type	lex_builtin_rule_pylike_comment, @function
lex_builtin_rule_pylike_comment:
.LFB82:
	.cfi_startproc
	endbr64
	movl	$1, %edx
	leaq	.LC6(%rip), %rsi
	leaq	.LC7(%rip), %rdi
	jmp	lex_match_region
	.cfi_endproc
.LFE82:
	.size	lex_builtin_rule_pylike_comment, .-lex_builtin_rule_pylike_comment
	.section	.rodata.str1.1
.LC8:
	.string	";"
	.text
	.p2align 4
	.globl	lex_builtin_rule_asmlike_comment
	.type	lex_builtin_rule_asmlike_comment, @function
lex_builtin_rule_asmlike_comment:
.LFB83:
	.cfi_startproc
	endbr64
	movl	$1, %edx
	leaq	.LC6(%rip), %rsi
	leaq	.LC8(%rip), %rdi
	jmp	lex_match_region
	.cfi_endproc
.LFE83:
	.size	lex_builtin_rule_asmlike_comment, .-lex_builtin_rule_asmlike_comment
	.section	.rodata.str1.1
.LC9:
	.string	"//"
	.text
	.p2align 4
	.globl	lex_builtin_rule_clike_comment
	.type	lex_builtin_rule_clike_comment, @function
lex_builtin_rule_clike_comment:
.LFB84:
	.cfi_startproc
	endbr64
	movl	$1, %edx
	leaq	.LC6(%rip), %rsi
	leaq	.LC9(%rip), %rdi
	jmp	lex_match_region
	.cfi_endproc
.LFE84:
	.size	lex_builtin_rule_clike_comment, .-lex_builtin_rule_clike_comment
	.section	.rodata.str1.1
.LC10:
	.string	"*/"
.LC11:
	.string	"/*"
	.text
	.p2align 4
	.globl	lex_rule_builtin_rule_clike_mlcomment
	.type	lex_rule_builtin_rule_clike_mlcomment, @function
lex_rule_builtin_rule_clike_mlcomment:
.LFB85:
	.cfi_startproc
	endbr64
	xorl	%edx, %edx
	leaq	.LC10(%rip), %rsi
	leaq	.LC11(%rip), %rdi
	jmp	lex_match_region
	.cfi_endproc
.LFE85:
	.size	lex_rule_builtin_rule_clike_mlcomment, .-lex_rule_builtin_rule_clike_mlcomment
	.section	.rodata.str1.1
.LC12:
	.string	"%zu\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB86:
	.cfi_startproc
	endbr64
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$10, %edx
	movl	$2, %edi
	xorl	%eax, %eax
	leaq	.LC12(%rip), %rsi
	call	__printf_chk@PLT
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE86:
	.size	main, .-main
	.section	.rodata
	.align 16
	.type	colors.0, @object
	.size	colors.0, 28
colors.0:
	.long	31
	.long	32
	.long	33
	.long	34
	.long	35
	.long	36
	.long	37
	.align 16
	.type	styles.1, @object
	.size	styles.1, 24
styles.1:
	.long	0
	.long	1
	.long	3
	.long	4
	.long	7
	.long	9
	.local	buf.2
	.comm	buf.2,1024,32
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC1:
	.byte	10
	.byte	10
	.byte	10
	.byte	10
	.byte	10
	.byte	10
	.byte	10
	.byte	10
	.byte	10
	.byte	10
	.byte	10
	.byte	10
	.byte	10
	.byte	10
	.byte	10
	.byte	10
	.align 16
.LC2:
	.quad	1
	.quad	1
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
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
