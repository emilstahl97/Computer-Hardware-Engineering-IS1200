  # vectors.S
  # This file written 2015 by Axel Isaksson
  # Modified 2015 by F Lundevall
  # For copyright and licensing, see file COPYING

.macro movi reg, val
	lui \reg, %hi(\val)
	ori \reg, \reg, %lo(\val)
.endm

.macro STUB num
	.align 4
	.section .vector_new_\num,"ax",@progbits
	.global __vector_\num
	__vector_\num:
		movi $k0, _isr_primary_install
		lw $k0, \num * 4($k0)
		jr $k0
.endm

.align 4
.global __use_isr_install
__use_isr_install:
STUB 0
STUB 1
STUB 2
STUB 3
STUB 4
STUB 5
STUB 6
STUB 7

STUB 8
STUB 9
STUB 10
STUB 11
STUB 12
STUB 13
STUB 14
STUB 15

STUB 16
STUB 17
STUB 18
STUB 19
STUB 20
STUB 21
STUB 22
STUB 23

STUB 24
STUB 25
STUB 26
STUB 27
STUB 28
STUB 29
STUB 30
STUB 31

STUB 32
STUB 33
STUB 34
STUB 35
STUB 36
STUB 37
STUB 38
STUB 39

STUB 40
STUB 41
STUB 42
STUB 43
STUB 44
STUB 45
STUB 46
STUB 47

STUB 48
STUB 49
STUB 50
STUB 51
STUB 52
STUB 53
STUB 54
STUB 55

STUB 56
STUB 57
STUB 58
STUB 59
STUB 60
STUB 61
STUB 62
STUB 63

.text

.align 4
.global _isr_primary_install
_isr_primary_install:
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline

.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline

.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline

.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline

.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline

.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline

.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline

.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline
.word _isr_trampoline

# Interrupts are handled here
.align 4
.set noreorder
.global _isr_trampoline
_isr_trampoline:	
	# this is an interrupt service routine

	# tell the assembler not to use $1 right now
	.set noat

	# save all caller-save registers, and also ra
	addi $sp,$sp,-72
	sw $ra, 0($sp)
	sw  $1, 4($sp) # $at
	sw  $2, 8($sp) # $v0
	sw  $3,12($sp) # $v1
	sw  $4,16($sp) # $a0
	sw  $5,20($sp) # $a1
	sw  $6,24($sp) # $a2
	sw  $7,28($sp) # $a3
	sw  $8,32($sp) # $t0
	sw  $9,36($sp) # $t1
	sw $10,40($sp) # $t2
	sw $11,44($sp) # $t3
	sw $12,48($sp) # $t4
	sw $13,52($sp) # $t5
	sw $14,56($sp) # $t6
	sw $15,60($sp) # $t7
	sw $24,64($sp) # $t8 
	sw $25,68($sp) # $t9 

	# Any callee-saved regs ($s0 etc) used by user's handler
	# will be saved and restored by that handler
	# (the C compiler will see to that).

	# call user's handler
	jal user_isr
	nop

	# restore saved registers
	lw $25,68($sp)
	lw $24,64($sp)
	lw $15,60($sp)
	lw $14,56($sp)
	lw $13,52($sp)
	lw $12,48($sp)
	lw $11,44($sp)
	lw $10,40($sp)
	lw  $9,36($sp)
	lw  $8,32($sp)
	lw  $7,28($sp)
	lw  $6,24($sp)
	lw  $5,20($sp)
	lw  $4,16($sp)
	lw  $3,12($sp)
	lw  $2, 8($sp)
	lw  $1, 4($sp)
	lw $ra, 0($sp)
	addi $sp,$sp,72

	.set at
	# now the assembler is allowed to use $1 again

	# standard epilogue follows
	eret
	nop


# Exceptions are handled here (trap, syscall, etc)
.section .gen_handler,"ax",@progbits
.set noreorder
.ent _gen_exception
_gen_exception:
	mfc0 $k0, $14, 0
	addi $k0, $k0, 4
	mtc0 $k0, $14, 0
	eret
	nop

.end _gen_exception
