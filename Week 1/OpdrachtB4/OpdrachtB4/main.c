/*
 * OpdrachtB4.c
 *
 * Created: 7-2-2025 12:13:03
 * Author : leonb
 */ 

#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>

void wait(int ms)
{
	for (int i = 0; i < ms; i++)
	{
		_delay_ms(1);
	}
}


int main(void)
{
	// Sets all the bits of port d for output
	DDRD = 0xFF;

	while(1)
	{
		// Turn of the first led
		PORTD = 0b10000000;
		wait(50);

		// Loop through all leds to turn them on
		for (int i = 0; i < 8; i++)
		{
			PORTD = PORTD >> 1;
			wait(50);
		}
	}
}

