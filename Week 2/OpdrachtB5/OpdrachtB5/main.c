/*
 * OpdrachtB5.c
 *
 * Created: 14-2-2025 09:11:07
 * Author : leonb
 */ 

#define F_CPU 8e6

#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int main( void ) {
	init();
	char* str = "Hello world!??";
	display_text(str);
	

	return 1;
}

