/*
 * OpdrachtB3.c
 *
 * Created: 14-2-2025 12:21:30
 * Author : leonb
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Enum for all the numbers on the seven segment display with their values
enum SevenSegment {
	ZERO  = 0b10111111,
	ONE   = 0b00000110,
	TWO   = 0b01011011,
	THREE = 0b01001111,
	FOUR  = 0b01100110,
	FIVE  = 0b01101101,
	SIX   = 0b01111101,
	SEVEN = 0b00000111,
	EIGHT = 0b01111111,
	NINE  = 0b01101111,
	A     = 0b01110111,
	B     = 0b01111100, 
	C     = 0b00111001, 
	D     = 0b01011110, 
	E     = 0b01111001, 
	F     = 0b01110001  
};

// index to count
int currentIndex = 0;

// If the interupt button pressed, the value goes up by one
ISR( INT0_vect ) {
	if (currentIndex > 15)
	{
		return;
	} 
	
	currentIndex++;
}

// If the interupt button pressed, the value goes down by one
ISR( INT1_vect ) {
	
	if (currentIndex < 0)
	{
		return;
	}
	
	currentIndex--;
}

void wait( int ms ) {
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

int main( void ) {
	 DDRC = 0xFF;					// PORTC all output
	
	 // Init Interrupt hardware
	 EICRA |= 0x0B;            // INT1 falling edge, INT0 rising edge
	 EIMSK |= 0x03;            // Enable INT1 & INT0

	 // Enable global interrupt system
	 //SREG = 0x80;            // Of direct via SREG of via wrapper
	 sei();
	
	while (1==1) {
		// Switch case to set the correct value to the seven segment display
		switch (currentIndex){
			case 0:  PORTC = ZERO; break;
			case 1:  PORTC = ONE; break;
			case 2:  PORTC = TWO; break;
			case 3:  PORTC = THREE; break;
			case 4:  PORTC = FOUR; break;
			case 5:  PORTC = FIVE; break;
			case 6:  PORTC = SIX; break;
			case 7:  PORTC = SEVEN; break;
			case 8:  PORTC = EIGHT; break;
			case 9:  PORTC = NINE; break;
			case 10: PORTC = A; break;
			case 11: PORTC = B; break;
			case 12: PORTC = C; break;
			case 13: PORTC = D; break;
			case 14: PORTC = E; break;
			case 15: PORTC = F; break;
			default: PORTC = E; break;
		}
	}

	return 1;
}

