@ This ARM Assembler code should implement a matching function, for use in the MasterMind program.
@It should produce as output 2 numbers, the first for the
@ exact matches (peg of right colour and in right position) and approximate matches (peg of right
@ color but not in right position). Make sure to count each peg just once!
	
@ Example (first sequence is secret, second sequence is guess):
@ 1 2 1
@ 3 1 3 ==> 0 1
@ You can return the result as a pointer to two numbers, or two values
@ encoded within one number
@
@ -----------------------------------------------------------------------------

.text
@ this is the matching fct that should be called from the C part of the CW	
.global         matches
@ use the name `main` here, for standalone testing of the assembler code
@ when integrating this code into `master-mind.c`, choose a different name
@ otw there will be a clash with the main function in the C code
.global         main
main: 
	LDR  R2, =secret	@ pointer to secret sequence
	LDR  R3, =guess		@ pointer to guess sequence

	@ you probably need to initialise more values here

	@ ... COMPLETE THE CODE BY ADDING YOUR CODE HERE, you should use sub-routines to structure your code
	LDR  R2, =secret	@ pointer to secret sequence
	LDR  R3, =guess		@ pointer to guess sequence

	BL   matches       @ call the matching function

	@ print the results
	MOV  R0, R4         @ move the result to output register
	MOV  R1, #f2str     @ move the format string to R1
	BL   printf         @ call the libc printf function

	@ exit
	MOV  R0, #0
	MOV  R7, #1
	SWI  0

exit:	@MOV	 R0, R4		@ load result to output register
	MOV 	 R7, #1		@ load system call code
	SWI 	 0		@ return this value

@ -----------------------------------------------------------------------------
@ sub-routines

@ this is the matching fct that should be callable from C	
matches:			@ Input: R0, R1 ... ptr to int arrays to match ; Output: R0 ... exact matches (10s) and approx matches (1s) of base COLORS
	@ COMPLETE THE CODE HERE
	matches:
    PUSH {LR}          @ save the return address
    MOV R1, #0         @ initialize exact matches to 0
    MOV R2, #0         @ initialize approximate matches to 0
    LDR R3, [R0]       @ load the first element of the secret sequence
    LDR R4, [R1]       @ load the first element of the guess sequence
check_exact:
    CMP R3, R4         @ compare the two elements
    BNE check_approx   @ if they are not equal, go to check approximate matches
    ADD R1, #4         @ if they are equal, increment both pointers by 4
    ADD R0, #4
    ADD R2, #10        @ increment the number of exact matches by 10
    CMP R1, R0         @ check if we've gone through the entire sequence
    BEQ exit_matches   @ if we have, exit the function
    LDR R3, [R0]       @ load the next element of the secret sequence
    LDR R4, [R1]       @ load the next element of the guess sequence
    B check_exact      @ check for exact matches again
check_approx:
    LDR R3, [R0]       @ load the first element of the secret sequence
    LDR R4, [R1]       @ load the first element of the guess sequence
    CMP R3, R4         @ compare the two elements
    BEQ found_color    @ if they are equal, go to check if the color is already counted
    ADD R1, #4         @ if they are not equal, increment only the guess pointer by 4
    CMP R1, R0         @ check if we've gone through the entire sequence
    BNE check_approx   @ if we haven't, continue checking for approximate matches
    B exit_matches     @ if we have, exit the function
found_color:
    ADD R3, #COL       @ add the number of colors to the corresponding counter
    STR R3, [R5, R4]   @ store the counter in the appropriate index of the counter array
    ADD R1, #4         @ increment the guess pointer by 4
    CMP R1, R0         @ check if we've gone through the entire sequence
    BEQ exit_matches   @ if we have, exit the function
    LDR R4, [R1]       @ load the next element of the guess sequence
    B check_approx     @ check for approximate matches again
exit_matches:
    MOV R0, R2         @ move the result to the output register
    POP {PC}           @ restore the return address and return

	

@ show the sequence in R0, use a call to printf in libc to do the printing, a useful function when debugging 
showseq: 			@ Input: R0 = pointer to a sequence of 3 int values to show
	@ COMPLETE THE CODE HERE (OPTIONAL)
    PUSH {LR}          @ save the return address
    STMFD SP!, {R0-R2} @ save R0-R2 on the stack
    LDR R0, =f4str     @ load the format string for printf
    LDR R1, [SP, #4]   @ load the first element of the sequence
    LDR R2, [SP, #8]   @ load the second element of the sequence
    MOV R3, R2, LSL #16@ move the third element of the sequence to the high 16 bits of R3
    ORR R3, R3, R1     @ OR the first and third elements together
    BL printf

	
@ =============================================================================

.data

@ constants about the basic setup of the game: length of sequence and number of colors	
.equ LEN, 3
.equ COL, 3
.equ NAN1, 8
.equ NAN2, 9

@ a format string for printf that can be used in showseq
f4str: .asciz "Seq:    %d %d %d\n"

@ a memory location, initialised as 0, you may need this in the matching fct
n: .word 0x00
	
@ INPUT DATA for the matching function
.align 4
secret: .word 1 
	.word 2 
	.word 1 

.align 4
guess:	.word 3 
	.word 1 
	.word 3 

@ Not strictly necessary, but can be used to test the result	
@ Expect Answer: 0 1
.align 4
expect: .byte 0
	.byte 1

.align 4
secret1: .word 1 
	 .word 2 
	 .word 3 

.align 4
guess1:	.word 1 
	.word 1 
	.word 2 

@ Not strictly necessary, but can be used to test the result	
@ Expect Answer: 1 1
.align 4
expect1: .byte 1
	 .byte 1

.align 4
secret2: .word 2 
	 .word 3
	 .word 2 

.align 4
guess2:	.word 3 
	.word 3 
	.word 1 

@ Not strictly necessary, but can be used to test the result	
@ Expect Answer: 1 0
.align 4
expect2: .byte 1
	 .byte 0

