
.text

	addi $a0,$0,4     # set $a0 to 4
	addi $v0,$a0,2	  # test addi, set $v0
	add  $v0,$v0,$a0  # start of counter. Should be 10
loop:	beq  $v0,$a0,done # test, jump to done. 
        addi $v0,$v0,-3   # decrement. Loops twice.
        beq  $0,$0,loop   # emulating an unconditional jump
done:	add  $0,$0,$0	  # NOP	
	