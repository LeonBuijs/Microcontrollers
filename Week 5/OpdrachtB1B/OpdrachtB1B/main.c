/*
 * OpdrachtB1B.c
 *
 * Created: 14-3-2025 10:04:36
 * Author : leonb
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>

#define DDR_SPI DDRB
#define PORT_SPI PORTB
#define SPI_SS PB2
#define SPI_MOSI PB3
#define SPI_MISO PB4
#define SPI_SCK PB5

#define BIT(x) (1 << (x))

void wait(int ms)
{
	for (int i = 0; i < ms; i++)
	{
		_delay_ms(1);
	}
}

void spi_masterInit(void)
{
	DDR_SPI = 0xFF;							// Alle SPI-pinnen als output
	DDR_SPI &= ~BIT(SPI_MISO);				// MISO als input
	PORT_SPI |= BIT(SPI_SS);				// SS hoog houden (inactief)
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1);	// SPI Enable, Master Mode, fck/64 (125kHz)
}

// Schrijf een byte via SPI
void spi_write(unsigned char data)
{
	SPDR = data;							// Start transmissie
	while (!(SPSR & BIT(SPIF))); 			// Wachten op voltooiing
}

// Selecteer slave op bepaalde poort
void spi_slaveSelect(unsigned char chipNumber)
{
	PORTB &= ~BIT(chipNumber);
}

// Deselecteer slave
void spi_slaveDeSelect(unsigned char chipNumber)
{
	PORTB |= BIT(chipNumber);
}

// Initialiseer de MAX7219 display driver
void displayDriverInit()
{
	spi_slaveSelect(0);
	spi_write(0x09);      			// Decode mode
	spi_write(0xFF);				// BCD-mode voor alle digits
	spi_slaveDeSelect(0);

	spi_slaveSelect(0);
	spi_write(0x0A);      			// Intensiteit
	spi_write(0x0F);    			// Maximaal
	spi_slaveDeSelect(0);

	spi_slaveSelect(0);
	spi_write(0x0B);  				// Scan-limit
	spi_write(0x03);   				// Gebruik digits 0..3
	spi_slaveDeSelect(0);

	spi_slaveSelect(0);
	spi_write(0x0C); 				// Shutdown register
	spi_write(0x01); 				// Normale werking
	spi_slaveDeSelect(0);
}

// Display aanzetten
void displayOn()
{
	spi_slaveSelect(0);
	spi_write(0x0C);
	spi_write(0x01);
	spi_slaveDeSelect(0);
}

// Display uitzetten
void displayOff()
{
	spi_slaveSelect(0);
	spi_write(0x0C);
	spi_write(0x00);
	spi_slaveDeSelect(0);
}

int main()
{
	DDRB = 0x01;
	spi_masterInit();
	displayDriverInit();

	// Clear display
	for (char i = 1; i <= 4; i++)
	{
		spi_slaveSelect(0);
		spi_write(i);
		spi_write(0);
		spi_slaveDeSelect(0);
	}
	wait(1000);

	// Write 4-digit data
	for (char i = 1; i <= 4; i++)
	{
		spi_slaveSelect(0);
		spi_write(i);
		spi_write(i);  // Weergave van cijfer 1-4 op de juiste plaats
		spi_slaveDeSelect(0);
		
		wait(1000);
	}
	wait(1000);

	return (1);
}


