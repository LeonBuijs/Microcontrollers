/*
 * OpdrachtB1.c
 *
 * Created: 14-3-2025 09:06:25
 * Author : leonb
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>

#define BIT(x)      ( 1<<x )
#define DDR_SPI     DDRB                    // spi Data direction register
#define PORT_SPI    PORTB                   // spi Output register
#define SPI_SCK     1                       // PB1: spi Pin System Clock
#define SPI_MOSI    2                       // PB2: spi Pin MOSI
#define SPI_MISO    3                       // PB3: spi Pin MISO
#define SPI_SS      0                       // PB0: spi Pin Slave Select

// Functie declaraties
void spi_slaveSelect();
void spi_slaveDeSelect();

// wait(): busy waiting for 'ms' millisecond
// used library: util/delay.h
void wait(int ms)
{
    for (int i=0; i<ms; i++)
    {
        _delay_ms(1);
    }
}

void spi_masterInit(void)
{
    DDR_SPI = 0xff;                         // All pins output: MOSI, SCK, SS, SS_display as output
    DDR_SPI &= ~BIT(SPI_MISO);              //  except: MISO input
    PORT_SPI |= BIT(SPI_SS);                // SS_ADC == 1: deselect slave
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);    // or: SPCR = 0b11010010;
                                            // Enable spi, MasterMode, Clock rate fck/64, bitrate=125kHz
                                            // Mode = 0: CPOL=0, CPPH=0;
}

// Write a byte from master to slave
void spi_write( unsigned char data )                
{
    SPDR = data;                            // Load byte to Data register --> starts transmission
    while( !(SPSR & BIT(SPIF)) );           // Wait for transmission complete 
}

// Write a word = address byte + data byte from master to slave
void spi_writeWord ( unsigned char address, unsigned char data ) 
{
    spi_slaveSelect();                      // Select display chip (geen argument meer)
    spi_write(address);                      // Send address byte
    spi_write(data);                         // Send data byte
    spi_slaveDeSelect();                    // Deselect display chip (geen argument meer)
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

// Initialize the driver chip (type MAX 7219)
void displayDriverInit() 
{
    spi_writeWord(0x09, 0xFF);  // Register 09: Decode Mode -> BCD mode for all digits
    spi_writeWord(0x0A, 0x0F);  // Register 0A: Intensity -> Maximum intensity
    spi_writeWord(0x0B, 0x03);  // Register 0B: Scan-limit -> Display digits 0..3
    spi_writeWord(0x0C, 0x01);  // Register 0C: Shutdown register -> Normal operation
}

// Set display on ('normal operation')
void displayOn() 
{
    spi_writeWord(0x0C, 0x01);
}

// Set display off ('shut down')
void displayOff() 
{
    spi_writeWord(0x0C, 0x00);
}

int main()
{
    // initialize
    DDRB = 0x01;                     // Set PB0 pin as output for display select
    spi_masterInit();                 // Initialize spi module
    displayDriverInit();               // Initialize display chip

    // clear display (all zero's)
    for (char i = 1; i <= 4; i++)
    {
        spi_writeWord(i, 0);         // Write 0 to each digit
    }    
    wait(1000);

    // write 4-digit data  
    for (char i = 1; i <= 4; i++)
    {
        spi_writeWord(i, i);         // Write digit place value to display
        wait(1000);
    }
    wait(1000);

    return (1);
}
