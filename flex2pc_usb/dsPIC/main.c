#include "ee.h"
#include "cpu/pic30/inc/ee_irqstub.h"
#include <string.h> 

/* Primary (XT, HS, EC) Oscillator without PLL */
_FOSCSEL(FNOSC_PRIPLL);		
/* OSC2 Pin Function: OSC2 is Clock Output 
 * Primary Oscillator Mode: XT Crystanl */
_FOSC(OSCIOFNC_ON & POSCMD_XT);	
/* Watchdog Timer Enabled/disabled by user software */
_FWDT(FWDTEN_OFF);		
/* Disable Code Protection */		
_FGS(GCP_OFF);           	

/******************************************************************************/
/*                         Constant Definitions                               */
/******************************************************************************/
#define ECHO_PERIOD 	100
#define LCD_PERIOD 	100
#define MAX_MESSAGE_SIZE 60

/******************************************************************************/
/*                         Global Variables                                   */
/******************************************************************************/
static unsigned char lcd_pos = 0;
static char lcd_buf[32];
static volatile unsigned char wait_button = 1;
static char in_message[MAX_MESSAGE_SIZE];

/******************************************************************************/
/*                          Function Definitions                              */
/******************************************************************************/
#ifdef __USE_DEMOBOARD__
/* ---------------------------------------------------------------------------- 
|  Put String on LCD:                                                          |
|                                                                              |
|  Access the LCD through the distributed buffer and the lcd_writer meism      |
 ---------------------------------------------------------------------------- */
static void lcd_puts(const char *ch)
{
	GetResource(lcd_mutex);
	while (*ch != '\0') {
		if (lcd_pos == 0) {			
			memcpy(lcd_buf, lcd_buf + 16, 16);
			memset(lcd_buf + 16, ' ', 16);
			lcd_pos = 16;
		}
		if (*ch == '\n') {
			memset(lcd_buf+lcd_pos, ' ', 32 - lcd_pos);
			ch++;
			lcd_pos = 31;
		} else {
			lcd_buf[lcd_pos] = *ch++;
		}
		lcd_pos = (lcd_pos + 1) % 32;
	}
	ReleaseResource(lcd_mutex);	
}
/* ---------------------------------------------------------------------------- 
|  Start System:                                                               |
|                                                                              |
|  Reset the wait_button variable (allow active wait)                          |
 ---------------------------------------------------------------------------- */
static void button_action(void)
{
	if (EE_button_get_S1() == 1 && wait_button == 1) {
		wait_button = 0;
		EE_leds_off();
	}
}
#endif

/* ---------------------------------------------------------------------------- 
|  Setup system timers:                                                        |
|                                                                              |
|  Timer1: used for system ticks (CounterTick)                                 |
 ---------------------------------------------------------------------------- */
static void timer_set(void)
{
	/* TIMER 1  */
	T1CON = 0;		/* Stops the Timer1 and reset control reg */
	TMR1 = 0;		/* Clear contents of the timer register	*/
	PR1 = 0x9C40;		/* Load the Period register -> 1ms @ 40MHz */
	IPC0bits.T1IP = 5;	/* Set Timer1 priority to 5 */
	IFS0bits.T1IF = 0;	/* Clear the Timer1 interrupt status flag */
	IEC0bits.T1IE = 1;	/* Enable Timer1 interrupts */
	T1CONbits.TON = 1;	/* Start Timer1 without prescaler */
}

/******************************************************************************/
/*                                  TASKs                                     */
/******************************************************************************/
/* ---------------------------------------------------------------------------- 
|  LCD Writer                                                                  |
|                                                                              |
|  Manage writing on the LCD                                                   |
 ---------------------------------------------------------------------------- */
TASK(lcd_write)
{
	#ifdef __USE_DEMOBOARD__
	unsigned char i;

	GetResource(lcd_mutex);
	for (i = 0; i < 16; i++)
		EE_lcd_putc(lcd_buf[i]);
	EE_lcd_line2();
	for (i = 16; i < 32;i++)
		EE_lcd_putc(lcd_buf[i]);
	EE_lcd_home();	
	ReleaseResource(lcd_mutex);	
	#endif
}

/* ---------------------------------------------------------------------------- 
|  Brief Description                                                           |
|                                                                              |
|  Detailed Description                                                        |
 ---------------------------------------------------------------------------- */
TASK(echo_usb)
{
	int retv;
	static unsigned char led_status = 1;

	memset(in_message, '\0', MAX_MESSAGE_SIZE);
	retv = EE_usb_read((EE_UINT8*) in_message, MAX_MESSAGE_SIZE);
	if (retv > 0) {
		if (strcmp(in_message, "claudiosalvadori") == 0)
			strcpy(in_message, "gran * comunista");
		retv = EE_usb_write((EE_UINT8*) in_message, retv);
		if (retv <= 0) {
			#ifdef __USE_DEMOBOARD__
			lcd_puts("\nUSB WRITE\nERROR!");
			#endif
		} else {
			#ifdef __USE_DEMOBOARD__
			lcd_puts("\n\nEco=");
			lcd_puts(in_message);
			#endif
			if (led_status) {
				led_status = 0;
				EE_led_off();
			} else {
				led_status = 1;
				EE_led_on();
			}
		} 
	}
}

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/
int main(void)
{
	/* Clock setup for 40 MHz */
	CLKDIVbits.DOZEN   = 0;
	CLKDIVbits.PLLPRE  = 0; 
	CLKDIVbits.PLLPOST = 0; 
	PLLFBDbits.PLLDIV  = 78;
	while (OSCCONbits.LOCK != 1) ;	
	/* Init components */	
	#ifdef __USE_DEMOBOARD__
	EE_demoboard_leds_init();
	EE_buttons_init(&button_action, 0x0F);
	EE_lcd_init();
	EE_lcd_clear();
	#else
	EE_led_init();
	EE_led_off();
	#endif 
	timer_set();		
	/* Start the system */
	#ifdef __USE_DEMOBOARD__
	SetRelAlarm(lcd_write_alarm, 1000, LCD_PERIOD); /* Start LCD writer */
	EE_leds_on();
	lcd_puts("\nPress 1 to\ninit");
	wait_button = 1;
	while (wait_button) ;
	EE_usb_init();	/* Init USB*/
	lcd_puts("\nUSB OK, press\n1 to start");
	wait_button = 1;
	while (wait_button) ;
	#else
	EE_usb_init();	/* Init USB*/
	EE_led_on();
	#endif
	SetRelAlarm(echo_usb_alarm, 1000, ECHO_PERIOD); /* Start echo task */
	/* Forever loop: background activities (if any) should go here */
	for (;;) ;
	return 0;
}

/******************************************************************************/
/*                                     ISRs                                   */
/******************************************************************************/
/* ---------------------------------------------------------------------------- 
|  ISR2 Timer1:                                                                |
|                                                                              |
|  Calls CounterTick for task_counter                                          |
 ---------------------------------------------------------------------------- */
ISR2(_T1Interrupt)
{
	IFS0bits.T1IF = 0;
	CounterTick(task_counter);
}

/******************************************************************************/
/*                                    TRAPs                                   */
/******************************************************************************/
/*
void _ISRFAST _OscillatorFail(void) { for(;;); }

void _ISRFAST _AddressError(void) { for(;;); }

void _ISRFAST _StackError(void) { for(;;); }

void _ISRFAST _MathError(void) 	{ for(;;); }

void _ISRFAST _DMACError(void) { for(;;); }
*/
