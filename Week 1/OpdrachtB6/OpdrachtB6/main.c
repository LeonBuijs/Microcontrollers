/*
 * OpdrachtB6.c
 *
 * Created: 7-2-2025 13:02:05
 * Author : leonb
 */ 

#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

void wait(int ms)
{
	for (int i = 0; i < ms; i++)
	{
		_delay_ms(1);
	}
}

int main(void)
{
	bool speed_up = false;
	
	// Sets all the bits of port d to output
	DDRD = 0xFF;
	// Sets one bit of port a to input
    PORTA = 0x01;
	
	while (1) 
    {
		// If the button is pressed, speed up de blinking
		if(PINA&0x01){
			speed_up = !speed_up;
		}
		
		// Blinking slow or fast
		if (!speed_up)
		{
			PORTD = 0b10000000;
			wait(1000);
			PORTD = 0b00000000;
			wait(1000);
		} else {
			PORTD = 0b10000000;
			wait(250);
			PORTD = 0b00000000;
			wait(250);
		}
    }
}

