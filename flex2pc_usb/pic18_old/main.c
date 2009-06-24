/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include <timers.h>
#include "system\typedefs.h"                        // Required
#include "system\usb\usb.h"                         // Required
#include "io_cfg.h"                                 // Required

#include "system\usb\usb_compile_time_validation.h" // Optional
#include "user\pickit.h"
#include "user\pk_isr.h"

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
char inbuffer[64];
char outbuffer[64];
char leggi_spi_h;
char leggi_spi_l;
char x_level_h;
char x_level_l;
char y_level_h;
char y_level_l;
char counter_h;
char counter_l;
char Z_level_h;
char Z_level_l;
char adc1_h;
char adc1_l;
char adc2_h;
char adc2_l;
char adc3_h;
char adc3_l;
int status;


/** P R I V A T E  P R O T O T Y P E S ***************************************/
static void InitializeSystem(void);
void USBTasks(void);
void Send_Command(void);
void Write_SPI(unsigned char data_out);

/** V E C T O R  R E M A P P I N G *******************************************/
extern void _startup (void);        // See c018i.c in your C18 compiler dir
#pragma code _RESET_INTERRUPT_VECTOR = 0x002000
void _reset (void)
{
    _asm goto _startup _endasm
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


/******************************************************************************
 * Function:        void main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Main program entry point.
 *
 * Note:            None
 *****************************************************************************/
unsigned char ReadSPI( char read )
{
  SSPBUF = read;                  // initiate bus cycle
  while ( !SSPSTATbits.BF );      // wait until cycle complete
  return ( SSPBUF );              // return with byte read 
}

void Process_SPI_read(char status){

	leggi_spi_h=ReadSPI(status);

	leggi_spi_l=ReadSPI(status);
}

void Process_SPI(char status){

	//while(!INTCONbits.RBIF);
	//while(PORTBbits.RB4 != status);
	//PORTBbits.RB5=PORTBbits.RB4;
	//PORTBbits.RB5=LATBbits.LATB4;
	INTCONbits.RBIF=0;
	leggi_spi_h=ReadSPI(status);

	leggi_spi_l=ReadSPI(status);
}

void Write_SPI(unsigned char data_out){
  SSPBUF = data_out;          // write byte to SSP1BUF register
    while( !SSPSTATbits.BF ); // wait until bus cycle complete 
}

void Send_Command(void){
	WriteSPI(0);	//switch off led buona
	WriteSPI(77);
	
	WriteSPI(0);	//switch off led buona
	WriteSPI(0);
	
	WriteSPI(0);	//switch off led buona
	WriteSPI(0);

	WriteSPI(0);	//switch off led buona
	WriteSPI(0);
}

void process_io(void) 
{
	static unsigned char count = 0;

	if (count++ == 0)
		LATBbits.LATB5 = ! LATBbits.LATB5;
    
//	if (HIDRxReport(inbuffer, 64) > 0) {  // USB receive buffer has data
//		if (inbuffer[0] == 2) {
///			INTCONbits.RBIF = 0;
			while (mHIDTxIsBusy()) ;         // blocking
	
			outbuffer[0] = 0xAA;
			outbuffer[1] = 0xCC;
			outbuffer[2] = 0xDD;
			outbuffer[3] = 0xEE;
			outbuffer[4] = 0xFF;
			outbuffer[5] = count;

			HIDTxReport(outbuffer, 64);
//		}
//	}

	USBDriverService();
}

void ProcessIO(void)
{
	unsigned char usb_idx = 0;		// index of USB buffer
	unsigned int i=0;
	unsigned int max=0;

	//status=1;
    if (HIDRxReport(inbuffer, 64) > 0)  // USB receive buffer has data
	{
	//PORTBbits.RB5=!PORTBbits.RB5;
	//	do
	//	{
			switch(inbuffer[usb_idx])        // parse buffer for commands			
			{
                case 1:
					//WriteSPI(75);	//switch on led
		//			PORTBbits.RB5=1;
				break;
				case 0:
					//WriteSPI(80);	//switch off led
	//				PORTBbits.RB5=0;
				break;
				case 2:
					status=1;
					//Send_Command();

				break;
				case 6:
					WriteSPI(0);	//switch off led buona
					WriteSPI(75);

					WriteSPI(0);	//switch off led buona
					WriteSPI(inbuffer[1] );

					WriteSPI(0);	//switch off led buona
					WriteSPI(inbuffer[2]);

					WriteSPI(0);	//switch off led buona
					WriteSPI(inbuffer[3]);
				break;
				case 3:
					WriteSPI(76);	//switch on led
					Process_SPI(1);
					//y_level=leggi_spi_h;
				break;
				case 4:
					WriteSPI(77);	//switch on led
					Process_SPI(1);
					//z_level=leggi_spi_h;
				break;
				case 5:
					status=0;
					ReadSPI(0);
					ReadSPI(0);
					outbuffer[4]=40;
				break;
				case 7:
					Send_Command();
				break;
				case 8:	// write data to dsPIC
					i=0;
					status=2;
					for(i=0;i<inbuffer[1];i++)
					{
							WriteSPI(inbuffer[i+2]);	
					}
				break;	
				default:					// End of Buffer or unrecognized command
					usb_idx = 64;			// Stop processing.
			} // end switch
	} // end if (HIDRxReport(inbuffer, 64) > 0)
	if(status){
		if(status == 2)
		{
			status = 0;
			goto serv;
		}
		i=0;
		while(PORTBbits.RB4){// && !INTCONbits.RBIF){
		i++;
			if(i>250) {
				Send_Command();
				goto serv;
			}
		}
		
		Process_SPI_read(0);	//dummy
		Process_SPI_read(0);
		outbuffer[62]=leggi_spi_l<<1;
		outbuffer[63]=leggi_spi_h;
		
		max=(outbuffer[62]>>1);
		if(max > 32) {
			Send_Command();
			goto serv;
		}
		for(i=0;i<(max);i++){
			Process_SPI_read(0);
			outbuffer[i<<1]=leggi_spi_l;
			outbuffer[(i<<1)+1]=leggi_spi_h;
		}

		INTCONbits.RBIF=0;
	    // transmit version number 

	}  
serv:
	if(status){
		while(mHIDTxIsBusy()){}         // blocking
	//	PORTBbits.RB5=1;   
	//		outbuffer[0] = 0xAA;
	//		outbuffer[1] = 0xCC;
	//		outbuffer[2] = 0xDD;
	//		outbuffer[3] = 0xEE;
	//		outbuffer[4] = 0xFF;
	   	HIDTxReport(outbuffer, 64);
//		PORTBbits.RB5=0;
	}
	USBDriverService();

} // end void ProcessIO(void)

void main(void)
{
	int i;
    InitializeSystem();
	   for (i=0; i<BUF_SIZE; i++) {    // initialize input and output buffer to 0
        inbuffer[i]=0;
       // outbuffer[i]=0;
    }
	status=0;
	LATB=0;
	PORTB=0;
	ADCON1=0x0F;
	SSPSTAT= 0b00000000;
	SSPCON1= 0b00100001;
	TRISBbits.TRISB1=0;
	TRISBbits.TRISB0=1;

	TRISCbits.TRISC7=0;
	TRISBbits.TRISB5=0;
	TRISBbits.TRISB4=1;
	INTCONbits.RBIF=0;
    while(1) {

        USBTasks();         // USB Tasks
	

//		ProcessIO();
		process_io();
	//	ReadSPI();

    } // end while

} // end main


/******************************************************************************
 * Function:        static void InitializeSystem(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        InitializeSystem is a centralize initialization routine.
 *                  All required USB initialization routines are called from
 *                  here.
 *
 *                  User application initialization routine should also be
 *                  called from here.                  
 *
 * Note:            None
 *****************************************************************************/
static void InitializeSystem(void)
{
    
    #if defined(USE_USB_BUS_SENSE_IO)       // see usbcfg.h
        tris_usb_bus_sense = INPUT_PIN;     // See io_cfg.h
    #endif
    
    #if defined(USE_SELF_POWER_SENSE_IO)    // see usbcfg.h
        tris_self_power = INPUT_PIN;
    #endif
    
    mInitializeUSBDriver();                 // See usbdrv.h
    

	SSPSTAT= 0b00000000;
	SSPCON1= 0b00100010;
	TRISBbits.TRISB1=0;
	TRISBbits.TRISB0=1;

	TRISCbits.TRISC7=0;
	TRISBbits.TRISB5=0;

//	INTCONbits.TMR0IE=1;
//	INTCONbits.PEIE = 1;            // enable Peripheral Interrupt Enable
 //   INTCONbits.GIE = 1;             // enable global interrupts

   // PICkitInit();                           // See pickit.c and pickit.h

} // end InitializeSystem


/******************************************************************************
 * Function:        void USBTasks(void)
 *
 * PreCondition:    InitializeSystem has been called.
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Service loop for USB tasks.
 *
 * Note:            None
 *****************************************************************************/
void USBTasks(void)
{
    /*
     * Servicing Hardware
     */
    USBCheckBusStatus();                    // Must use polling method
//    if(UCFGbits.UTEYE!=1)
        USBDriverService();                 // Interrupt or polling method

//    #if defined(USB_USE_CDC)
//        CDCTxService();
//    #endif

} // end USBTasks

/*****************************************************************************
 * The following is a value stored in upper program memory to let the boot-
 * loader know if an application program is loaded.  If the bootloader does
 * not read this code, it jumps to bootloader mode.
 ****************************************************************************/

//#pragma romdata _bootcode = 0x7FFE
//rom word bootcode = 0x5555;



/** EOF main.c ***************************************************************/
