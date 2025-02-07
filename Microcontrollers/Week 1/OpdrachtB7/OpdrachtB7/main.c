/*
 * OpdrachtB7.c
 *
 * Created: 7-2-2025 13:11:34
 * Author : leonb
 */ 

#include <avr/io.h>

typedef enum { startstate, state1, state2, state3, endstate } ownBool;

int main(void)
{
	ownBool state = startstate;
	DDRD = 0xFF;
	
	PORTA = 0x00;
	
    /* Replace with your application code */
    while (1) 
    {
		switch(state){
			case startstate:
				if (PINA & 0b00000110){ // 6
					state = state1;
				} else if (PINA & 0b00000101) // 5
				{
					state = state2;
				}
				break;
			case state1:
				if (PINA & 0b00000101){ // 5
					state = state2;
				} else if (PINA & 0b00000111) // 7
				{
					state = startstate;
				}
				break;
			case state2:
				if (PINA & 0b00000101){ // 5
					state = state3;
				} else if (PINA & 0b00000110) // 6
				{
					state = state1;
				} else if (PINA & 0b00000111) // 7
				{
					state = startstate;
				}
				break;
			case state3:
				if (PINA & 0b00000101){ // 5
					state = endstate;
				} else if (PINA & 0b00000110) // 6
				{
					state = endstate;
				} else if (PINA & 0b00000111) // 7
				{
					state = startstate;
				}
				break;
			case endstate:
				if (PINA & 0b00000111) // 7
				{
					state = startstate;
				}
				break;
				
			PORTD = 0b10000000 >> state;
		}
    }
}


