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
#define DDR_SPI     DDRB                    // SPI Data direction register
#define PORT_SPI    PORTB                    // SPI Output register
#define SPI_SCK     1                        // PB1: SPI Pin System Clock
#define SPI_MOSI    2                        // PB2: SPI Pin MOSI
#define SPI_MISO    3                        // PB3: SPI Pin MISO
#define SPI_SS      0                        // PB0: SPI Pin Slave Select

// Functie declaraties
void spi_slaveSelect();
void spi_slaveDeSelect();
void spi_write(unsigned char data);
void spi_writeWord(unsigned char address, unsigned char data);
void spi_masterInit(void);
void displayDriverInit();
void displayOn();
void displayOff();
void writeLedDisplay(int value);
void wait(int ms);

// wait(): busy waiting for 'ms' millisecond
void wait(int ms)
{
    for (int i = 0; i < ms; i++)
    {
        _delay_ms(1);
    }
}

void spi_masterInit(void)
{
    DDR_SPI = 0xff;                         // Alle pins output: MOSI, SCK, SS
    DDR_SPI &= ~BIT(SPI_MISO);              // MISO blijft input
    PORT_SPI |= BIT(SPI_SS);                // SS hoog: geen slave geselecteerd
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1);    // SPI aan, master mode, klok /64
}

// Schrijf een byte naar de SPI slave
void spi_write(unsigned char data)                
{
    SPDR = data;                            // Stuur byte via SPI
    while (!(SPSR & BIT(SPIF)));            // Wacht tot verzending klaar is
}

// Schrijf een woord (adres + data) naar de SPI slave
void spi_writeWord(unsigned char address, unsigned char data) 
{
    spi_slaveSelect();                      
    spi_write(address);                     
    spi_write(data);                        
    spi_slaveDeSelect();                    
}

// Selecteer het SPI-apparaat (Slave Select laag)
void spi_slaveSelect()
{
    PORTB &= ~BIT(SPI_SS);
}

// Deselecteer het SPI-apparaat (Slave Select hoog)
void spi_slaveDeSelect()
{
    PORTB |= BIT(SPI_SS);
}

// Initialiseer de MAX7219 display driver
void displayDriverInit() 
{
    spi_writeWord(0x09, 0xFF);  // Register 09: Decode Mode -> BCD mode voor alle digits
    spi_writeWord(0x0A, 0x0F);  // Register 0A: Intensiteit -> Maximale intensiteit
    spi_writeWord(0x0B, 0x03);  // Register 0B: Scan-limiet -> Display cijfers 0..3
    spi_writeWord(0x0C, 0x01);  // Register 0C: Normale werking
}

// Zet het display aan
void displayOn() 
{
    spi_writeWord(0x0C, 0x01);
}

// Zet het display uit
void displayOff() 
{
    spi_writeWord(0x0C, 0x00);
}

// Toon een getal op het 4-digit LED-display
void writeLedDisplay(int value) 
{
    if (value < 0 || value > 9999) return; // Beperk de waarde tot 4 cijfers
    
    for (char i = 1; i <= 4; i++) 
    {
        char digit = value % 10;   // Haal het laatste cijfer
        value /= 10;               // Verwijder het laatste cijfer uit de waarde
        spi_writeWord(i, digit);   // Schrijf het cijfer naar het juiste display-segment
    }
}

int main()
{
    // Initialisatie van SPI en display
    DDRB = 0x01;                     // Zet PB0 als output voor display select
    spi_masterInit();                 // Initialiseer SPI
    displayDriverInit();              // Initialiseer display driver

    // Test met verschillende getallen
    writeLedDisplay(1234);
    wait(2000);
    
    writeLedDisplay(56);
    wait(2000);
    
    writeLedDisplay(7);
    wait(2000);
    
    writeLedDisplay(9999);
    wait(2000);

    return 1;
}
