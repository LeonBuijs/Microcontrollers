/*
 * OpdrachtB1.c
 *
 * Created: 14-3-2025 09:06:25
 * Author : leonb
 */

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>

#define BIT(x)      (1 << x)
#define DDR_SPI     DDRB
#define PORT_SPI    PORTB
#define SPI_SCK     1
#define SPI_MOSI    2
#define SPI_MISO    3
#define SPI_SS      0

// Functie declaraties
void spi_slaveSelect();
void spi_slaveDeSelect();
void wait(int ms);
void spi_masterInit(void);
void spi_write(unsigned char data);
void spi_writeWord(unsigned char address, unsigned char data);
void displayDriverInit();
void displayOn();
void displayOff();
void writeLedDisplay(int value);


void spi_masterInit(void)
{
    DDR_SPI = 0xFF;
    DDR_SPI &= ~BIT(SPI_MISO);
    PORT_SPI |= BIT(SPI_SS);
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1);
}

// Stuur een byte via SPI
void spi_write(unsigned char data)
{
    SPDR = data;
    while (!(SPSR & BIT(SPIF)));
}

// Stuur een woord (adres + data) via SPI
void spi_writeWord(unsigned char address, unsigned char data)
{
    spi_slaveSelect();
    spi_write(address);
    spi_write(data);
    spi_slaveDeSelect();
}

void spi_slaveSelect()
{
    PORTB &= ~BIT(SPI_SS);
}

void spi_slaveDeSelect()
{
    PORTB |= BIT(SPI_SS);
}

// Initialiseer MAX7219 displaydriver
void displayDriverInit()
{
    spi_writeWord(0x09, 0xFF);
    spi_writeWord(0x0A, 0x0F);
    spi_writeWord(0x0B, 0x03);
    spi_writeWord(0x0C, 0x01);
}

void displayOn()
{
    spi_writeWord(0x0C, 0x01);
}

void displayOff()
{
    spi_writeWord(0x0C, 0x00);
}

// Functie om een geheel getal op het 4-digit display te tonen
void writeLedDisplay(int value)
{
    int isNegative = 0;
    if (value < 0)
    {
        isNegative = 1;  // Markeer als negatief
        value = -value;  // Zet om naar positief voor verwerking
    }

    int digits[4] = {0};  // Array om afzonderlijke cijfers op te slaan
    int index = 0;

    // Splits het getal in afzonderlijke cijfers
    do
    {
        digits[index++] = value % 10;
        value /= 10;
    } while (value > 0 && index < 3);  // Maximaal 3 cijfers

    // Als het getal negatief was, zet een minteken op de juiste plek
    if (isNegative)
    {
        digits[index] = 10;  // 10 = code voor het minteken in MAX7219
        index++;
    }

    // Vul het display
    for (int i = 0; i < 4; i++)
    {
        spi_writeWord(i + 1, digits[i]);  // Stuur de cijfers naar de display
    }
}

void segment_init()
{
	DDRB = 0x01;
	spi_masterInit();
	displayDriverInit();
}
