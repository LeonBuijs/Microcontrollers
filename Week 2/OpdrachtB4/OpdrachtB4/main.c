/*
 * OpdrachtB4.c
 *
 * Created: 14-2-2025 13:52:44
 * Author : leonb
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>

// Struct to save the pattern with their data en the delay
typedef struct {
	unsigned char data;
	unsigned int delay ;
} PATTERN_STRUCT;

// The pattern of the type struct
PATTERN_STRUCT pattern[] = {
	{0x80, 150}, {0x00, 150},
	{0x80, 150}, {0x00, 150},
	{0x01, 150}, {0x02, 150}, {0x40, 150}, {0x20, 150},
	{0x01, 150}, {0x02, 150}, {0x40, 150}, {0x20, 150},
	{0x00, 150},
	{0x01, 150}, {0x03, 150}, {0x43, 150}, {0x63, 150},
	{0x01, 150}, {0x03, 150}, {0x43, 150}, {0x63, 150},
	{0x00, 150},
	{0xFF, 0}
};

void wait( int ms ) {
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}


int main( void ) {
	DDRD = 0b11111111;					// PORTD all output
	
	while (1==1) {
		// Set index to begin of pattern array
		int index = 0;
		// as long as delay has meaningful content
		while( pattern[index].delay != 0 ) {
			// Write data to PORTD
			PORTD = pattern[index].data;
			// wait
			wait(pattern[index].delay);	
			// increment for next round
			index++;
		}
	}

	return 1;
}

