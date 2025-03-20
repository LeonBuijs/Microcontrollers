/*
 * Opdracht3.c
 *
 * Created: 28-2-2025 13:09:41
 * Author : leonb
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>

#define BIT(x) (1 << (x))

// Wachtfunctie voor 'ms' milliseconden
void wait(int ms)
{
	for (int tms = 0; tms < ms; tms++)
	{
		_delay_ms(1);
	}
}

// Initialiseer ADC voor kanaal 3, 8-bit resolutie, enkelvoudige meting
void adcInit(void)
{
	ADMUX = 0b01100011; // AREF = VCC, left-adjusted, kanaal 3 (PF3)
	ADCSRA = 0b10000110; // ADC-enable, geen interrupt, geen free-running, prescaler 64
}

// Start een enkele ADC meting en retourneer 8-bit resultaat
uint8_t adcRead()
{
	ADCSRA |= BIT(6); // Start conversie
	while (ADCSRA & BIT(6)); // Wachten tot conversie klaar is
	return ADCH; // Alleen de hoogste 8 bits gebruiken
}

int main(void)
{
	DDRF = 0x00; // PORTF als input (ADC)
	DDRA = 0xFF; // PORTA als output
	adcInit();   // Initialiseer ADC

	while (1)
	{
		wait(100);   // Wachten op aanvraag (polling)
		PORTA = adcRead(); // Start conversie en toon 8-bit resultaat
	}
}