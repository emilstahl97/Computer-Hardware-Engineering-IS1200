/* mipslabwork.c
	 This file written 2015 by F Lundevall
	 This file should be changed by YOU! So add something here:
	 Latest update 2015-08-28 by F Lundevall
	 For copyright and licensing, see file COPYING */

#include <stdint.h>	 /* Declarations of uint_32 and the like */
#include <pic32mx.h>	/* Declarations of system-specific addresses etc */
#include "mipslab.h"	/* Declatations for these labs */

#define TMR2PERIOD ((80000000 / 256) / 10) /* 100ms */
#if TMR2PERIOD > 0xffff
#error "TimerPeriodIsTooBig"
#endif

int mytime = 0x0000;
int timeoutcount = 0;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
	return;
}

/* Lab-specific initialization goes here */
void labinit( void ) {

	// Set *E to address of TRISE.
	volatile int *E = (volatile int *) 0xbf886100;
	
	// Set last 8 bits to zero, i.e. sets them as output pins.
	*E = *E & 0xff00;

	// Initialize port D, set bits 11-5 as inputs.
	TRISD = TRISD & 0x0fe0;

	/*
	Set 0x70, 0111 000 for 1:256 prescaling.
	Set timeperiod.
	Reset timer.
	Start the timer by setting the 'ON' bit to '1', bit #15
	in T2CON.
	Heavily inspired byexercise 2, question 4.
	*/	
	T2CONSET = 0x70;
	PR2 = TMR2PERIOD;
	TMR2 = 0;
	T2CONSET = 0x8000; 

	return;
}

/* This function is called repetitively from the main program */
void labwork( void ) {

	int sw = getsw();
	int btn = getbtns();
	/* 
	Checks if bit 1(001) is pressed and 2(010) and so on ..	
	sw is a number 0-f, shift it into the right position and OR it with the correct zeroed byte of mytime.
	*/	

	if (btn & 1) {
		mytime = (sw << 4) | (mytime & 0xff0f);
	}
	if (btn & 2) {
		mytime = (sw << 8) | (mytime & 0xf0ff);
	}
	if (btn & 4) {
		mytime = (sw << 12) | (mytime & 0x0fff);
	}

	//delay( 1000 );
	
	// Check time-out event flag.
	if (IFS(0) & 0x100) {

		// Reset all event flags. (Not so elegant)
		IFS(0) = 0;		
		timeoutcount++;

		if (timeoutcount == 10) {

		time2string( textstring, mytime );
		display_string( 3, textstring );
		display_update();
		tick( &mytime );
		display_image(96, icon);
				
		timeoutcount = 0;		
		}
	}		
}


