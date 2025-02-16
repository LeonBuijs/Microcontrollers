/*
 * OpdrachtB2.c
 *
 * Created: 7-2-2025 11:41:34
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
	// All pins PORTD are set to output
    DDRD = 0b11111111;			
    
    while (1) {
		// Write 10000000b PORTD, PD7 is turning on and the other ones off
	    PORTD = 0b10000000;
	    wait( 500 );
		
		// Write 01000000b PORTD, PD6 is turning on and the other ones off
	    PORTD = 0b01000000;			
	    wait( 500 );
    }

    return 1;
}



