/*
 * OpdrachtB2.c
 *
 * Created: 14-2-2025 13:50:24
 * Author : leonb
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void wait(int ms)
{
	for (int i = 0; i < ms; i++)
	{
		_delay_ms(1);
	}
}

// Method to handle inturupt of a button
ISR( INT0_vect ) {
	PORTC = PORTC >> 1;
	wait(500);
}


int main(void)
{
	// Init Port C and D
	DDRC = 0xFF;
	DDRD = 0xF0;
	PORTC = 0b10000000;

	// Init Interrupt hardware
	EICRA |= 0x0B;            // INT1 falling edge, INT0 rising edge
	EIMSK |= 0x03;            // Enable INT1 & INT0

	// Enable global interrupt system
	//SREG = 0x80;            // Of direct via SREG of via wrapper
	sei();

	while (1)
	{
	}
}

