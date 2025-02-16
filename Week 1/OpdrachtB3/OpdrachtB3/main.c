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
{
	// Set PORTD for output
	DDRD = 0xFF;
	
	// Set PORTA for input
	DDRA=0x00;
	PORTA=0x01; // Set bit0 high, pull up resistor
	
	while(1)
	{
		// Button push is active LOW!!
		// Check if PA0 button is pressed
		if(PINA & 0x01)// Mask with 0b0000001, bit0
		{
			// Turns on the PD7 led and all the others off
			PORTD = 0b10000000;
			wait(250);
			// Turns on the PD6 led and all the others off
			PORTD = 0b00000000;
			wait(250);
		} else
		{
			// If the buttons isn't pressed, turn all leds off
			PORTD=0x00;
		}
	}
		return 1;
}

