# labwork.S
# Written 2015 by F Lundevall
# Skeleton file for IS1200/IS1500 lab 1 2015-2016.
# The macros PUSH and POP are in the public domain.

.global delay
.global    time2string
.global enable_interrupt

.macro    PUSH reg
addi    $sp,$sp,-4
sw    \reg,0($sp)
.endm

.macro    POP reg
lw    \reg,0($sp)
addi    $sp,$sp,4
.endm

# You can add your own code here
#
.data
.text

hexasc:
andi    $t0, $a0, 0xf

slti    $t1,$a0,10        # checks if $t0 < 9
bne        $t1,$zero,low    # if $t0 <= t0, goes to low

slti    $t1,$a0,16        # checks if $t0 < 15
bne        $t1,$zero,high    # if $t0 <= t0, goes to high

low:
addi    $v0,$t0, 0x30
jr $ra
nop
high:
addi    $v0,$t0, 0x37
jr $ra
nop

delay:

# constant
li $t1, 11500
# ms
move $t2, $a0


while:

# ms > 0;
ble $t2, 0, exit
# ms = ms - 1;
sub $t2, $t2, 1
# int i = 0;
li $t0, 0

for:

# i < $t1
bge $t0, $t1, while
nop
addi $t0, $t0, 1

j for
nop

j while
nop

exit:

jr $ra
nop

time2string:
PUSH $ra
PUSH $s0
PUSH $s1

move $s0, $a0 # Address where well store printable time
move $s1, $a1 # Address of mytime

#ADD CHECK FOR RESET?

andi $t0, $s1, 0xf000 # Get first digit
srl $a0, $t0, 0x0c # Shift right by 12
jal hexasc
nop
move $t1, $v0
sb $t1, 0($s0)

andi $t0, $s1, 0xf00 # Get second digit
srl $a0, $t0, 0x8 # Shift right by 8
jal hexasc
nop
move $t1, $v0
sb $t1, 1($s0)

li $t1, 0x3a # Load :
sb $t1, 2($s0)

andi $t0, $s1, 0xf0 # Get third digit
srl $a0, $t0, 0x4 # Shift right by 4
jal hexasc
nop
move $t1, $v0
sb $t1, 3($s0)

andi $a0, $s1, 0xf # Get last digit, no shift needed
jal hexasc
nop
move $t1, $v0
sb $t1, 4($s0)

# Surprise assignment
# Last digit stored in $a0 from previous
and $t2, $a0, 0x1
beq $t2, $zero, even
nop

li $t1, 0x44
sb $t1, 5($s0) # Store string terminator

j odd
nop

even:
li $t1, 0x45
sb $t1, 5($s0) # Store string terminator
odd:


li $t1, 0x0
sb $t1, 6($s0) # Store string terminator

POP $s1
POP $s0
POP $ra
jr $ra
nop

enable_interrupt:
ei
jr $ra
nop
