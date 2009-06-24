/******************************************************************************
 * Console Module Header
 * AMAD Applications
 *****************************************************************************/

#ifndef CONIO_H
#define	CONIO_H

#include "system\typedefs.h"

//Definitions
#define B115K48MHZ      103        // = 115.2K baud @48MHz

//Structures

//Externs
extern char _string[8];


//Prototypes
void TXInit(byte baud);
void TXbyte(byte data);
void StringDisplay(rom char *pString);
void DisplayHex(byte data);
byte BinaryToASCII(byte data);

void UARTinit(char b);
char putc(char data);
char getc(void);
char getsn(char *s,char n);
void putrs(const rom char *s);
void puts(char *s);
char * btox(unsigned char x, char *p);


//Macros
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

#endif //end CONIO_H
