# labwork.S
# Written 2015-2017 by F Lundevall
# Skeleton file for IS1200/IS1500 lab 1.
# The macros PUSH and POP are in the public domain.
# Please add your own code at the end of the file.

#
# Please keep the two macros PUSH and POP unchanged
#
.macro	PUSH reg
addi	$sp,$sp,-4
sw	\reg,0($sp)
.endm

.macro	POP reg
lw	\reg,0($sp)
addi	$sp,$sp,4
.endm
#
# Please add your own code below this line
#
.data
.align 2
.text

.global delay
delay:

 	addi $t1,$0,4444
 	while:

 		ble $a0,$0,done
 		nop
 		#assign i
 		add $t0,$0,$0
 		#m= m-1
 		addi $a0,$a0,-1
 		for:

			ble $t1,$t0,while
			nop
			addi $t0,$t0,1
			j for
			nop

	done:
	jr $ra
	nop


.global hexasc
hexasc:
	andi $a0,0x0f			#makes the register $a0 the value of the input

	li $v0,0x30			#gives the return variable $v0 the value of zero

	ble $a0,0x00000009,digit	#if $a0 is a number between [0,9] -> jump to digit
	nop				#to prevent bugs

	addi $v0,$v0,0x7		#adds 7 to $v0 to expand the register $a0 up to number 15, to enable the use of letters [a,f]

.global digit
digit:					#jump here if the value on the regsiter $a0 is less than 9, or equal to 9
	nop
	add $v0,$v0,$a0			#puts the value from the register $a0 to the return value at reference $v0

	jr $ra				#jumps to main: and returns the values located at the initialized register
	nop				#safety numba one priority!


.global time2string
	 time2string:
	 	PUSH  $ra
	 	PUSH  $s0
	 	PUSH  $s1

	 	move $s0,$a0
	 	srl $a0,$a1,12
	 	jal hexasc
	 	nop
	 	sb $v0,0($s0)



	 	srl $a0,$a1,8
	 	jal hexasc
	 	nop
	 	sb $v0,1($s0)

		addi $s1,$0,0x3A
		sb $s1,2($s0)

		srl $a0,$a1,4
	 	jal hexasc
	 	nop
	 	sb $v0,3($s0)

	 	srl $a0,$a1,0
	 	jal hexasc
	 	nop
	 	sb $v0,4($s0)
	 	#extra uppgift printa E om j�mn D om udd
	 	move $s2,$a1
	 	andi $s2,$s2,0x01

	 	bne $s2,$0,udda
	 	nop

		addi $t0,$0,0x45
		sb $t0,5($s0)
		j end
		nop
	udda:

		addi $t0,$0,0x44
		sb $t0,5($s0)

	end:
	 	addi $s1,$0,0x00
		sb $s1,6($s0)

		POP $s1
		POP $s0
		POP $ra

		jr $ra
		nop
