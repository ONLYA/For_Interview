/*
 * 1.c
 
 This is to increase a variable called "itter" every 1.02 seconds with timer interrupt. The timer resolution should be as high as possible.
 
 In order to make the most precise timer interrupt, the 16-bit Timer/Counter 1 is used.
 From the user manual p.101, the perferred calculated TOP value is 1.02/(1/16M*N)-1=63749 with prescaler N=256. CTC mode is used. Interrupt will be attached to the Compare timer.
 
 The code can be tested on real hardware by uncommenting the commented code. Probe PB0 to observe a square wave with the period of 1.02*2=2.04s.
 *
 * Created: 13/08/2021 18:05:25
 * Author : User
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

/* Declarations of the functions */
void init(void);
void init_timer1(void);

/* Declaration of itter variable. Volatile as it may be used unexpectively. unsigned integer as the count value as it cannot be negative. */
volatile unsigned int itter = 0;

int main(void)
{
    /* Initialisation */
	init();
	// DDRB = 1; // Uncomment this to test
	
	/* empty loop, probably do something in this loop */
    while (1) 
    {
		// PORTB = itter & 1; // Uncomment this to test
    }
}

/************************************************************************/
/* Init function to call all initialisations                            */
/************************************************************************/
void init(void)
{
	init_timer1(); // Init the Timer 1 configurations
}

/************************************************************************/
/* Init the 16-bit Timer 1 and interrupt                                */
/* Set the TOP value as 31874 to achieve 1.02 seconds interrupt         */
/************************************************************************/
void init_timer1()
{
	/* Set CTC mode with TOP value of OCR1A, and set clock prescaler of 256 */
	TCCR1B = 1<<WGM12 | 1<<CS12;
	
	/* Set TOP value of 63749 */
	OCR1A = 63749;
	
	/* Set Output Compare A Match Interrupt Enable */
	TIMSK1 = 1<<OCIE1A;
	
	/* Enable global interrupt */
	sei();
}

/************************************************************************/
/* Timer 1 A Comparison Vector Interrupt Routine                        */
/************************************************************************/
ISR(TIMER1_COMPA_vect)
{
	itter += 1;	// Increase "itter" variable
}