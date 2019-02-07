/* mipslabwork.c
 This file written 2015 by F Lundevall
 This file should be changed by YOU! So add something here:
 Latest update 2015-08-28 by F Lundevall
 For copyright and licensing, see file COPYING */

#include <stdint.h>     /* Declarations of uint_32 and the like */
#include <pic32mx.h>    /* Declarations of system-specific addresses etc */
#include "mipslab.h"    /* Declatations for these labs */

#define TMR2PERIOD ((80000000 / 256) / 10) /* 100ms */
#if TMR2PERIOD > 0xffff
#error "TimerPeriodIsTooBig"
#endif

int mytime = 0x0000;
int timeoutcount = 0;
int prime = 1234567;
volatile int *porte = (volatile int *) 0xbf886110;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */

void user_isr (void) {
    
    
    if (IFS(0) & 0x100) {
        
        timeoutcount++;
        IFS(0) = 0;
        
        if (timeoutcount == 10) {
            
            time2string(textstring, mytime);
            display_string(3, textstring);
            display_update();
            tick( &mytime);
            timeoutcount = 0;
        }
        
    }
    if (IFS(0) & (1 << 15)){
        
        IFS(0) = 0;
        (*porte)++;
    }
    
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
    
    // Enable interrupts from TMR2
    IPC(2) = 4;
    IEC(0) = 0x100;
    // Enable interrupts for SW3
    IPC(3) = 0x1c000000;
    IEC(0) = IEC(0) | (1 << 15);
    
    
    
    // Enable interrupts globally
    enable_interrupt();
    
    return;
}

/* This function is called repetitively from the main program */
void labwork( void ) {
    
    prime = nextprime(prime);
    display_string(0, itoaconv(prime));
    display_update();
}




