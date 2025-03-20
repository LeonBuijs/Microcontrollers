/*
 * Week 4 TempSensor.c
 *
 * Created: 28-2-2025 13:59:58
 * Author : jaspe
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "bigavr_lcd.h"

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
	ADMUX = 0b11100011; // Interne referentie 2,56V, left-adjusted, kanaal 3 (PF3)
	ADCSRA = 0b10000110; // ADC-enable, geen interrupt, geen free-running, prescaler 64
}

// Start een enkele ADC meting en retourneer 8-bit resultaat
uint8_t adcRead()
{
	ADCSRA |= BIT(6); // Start conversie
	while (ADCSRA & BIT(6)); // Wachten tot conversie klaar is
	return ADCH; // Alleen de hoogste 8 bits gebruiken
}

// Methode die de temperatuur toont op het scherm
void print_temperature_lcd(int temperature) {
	lcd_clear();
	char tempString[30];
	sprintf(tempString, "Temperature %d", temperature);
	lcd_write_string(tempString);
}


int main(void)
{	
	DDRD = 0xFF;		// PORTD(7) output, PORTD(6:0) input
	DDRF = 0x00;		// PORTF als input (ADC)
	DDRA = 0xFF;		// PORTA als output
	adcInit();			// Initialiseer ADC
	init_4bits_mode();	// Initialiseer LCD
	lcd_clear();		// LCD clearen

		lcd_write_string("Temperature Test");

	while (1)
	{
		wait(1000);					// Wachten op aanvraag (polling)
		uint8_t adcValue = adcRead();	// Start conversie en toon 8-bit resultaat
		PORTA = adcValue;				// Data tonen op LEDs
		print_temperature_lcd(adcValue);	// temperatuur naar LCD printen
	}
}


