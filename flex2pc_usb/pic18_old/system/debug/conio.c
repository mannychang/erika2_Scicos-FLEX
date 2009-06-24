/*----------------------------------------------------------------------
** CONIO.H
** 
** 	Serial Port support (USART1) with harware handshake
**	Console utilities for VT100 terminals [HiperTerminal]
**	38k@16MHz or 115k@20MHz 
** language: MPLAB C18
** revision:
** 1.00     02/14/03    Lucio Di Jasio
** 2.00     05/04/03    LDJ BOOT button 
*/

#include "p18cxxx.h"
#include "ctype.h"
#include "system\usb\usb.h"

#define B38K16MHZ		25	// = 38K baud @16MHz
#define B115K20MHZ		10  // = 115.2K baud @20MHz
#define B115K26MHZ      0x0C

#define KEY_BACK        0x8    // ascii code for backspace

#define SPBRG1 SPBRG
#define TXSTA1 TXSTA
#define RCSTA1 RCSTA
#define TXREG1 TXREG
#define TXSTA1bits TXSTAbits
//----------------------------------------------------------------------
// UART init
//

//#if debug
void UARTinit( char b)
// b    baud rate generator period,use B38K16MHZ or B115K20MHZ
{
    TRISCbits.TRISC7=1;
    TRISCbits.TRISC6=0;
    //SPBRG1 = b;         // set the baud rate
    SPBRG = 0x71;
    SPBRGH = 0x02;      // 0x0271 for 48MHz -> 19200 baud
    TXSTA1 = 0x24;      // TX enable BRGH=1
    RCSTA1 = 0x90;      // continuos RX, SPI enabled
    BAUDCON = 0x08;                     // BRG16 = 1
} // UARTinit
    

//----------------------------------------------------------------------
// putc     send a character to the console
//

char putc( char data)
// data     ascii character to send to console
{			
			while ( !TXSTA1bits.TRMT);  // wait for the transmit register to be empty
			return TXREG1 = data;       // Write the data byte to the USART1
} // putc			

//----------------------------------------------------------------------
// getsn    receive a string of n char max
//

void putrs( const  rom char *s)
{
	while(*s != '\0' )
		putc(*s++ );
} //putrs


void puts( char *s)
{
	while(*s != '\0' )
		putc(*s++ );
} //puts

char _string[8]; // temp string 
const rom char _hex[] = "0123456789ABCDEF";
char * btox( unsigned char x, char *p)
{
    *p = _hex[x>>4];
    *(p+1) = _hex[x & 0xf];
    *(p+2) = 0;
    return p;
} // btox

int xtoi( char *s)
{
    char j=0, i;
    int x=0;
    
    while ((*s != 0) && ( *s == ' '))
        s++;            // absorbe spaces
    do{
        for (i =0; i<16; i++)
            if ( _hex[i]==toupper(s[j])) break;
        if (i==16) 
            return x;
        else 
            x = (x<<4)+i;
    } while ((++j)< 4);
    return x;
} // xtoi

long xtol( char *s)
{
    char j=0, i;
    long x=0;
    
    while ((*s != 0) && ( *s == ' '))
        s++;            // absorbe spaces
    do{
        for (i =0; i<16; i++)
            if ( _hex[i]==toupper(s[j])) break;
        if (i==16) 
            return x;
        else 
            x = (x<<4)+i;
    } while ((++j)< 8);
    return x;
} // xtol

//#endif // #if debug

#define clrscr putrs( "\x1b[2J")
#define pcr()   putc( '\r');putc('\n');
#define putx( xb)  puts( btox( xb, _string))
#define putxi( xi) putx( (unsigned char)(xi>>8)); putx( (unsigned char) xi)
#define prints( s, r)  putrs(s); puts( r)
#define printb( s, b)  putrs(s); puts( btoa( b, _string))
#define printi( s, i)  putrs(s); puts( itoa( i, _string))
#define printl( s, l)  putrs(s); puts( ltoa( l, _string))
#define printxb( s, xb) putrs(s); putx( xb)
#define printxi( s, xi) putrs(s); putxi( xi)
#define printxl( s, xl) putrs(s); putxi( (xl>>16)); putxi( xl);
