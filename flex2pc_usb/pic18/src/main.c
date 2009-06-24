/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include <timers.h>
#include "system\typedefs.h"                        // Required
#include "system\usb\usb.h"                         // Required
#include "io_cfg.h"                                 // Required

#include "system\usb\usb_compile_time_validation.h" // Optional
#include "user\pickit.h"
#include "user\pk_isr.h"
#include "flex_usb_frame.h"

#define USB_TX_BUFFER_SIZE 1
#define USB_RX_BUFFER_SIZE 1
#define USB_FLEX_LED LATBbits.LATB5
#define USB_DSPIC_REQ PORTBbits.RB4

struct flex_usb_stat_t {
	unsigned rx_buffer_policy : 2; 
	unsigned rx_acked : 1; 
	unsigned rx_fragment : 1; 
	unsigned connected : 1;
	unsigned to_send_usb : 1;
	unsigned to_send_spi : 1;
	unsigned reserved : 1;
};

#pragma config PLLDIV = 5    /* Sets the oscillator mode to HS */
#pragma config CPUDIV = OSC1_PLL2
#pragma config USBDIV = 2
#pragma config FOSC = HSPLL_HS
#pragma config IESO = OFF
#pragma config PWRT = OFF
#pragma config BOR = OFF
#pragma config BORV = 3
#pragma config VREGEN = ON
#pragma config WDT = OFF   /* Turns the watchdog timer off */
#pragma config WDTPS = 32768
#pragma config CCP2MX = ON
#pragma config PBADEN = OFF
#pragma config LPT1OSC = OFF
#pragma config MCLRE = ON
#pragma config STVREN = ON
#pragma config LVP = OFF   /* Turns low voltage programming off */
#pragma config XINST = OFF
#pragma config DEBUG = OFF /* Compiles without extra debug code */

/** V A R I A B L E S ********************************************************/
#pragma udata
struct flex_usb_packet_t usb_rx_buffer[USB_RX_BUFFER_SIZE];
struct flex_usb_packet_t usb_tx_buffer[USB_TX_BUFFER_SIZE];
struct flex_usb_stat_t global_stat = {0, 0, 0, 0, 0, 0, 0};

/** V E C T O R  R E M A P P I N G *******************************************/
extern void _startup (void);        // See c018i.c in your C18 compiler dir
#pragma code _RESET_INTERRUPT_VECTOR = 0x002000
void _reset (void)
{
	_asm 
	goto _startup 
	_endasm
}

#pragma code _HIGH_INTERRUPT_VECTOR = 0x002008
void _high_ISR (void)
{
	_asm
	goto InterruptHandler       // jump to interrupt routine
	_endasm
}

#pragma code InterruptVectorLow = 0x002018
void InterruptVectorLow (void)
{
	_asm
	goto InterruptHandler       // jump to interrupt routine
	_endasm
}

/** D E C L A R A T I O N S **************************************************/
#pragma code

/** 
* @brief Perform an SPI cycle (Read and Write)
*
* This function perform an SPI cycle in order to write and read data.
* This is called by the SPI master and issues a SPI clock that allow to send
* and receive a byte to/from the slave.
* 
* The operation is executed in polling mode, the slave select line is not used. 
*
* If no data need to be sent then a dummy byte can be written and should be 
* ignored by the slave. 
* If no data need to be received then the return value of this function could 
* be ignored. 
*
* \todo Use a counter variable to limit the polling cycle on the 
*	\c PIR1bits.SSPIF, that currently could be infinite. In that case the
*	return value should be used for error notification (exit status) and
*	the read value should be set in a output parameter of the function.
* 
* @param[in] write 	The byte to be written.
* 
* @return 		The returned read byte.
*
* @pre			The SPI configutation register should be proberly set
*			to operate in master mode, 
*			e.g. <c>SSPCON1 = 0b00100001; SSPSTAT = 0b00000000;</c>
*/
static uint8_t spi_cycle(uint8_t write)
{
	uint8_t read;

	PIR1bits.SSPIF = 0;
	read = SSPBUF;			/* Dummy: errata sylicon work around */
	SSPBUF = write;			/* Write: initiate bus cycle */
	//while (SSPSTATbits.BF == 0) ;	/* Wait until cycle is complete */
	while (PIR1bits.SSPIF == 0) ;	/* Wait until cycle is complete */
	read = SSPBUF;			/* Read: read the SPI buffer */
	return read;
}

/** 
* @brief Implement the Flex USB bridge
*
* This function is the high level bridge between the dsPIC and the PC, 
* performing the convertion of protocol from the SPI bus (dsPIC-PIC18) to the
* USB one (PIC18-PC).
*
* The USB interface exported to the PC is a HID USB class used in report mode,
* i.e. interrupt mode is not used.
* The transmission is packet oriented. Each packet is 64 bytes large and has
* the format described by \ref flex_usb_packet_t. The packet are sent according
* to one of the following paradigms:
* - unacknowledged: 	the packet are transmitted in the simplest way, without 
*			any acknowledgment mechanism implemented at this level;
* - acknowledged:	for each packet sent/received an ack is received/sent;
* - fragmented:		fragmentation is performed within this transport level
*			using the acknowledgment mechanism. 
* 
* This function perform two main activities: it uses the SPI bus for to 
* exchange data with the dsPIC and then uses the USB HID class implemented by
* the Microchip USB stack to exchange data with the PC.
* 
* \todo Things to be done:
*	- The acknowledged and fragmented are NOT implemented
*	- The packets exchaged with the dsPIC have the same format of the one
*	  used with the PC communication (the PIC18 simply check the CRC and
*	  and forward the packets).
*	- Hot unplugging is not managed at all!
*	- Some polling \c while may generate infinite wait, need to be changed
*	  in a safe version (using counter to limit the loops).
*	- Use the led in a different manner?
*	- Create a mainpage for doxygen and move the protocol description there
*
* @pre	The SPI port should be properly initialized as well as all the I/O pin
* 	used. The functions \c USBCheckBusStatus() and \c USBDriverService() 
*	MUST be called before any call to this function.
*
* @post	The function \c USBDriverService() MUST be called before any call 
*	to this function.
*/
static void flex_usb_process(void) 
{
	uint16_t cmd, i;
	uint8_t sum;

	if (global_stat.to_send_spi && !USB_DSPIC_REQ) { /* Only write */
		for (i = 0; i < 64; i++) 
			 spi_cycle(((uint8_t*) usb_rx_buffer)[i]);
		global_stat.to_send_spi = 0;
	} else if (!global_stat.to_send_spi && USB_DSPIC_REQ) { /* Only read */
		for (i = 0; i < 64; i++) 
			((uint8_t*) usb_tx_buffer)[i] = spi_cycle(0);
		//for (sum = 0, i = 0; i < 63; i++)
		for (sum = 0, i = 1; i < 64; i++) // COMPLIANT TO "HEADER" CRC
			sum += ((uint8_t *)usb_tx_buffer)[i];
		if (usb_tx_buffer[0].crc != sum) { 
			/* DO NOTHING! */
			global_stat.to_send_usb = 0;
		} else {
			global_stat.to_send_usb = 1;
		}
	} else if (global_stat.to_send_spi && USB_DSPIC_REQ) { /* Both wr rd */
		for (i = 0; i < 64; i++) 
			((uint8_t*) usb_tx_buffer)[i] = 
					spi_cycle(((uint8_t*)usb_rx_buffer)[i]);
		//for (sum = 0, i = 0; i < 63; i++)
		for (sum = 0, i = 1; i < 64; i++) // COMPLIANT TO "HEADER" CRC
			sum += ((uint8_t *)usb_tx_buffer)[i];
		if (usb_tx_buffer[0].crc != sum) { 
			/* DO NOTHING! */
			global_stat.to_send_usb = 0;
		} else {
			global_stat.to_send_usb = 1;
		}
		global_stat.to_send_spi = 0;
	}
	/* chris: FIXME: should  be: retvalue == 64!! */
	if (HIDRxReport(usb_rx_buffer, sizeof(struct flex_usb_packet_t)) > 0) {
		/* USB rx buffer has data: parse the header */
		//for (sum = 0, i = 0; i < 63; i++)
		for (sum = 0, i = 1; i < 64; i++) // COMPLIANT TO "HEADER" CRC
			sum += ((uint8_t *)usb_rx_buffer)[i];
		if (sum != usb_rx_buffer[0].crc || 
		    usb_rx_buffer[0].start !=  FLEX_USB_HEADER_START) {
			global_stat.connected = 0;
			return; //chris: TODO: scegliere una politca!!
		}
		switch (usb_rx_buffer[0].type) {
		case FLEX_USB_HEADER_DATA:
			global_stat.to_send_spi = 1;
			/* TODO*/
			return;
			break;
		case FLEX_USB_HEADER_ACK:
			/* TODO*/
			return;
			break;
		case FLEX_USB_HEADER_COMMAND:
			cmd = usb_rx_buffer[0].payload[1];
			cmd += ((uint16_t) usb_rx_buffer[0].payload[0]) << 8;
			switch (cmd) {
			case FLEX_USB_COMMAND_CONNECT:
				USB_FLEX_LED = 1;
				global_stat.connected = 1;
				break;
			case FLEX_USB_COMMAND_DISCONNECT:
				USB_FLEX_LED = 0;
				global_stat.connected = 0;
				break;
			default:
			break;
			}
			break;
		default:
		break;
		}
	}
	if (global_stat.connected && global_stat.to_send_usb) {
		while (mHIDTxIsBusy()) ;         /* blocking */
	   	HIDTxReport(usb_tx_buffer, sizeof(struct flex_usb_packet_t));
		USB_FLEX_LED = ~USB_FLEX_LED; 
		global_stat.to_send_usb = 0;
	}
}

void main(void)
{
	#if defined(USE_USB_BUS_SENSE_IO)       /* see usbcfg.h */
	tris_usb_bus_sense = INPUT_PIN;		/* See io_cfg.h */
	#endif
	#if defined(USE_SELF_POWER_SENSE_IO)    /* see usbcfg.h */
	tris_self_power = INPUT_PIN;
	#endif
	mInitializeUSBDriver();                 /* See usbdrv.h */

	SSPSTAT = 0b00000000;
	SSPCON1 = 0b00100010;
	TRISCbits.TRISC7 = 0; 	/* SDO[out]: SPI output. */
	TRISBbits.TRISB0 = 1; 	/* chris: ??[in]: ??? */
	TRISBbits.TRISB1 = 0; 	/* SCK[out]: SPI clock controlled by master. */
	TRISBbits.TRISB4 = 1;	/* USB_BUSY[in]: Used by dsPIC to notify USB. */
	TRISBbits.TRISB5 = 0;	/* USB_LED[out]: Flex pic18 led. */
	//TRISAbits.TRISA5 = 0;	/* /SS[out]: SPI Slave Select. */
	LATB = 0;
	PORTB = 0;
	/* chris: FIXME; a che serve????*/
	ADCON1 = 0x0F;
	/* chris: FIXME:  questo sovrascrive il valore precedente!!!*/
	SSPCON1 = 0b00100001;
	USB_FLEX_LED = 0;
	INTCONbits.RBIF = 0;

	memset(usb_rx_buffer, 0x00, 
	       USB_RX_BUFFER_SIZE * sizeof(struct flex_usb_packet_t));	
	memset(usb_tx_buffer, 0x00, 
	       USB_TX_BUFFER_SIZE * sizeof(struct flex_usb_packet_t));	
	
	/* Master loop */
	while (1) {
		/* Call USB Functions */
		USBCheckBusStatus();	/* Must use polling method */
		USBDriverService();	/* Interrupt or polling method */
		/* Call Application Layer */
		flex_usb_process();
		/* Call USB Functions */
		USBDriverService();
	} 
} 

