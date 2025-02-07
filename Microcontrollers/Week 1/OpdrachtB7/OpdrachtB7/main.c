/*
 
OpdrachtB7.c*
Created: 7-2-2025 13:11:34
Author : leonb*/
#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>

enum state { startstate, state1, state2, state3, endstate };

void wait(int ms)
{
	for (int i = 0; i < ms; i++)
	{
		_delay_ms(1);
	}
}

enum state currentState = startstate;

int main(void)
{
    
	DDRA = 0x00;
    DDRD = 0xFF;

    PORTA = 0x00;
	//PORTD = 0b10000000;
	
	

    /* Replace with your application code */
    while (1) 
    {
        switch(currentState){
            case startstate:
                if (PINA & 0b01000000){ // 6
                    currentState = state1;
                } else if (PINA & 0b00100000) // 5
                {
                    currentState = state2;
                }
                break;
            case state1:
                if (PINA & 0b00100000){ // 5
                    currentState = state2;
                } else if (PINA & 0b10000000) // 7
                {
                    currentState = startstate;
                }
                break;
            case state2:
                if (PINA & 0b00100000){ // 5
                    currentState = state3;
                } else if (PINA & 0b01000000) // 6
                {
                    currentState = state1;
                } else if (PINA & 0b10000000) // 7
                {
                    currentState = startstate;
                }
                break;
            case state3:
                if (PINA & 0b00100000){ // 5
                    currentState = endstate;
                } else if (PINA & 0b01000000) // 6
                {
                    currentState = endstate;
                } else if (PINA & 0b10000000) // 7
                {
                    currentState = startstate;
                }
                break;
            case endstate:
                if (PINA & 0b10000000) // 7
                {
                    currentState = startstate;
                }
                break;

            //PORTD = 0b10000000 >> state;
        }
		
		switch (currentState)
		{
			case startstate:
			PORTD = 0b10000000;
			break;
			case state1:
			PORTD = 0b01000000;
			break;
			case state2:
			PORTD = 0b00100000;
			break;
			case state3:
			PORTD = 0b00010000;
			break;
			case endstate:
			PORTD = 0b00001000;
			break;
		}
		
		wait(100);
    }
}