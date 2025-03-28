/*
 * Opdracht3.c
 *
 * Created: 20-3-2025 11:35:41
 * Author : leonb
 */ 

#include <avr/eeprom.h>

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "segment.h"
#include "segmentD.h"
#include "bigavr_lcd.h"
#include <stdio.h>

#define BIT(x) (1 << (x))

volatile int temperature = 0;
volatile int trigger_temperature;
volatile uint8_t button_pressed = 0;  // Voor debounce

uint8_t EEMEM storedTriggerTemp = 26;  // Trigger temp opslaan in EEPROM

void saveTriggerTemp(uint8_t value) {
	eeprom_update_byte(&storedTriggerTemp, value);
}

uint8_t readTriggerTemp() {
	return eeprom_read_byte(&storedTriggerTemp);
}

void wait(int ms)
{
	for (int tms = 0; tms < ms; tms++)
	{
		_delay_ms(1);
	}
}

void adcInit(void)
{
	ADMUX = 0b11100011; // AREF = VCC, left-adjusted, kanaal 3 (PF3)
	ADCSRA = 0b10000110; // ADC-enable, geen interrupt, geen free-running, prescaler 64
}

uint8_t adcRead()
{
	ADCSRA |= BIT(6); // Start conversie
	while (ADCSRA & BIT(6)); // Wachten tot conversie klaar is
	return ADCH; // Alleen de hoogste 8 bits gebruiken
}

void print_temperature_lcd(int temperature, int trigger_temp) {
	lcd_clear();
	char tempString[30];
	sprintf(tempString, "Temp: %dC", temperature);
	lcd_write_string(tempString);
	lcd_write_command(0xC0); // Ga naar tweede regel
	char triggerString[30];
	sprintf(triggerString, "Trigger: %dC", trigger_temp);
	lcd_write_string(triggerString);
}

void initInterrupts() {
	EICRA |= (1 << ISC11) | (1 << ISC01); // Interrupt op vallende flank (falling edge)
	EIMSK |= (1 << INT0) | (1 << INT1); // Enable INT0 (PD2) en INT1 (PD3)
	sei(); // Globale interrupts inschakelen
}

ISR(INT0_vect) { // PD2 - trigger verlagen
	if (!button_pressed) {
		button_pressed = 1;
		if (trigger_temperature > 0) {
			trigger_temperature--;
			saveTriggerTemp(trigger_temperature);  // Opslaan in EEPROM
			print_temperature_lcd(temperature, trigger_temperature);
		}
	}
}

ISR(INT1_vect) { // PD3 - trigger verhogen
	if (!button_pressed) {
		button_pressed = 1;
		if (trigger_temperature < 50) {
			trigger_temperature++;
			saveTriggerTemp(trigger_temperature);  // Opslaan in EEPROM
			print_temperature_lcd(temperature, trigger_temperature);
		}
	}
}

int main(void)
{
	DDRF = 0x00; // PORTF als input (ADC)
	DDRA = 0xFF; // PORTA als output
	DDRG |= (1 << PG0); // PG0 als output
	
	DDRD &= ~((1 << PD2) | (1 << PD3)); // PD2 en PD3 als input
	PORTD |= (1 << PD2) | (1 << PD3); // Pull-ups inschakelen
	
	adcInit();
	segment_init();
	init_4bits_mode();
	lcd_clear();
	initInterrupts(); // Interrupts initialiseren

	// Laad trigger temperatuur uit EEPROM
	trigger_temperature = readTriggerTemp();
	if (trigger_temperature > 50) trigger_temperature = 26; // Standaardwaarde indien EEPROM corrupt is

	print_temperature_lcd(temperature, trigger_temperature);

	while (1)
	{
		wait(100);   // Wachten op aanvraag (polling)
		
		// Debounce resetten
		button_pressed = 0;

		uint8_t adc_value = adcRead(); // Lees ADC
		PORTA = adc_value; // Zet waarde op PORTA
		writeLedDisplay(adc_value);
		
		if (adc_value != temperature)
		{
			print_temperature_lcd(adc_value, trigger_temperature);
			temperature = adc_value;
		}
		
		if (adc_value >= trigger_temperature){
			PORTG |= (1 << PG0);
			} else {
			PORTG &= ~(1 << PG0);
		}
	}
}
