/*
 * OpdrachtB2.c
 *
 * Created: 21-2-2025 09:56:48
 * Author : leonb
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <avr/interrupt.h>

#define BIT(x) (1 << (x))

// Functie om Timer/Counter 2 in CTC mode in te stellen
void timer2Init(void) {
	OCR2 = 117.1875;  // 15ms hoog (met prescaler 64)
	TIMSK |= (1 << OCIE2);  // Zet de interrupt enable voor Timer/Counter2
	sei();  // Zet interrupts aan

	TCCR2 = 0b00001100;  // CTC mode, prescaler = 1024
}

// ISR voor de Timer/Counter2 compare match
ISR(TIMER2_COMP_vect) {
	static uint8_t pulseState = 0;  // Variabele voor de pulsen (hoog of laag)

	if (pulseState == 0) {
		PORTD |= (1 << PD7);   // Zet PORTD.7 hoog
		OCR2 = 218.75;            // Stel OCR2 in voor 25ms laag
		pulseState = 1;         // Zet de puls naar laag na 15ms
		} else if (pulseState == 1) {
		PORTD &= ~(1 << PD7);  // Zet PORTD.7 laag
		OCR2 = 117.1875;            // Stel OCR2 in voor 15ms hoog
		pulseState = 0;         // Zet de puls naar hoog na 25ms
	}
}

int main(void) {
	DDRD |= (1 << PD7);  // Zet PORTD.7 als output

	timer2Init();  // Initialiseer Timer/Counter2

	while (1) {
		// Het programma draait verder, de interrupt doet het werk
	}

	return 0;
}

