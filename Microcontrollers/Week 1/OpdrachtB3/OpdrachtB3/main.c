/*
 * OpdrachtB3.c
 *
 * Created: 7-2-2025 11:43:06
 * Author : leonb
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>

void wait(int ms)
{
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

int main(void)
{// Set PORTD for output except for bit7, this is an input, just an example
	DDRD=0xFF; // of 0b01111111
	
	// Set PORTA for input
	DDRA=0x00; // of 0b00000000, this line is not needed default setting is input
	PORTA=0x01; // Set bit0 high, pull up resistor
	
	/* Replace with your application code */
	while(1)
	{
		// Button push is active LOW!!
		if(PINA&0x01)// Mask with 0b0000001, bit0
		{
			PORTD = 0b10000000;
			wait(250);
			PORTD = 0b00000000;
			wait(250);
		} else
		{
			PORTD=0x00;
		}
	}
		return 1;
}

