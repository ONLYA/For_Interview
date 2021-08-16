/*
 * 1.c
 
 This is to increase a variable called "itter" every 1.02 seconds with timer interrupt. The timer resolution should be as high as possible.
 
 In order to make the most precise timer interrupt, the 16-bit Timer/Counter 1 is used.
 From the user manual p.101, the perferred calculated TOP value is 1.02/(1/16M*2*N)-1=31874 with prescaler N=256. CTC mode is used. Interrupt will be attached to the Compare timer.
 *
 * Created: 13/08/2021 18:05:25
 * Author : User
 */ 

#include <avr/io.h>

/* Declarations of the functions */
void init(void);
void init_timer1(void);

/* Declaration of itter variable. Volatile as it may be used unexpectively. unsigned integer as the count value as it cannot be negative. */
volatile unsigned int itter = 0;

int main(void)
{
    /* Initialisation */
	init();
	
	/* empty loop, probably do something in this loop */
    while (1) 
    {
    }
}

/************************************************************************/
/* Init function to call all initialisations                            */
/************************************************************************/
void init(void)
{
	init_timer1();
}

/************************************************************************/
/* Init the 16-bit Timer 1 and interrupt                                */
/* Set the TOP value as 31874 to achieve 1.02 seconds interrupt         */
/************************************************************************/
void init_timer1()
{
	/* Set CTC mode with TOP value of OCR1A, and set clock prescaler of 256 */
	TCCR1B = 1<<WGM12 | 1<<CS12;
	
	/* Set TOP value of 31874 */
	/* Set the lower 8 bits of 31874 */
	OCR1AL = (31874) & 0xFF;
	/* Set the higher 8 bits of 31874 */
	OCR1AH = (31874 >> 8) & 0xFF;
	
	/* Set Input Capture Interrupt Enable and Output Compare A Match Interrupt Enable */
	TIMSK1 = 1<<ICIE1 | 1<<OCIE1A;
}

/************************************************************************/
/* Timer 1 A Comparison Vector Interrupt Routine                        */
/************************************************************************/
ISR(TIMER1_COMPA_vect)
{
	itter += 1;	// Increase "itter" variable
}