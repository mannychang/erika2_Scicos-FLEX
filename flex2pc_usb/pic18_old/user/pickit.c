/*********************************************************************
 *
 *                  Microchip PICkit 2 v2 Flash Starter Kit
 *
 *********************************************************************
 * FileName:        pickit.c
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 3.00
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PICmicro® Microcontroller is intended and
 * supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Steven Bible      2004-11-04     Initial Write
 * Steven Bible      2005-04-20     Version 1 Released
 * Walter Kicinski   2006-May-15    Rewrite for v2
 ********************************************************************
 * Change log
 * v2.01 - WK - 
 *       initial v2 release
 * v2.02 - WK - May 2007
 *       Ensures BOR bits in CONFIG2L are disabled at reset
 * v2.10 - WK - Jun 2007
 *       Added ShiftBitsOutICSPHold(), and new script commands
 *          WRITE_BITS_LIT_HLD, WRITE_BITS_BUF_HLD
 *       Added new script commands to support Serial EEPROM devices:
 *          SET_AUX, AUX_STATE_BUFFER, I2C_START, I2C_STOP,
 *          I2C_WR_BYTE_LIT, I2C_WR_BYTE_BUF, I2C_RD_BYTE_ACK, 
 *          I2C_RD_BYTE_NACK, SPI_WR_BYTE_LIT, SPI_WR_BYTE_BUF,
 *          SPI_RD_BYTE_BUF
 *       Added new commands for calibration and internal EEPROM
 *          SET_VOLTAGE_CALS, WR_INTERNAL_EE, RD_INTERNAL_EE
 *       Added Serial Mode support and commands
 *          ENTER_UART_MODE, EXIT_UART_MODE
 ********************************************************************/

/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include "delays.h"
#include "system\typedefs.h"
#include "system\usb\usb.h"

#include "io_cfg.h"                 // I/O pin mapping
#include "user\pickit.h"
#include "user\pk_isr.h"

/** V A R I A B L E S ********************************************************/
#pragma udata	ScriptBuffer					// in banks 5,6, & 7.
// NOTE: must use SECTION command in linker script.
unsigned char uc_script_buffer[SCRIPTBUF_SIZE];		// Script Buffer

#pragma udata	USB_Buffers
char inbuffer[BUF_SIZE];            				// input to USB device buffer
char outbuffer[BUF_SIZE];            				// output to USB device buffer

#pragma udata	DownloadBuffer
unsigned char 	uc_download_buffer[DOWNLOAD_SIZE];	// Download Data Buffer

#pragma udata	UploadBuffer
unsigned char 	uc_upload_buffer[UPLOAD_SIZE];		// Upload Data Buffer

#define	INLINE_ASM_RAM 0x05C
#pragma udata access	AssemblyAccess = INLINE_ASM_RAM
near unsigned char asm_temp1;
near unsigned char asm_temp2;
near unsigned char asm_temp3;
near unsigned char asm_temp4;
#define ASM_TEMP1_RAM 	INLINE_ASM_RAM
#define ASM_TEMP2_RAM 	(INLINE_ASM_RAM+1)
#define ASM_TEMP3_RAM 	(INLINE_ASM_RAM+2)
#define ASM_TEMP4_RAM 	(INLINE_ASM_RAM+3)

#pragma udata	Misc
unsigned char 	*uc_ScriptBuf_ptr = &uc_script_buffer[0];

struct {						// Script table - keeps track of scripts in the Script Buffer.
	unsigned char	Length;
	int	StartIndex;	// offset from uc_script_buffer[0] of beginning of script.
} ScriptTable[SCRIPT_ENTRIES];

unsigned char icsp_pins;
unsigned char icsp_baud;
unsigned char aux_pin;

struct {
    unsigned char	write_index;        // buffer write index
    unsigned char	read_index;			// buffer read index
    unsigned int    used_bytes;         // # bytes in buffer
} downloadbuf_mgmt; 

struct {
    unsigned char	write_index;		// buffer write index
    unsigned char	read_index;			// buffer read index
    unsigned int    used_bytes;         // # bytes in buffer
} uploadbuf_mgmt; 

union {		// Status bits
	struct {
		unsigned char	StatusLow;
		unsigned char	StatusHigh;
	};
	struct{
		// StatusLow
		unsigned VddGNDOn:1;	// bit 0
		unsigned VddOn:1;
		unsigned VppGNDOn:1;
		unsigned VppOn:1;
		unsigned VddError:1;
		unsigned VppError:1;
        unsigned ButtonPressed:1;
		unsigned :1;
		//StatusHigh
        unsigned Reset:1;       // bit 0
		unsigned UARTMode:1;			
        unsigned ICDTimeOut:1;
		unsigned UpLoadFull:1;
		unsigned DownloadEmpty:1;
		unsigned EmptyScript:1;
		unsigned ScriptBufOvrFlow:1;
		unsigned DownloadOvrFlow:1;
	};
} Pk2Status;

struct {
	unsigned char   VddThreshold;	// error detect threshold
	unsigned char   VppThreshold;	// error detect threshold
    unsigned char   VddErrCount;
    unsigned char   VppErrCount;
    unsigned int    BlinkClount;    // counter for blinking Busy on VDD/VPP error
} VddVppLevels;

struct {
	unsigned char CCPRSetPoint;
	unsigned char UppperLimit;
	unsigned char LowerLimit;	
} Vpp_PWM;

struct {
    unsigned int    adc_calfactor;      // CalibratedResult = (ADRES * adc_calfactor) >> 8
    signed char	    vdd_offset;
    unsigned char	vdd_calfactor;		// Calibrated CCP value = (((CCP >> 6) + vdd_offset) * vdd_calfactor) >> 7
} VoltageCalibration; 

#pragma udata access	MiscAccess
near struct {
    unsigned char   TimerBaudLoadL;      // timer value to load for particular baud rate
    unsigned char   TimerBaudLoadH;
    unsigned char   RXbits;             // # bits received during byte reception
    unsigned char   RXbyte;             // Byte value being received
    unsigned char   TXbits;             // # bits send during byte transmission
    unsigned char   TXByte;             // Byte value being sent
    unsigned char   LastRXByte;         // Received byte to be saved
    unsigned char   LastRXByte2;         // Received byte to be saved
    unsigned char   LastRXByte3;         // Received byte to be saved
    unsigned char   NewRX;              // flag for new data in LastRXByte
} UARTStatus;
		

/** P R I V A T E  P R O T O T Y P E S ***************************************/
void EnterBootloader(void);
void SendFWVersionUSB(void);
void ClearScriptTable(void);
void StoreScriptInBuffer(unsigned char *usbindex);
void ScriptEngine(unsigned char *scriptstart_ptr, unsigned char scriptlength);
void SetICSP_PinStates(unsigned char icsp_byte);
unsigned char GetICSP_PinStates(void);
void SetAUX_PinState(unsigned char aux_byte);
unsigned char GetAUX_PinState(void);
void ShiftBitsOutICSP(unsigned char outputbyte, char numbits);
void ShiftBitsOutICSPHold(unsigned char outputbyte, char numbits);
unsigned char ReadDownloadBuffer(void);
void WriteDownloadDataBuffer(unsigned char *usbindex);
void WriteUploadBuffer(unsigned char byte2write);
unsigned char ShiftBitsInICSP(unsigned char numbits);
void RunScript(unsigned char scriptnumber, unsigned char repeat);
void ShortDelay(unsigned char count);
void LongDelay(unsigned char count);
void ReadUploadDataBuffer(void);
void ReadUploadDataBufferNoLength(void);
void SendStatusUSB(void);
void SendVddVppUSB(void);
void ADCConvert(unsigned char channel);
unsigned char ICDSlave_Receive (void);
void ICDSlave_transmit (unsigned char TransmitByte);
void SendScriptChecksumsUSB(void);
unsigned char ShiftBitsInPIC24(unsigned char numbits);
void WriteByteDownloadBuffer(unsigned char DataByte);
void EnsureBOROff(void);

void I2C_Start(void);
void I2C_Stop(void);
void I2C_Write(unsigned char outputbyte);
unsigned char I2C_Read(unsigned char giveack);
unsigned char SPI_ReadWrite(unsigned char outputbyte);

void SaveCalFactorsToEE(void);
void ReadCalFactorsFromEE(void);
void EE_WriteByte(unsigned char byte_address, unsigned char write_byte);
unsigned char EE_ReadByte(unsigned char byte_address);
unsigned char CalThresholdByte(unsigned char rawValue);
unsigned int CalADCWord(unsigned int rawValue);
void CalAndSetCCP1(unsigned char ccp1_upper, unsigned char ccp1_lower);
void WriteInternalEEPROM(unsigned char *usbindex);
void ReadInternalEEPROM(unsigned char *usbindex);

void UARTModeService(void);
void EnterUARTMode(unsigned char *usbindex);
void ExitUARTMode(void);

/** D E C L A R A T I O N S **************************************************/

#pragma code


/******************************************************************************
 * Function:        void PickitInit(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        
 *
 * Note:            None
 *****************************************************************************/
void PICkitInit(void)
{
    byte i;                         // index variable

    // set up LVD
    HLVDCON = 0x03;
    HLVDCONbits.HLVDEN;
    PIR2bits.HLVDIF = 0;
    PIE2bits.HLVDIE = 1;
/*
    //-------------------------
    // initialize I/O (see io_cfg.h)
    //-------------------------

    // ------
    // Port A
    // ------

    TRISAbits.TRISA0 = 1;           // RA0/AN0 Analog Input (Vpp_FEEDBACK)

    TRISAbits.TRISA1 = 1;           // RA1/AN1 Analog Input (Vdd_TGT_FB)

    ADCON1 = 0x0D;                  // RA0/AN0 and RA1/AN1 are analog input
    ADCON2 = 0x03;                  // left justified, 0 Tad, Frc

    tris_ICSPDAT = 1;               // RA2 Input (tristate)
    ICSPDAT_in = 0;                 // initialize output port to 0 (low)
    ICSPDAT_out = 0;                // initialize input latch to 0 (low)

    tris_ICSPCLK = 1;               // RA3 Input (tristate)
    ICSPCLK_in = 0;                 // initialize output port to 0 (low)
    ICSPCLK_out = 0;                // initialize input latch to 0 (low)

    tris_AUX = 1;                   // RA4 Input (tristate)
    AUX = 0;                        // initialize output latch to 0 (low)

    MCLR_TGT = 0;                   // initialize MCLR_TGT off (no MCLR_TGT)
    tris_MCLR_TGT = 0;              // RA5 Output

    // ------
    // Port B
    // ------

    // (future) need to initialize I2C for RB0 (SDA) and RB1 (SCL)

    // *************************************************************
    // * WARNING * WARNING * WARNING * WARNING * WARNING * WARNING * 
    // * Do not turn on both half-bridge gates at the same time.   *
    // * It will cause a direct short of +V_TGT to GROUND.         *
    // *************************************************************

    Vdd_TGT_N = 0;                  // initialize half-bridge N-gate off
    tris_Vdd_TGT_N = 0;             // Output

    Vdd_TGT_P = 1;                  // initialize half-bridge P-gate off
    tris_Vdd_TGT_P = 0;             // Output

    tris_PROG_SWITCH = 1;           // Input
    PROG_SWITCH_pin = 1;            // initialize port to 1
    INTCON2bits.NOT_RBPU = 0;       // initialize PORTB pull-ups on

    Vpp_ON = 0;                     // initialize Vpp_ON to off
    tris_Vpp_ON = 0;                // Output

    // ------
    // Port C
    // ------

    BUSY_LED = 0;                   // initialize Busy LED to off
    tris_BUSY_LED = 0;              // Output

    Vpp_PUMP = 0;                   // initialize latch to 0 (low)
    tris_Vpp_PUMP = 0;              // RC1 Output (CCP2)

    Vdd_TGT_ADJ = 0;                // initialize latch to 0 (low)
    tris_Vdd_TGT_ADJ = 0;           // RC2 Output (CCP1)

    tris_WP = 0;                    // Output
    WP = 1;                         // initialize WP to protect (1)

    //-------------------------
    // initialize variables
    //-------------------------

	Pk2Status.StatusLow = 0;		// init status.
	Pk2Status.StatusHigh = 1;       // set reset bit.

	icsp_pins = 0x03;		// default inputs
	icsp_baud = 0x00;		// default fastest
    aux_pin = 0x01;         // default input*/

    for (i=0; i<BUF_SIZE; i++) {    // initialize input and output buffer to 0
        inbuffer[i]=0;
        outbuffer[i]=0;
    }

	ClearScriptTable();     // init script table to empty.

    downloadbuf_mgmt.write_index = 0;   // init buffer to empty
    downloadbuf_mgmt.read_index = 0;
    downloadbuf_mgmt.used_bytes = 0;

    uploadbuf_mgmt.write_index = 0;     // init buffer to empty
    uploadbuf_mgmt.read_index = 0;
    uploadbuf_mgmt.used_bytes = 0; 

    ReadCalFactorsFromEE();
    if ((VoltageCalibration.adc_calfactor > 0x140) || (VoltageCalibration.adc_calfactor < 0xC0)
          || (VoltageCalibration.vdd_calfactor > 0xAF) || (VoltageCalibration.vdd_calfactor < 0x50))
    { // bad cals
        VoltageCalibration.adc_calfactor = 0x0100;  // Set default values. 
        VoltageCalibration.vdd_offset = 0x00;
        VoltageCalibration.vdd_calfactor = 0x80;
    }

    // configure Timer0 (used by SCRIPT_ENGINE)

    T0CON = 0x08;                   // Timer0 off, 16-bit timer, 
                                    // internal clock, prescaler not assigned

    // Timer1 is used by VddVpp voltage monitoring interrupt or UART RX.

    // configure Timer2 (used by CCP1 and CCP2)

    T2CON = 0x04;                   // 1:1 Postscale, 1:1 Prescale, Timer2 on
    PR2 = PWM_150kHz;               // Timer2 Period Register (sets PWM Period for CCP1 and 2)

    // Timer3 is used for UART TX.

    // initialize CCP1 for Vdd_TGT_ADJ
    // (Fosc = 48 MHz, Postscale = 1:1, Timer2 Prescale = 1:1)

    CCP1CON = 0x0C;                 // PWM mode
    CCPR1L = Vdd_3V3;               // PWM duty cycle- start at 3.3v so we don't blow PIC 18J & 24 WEK

	// initialize CCP2 for VPP WEK
	Vpp_PWM.CCPRSetPoint = 64;
	Vpp_PWM.UppperLimit = 62;		// default to 3.3v so we don't blow PIC 18J & 24
	Vpp_PWM.LowerLimit = 60;
	VddVppLevels.VddThreshold = 126;	// about 2.5 v
	VddVppLevels.VppThreshold = 45;		// about 2.5 v

    // initialize Comparator and Voltage Reference Modules

    CMCON = 0x07;                   // Comparators off
    CVRCON = 0x00;                  // VR off


    // Check BOR bits in CONFIG1L
    EnsureBOROff();

	// Set up Interrupts
    INTCONbits.PEIE = 1;            // enable Peripheral Interrupt Enable
    INTCONbits.GIE = 1;             // enable global interrupts

	// Start voltage monitoring
    VppVddADCTmr1_Start();

}

/******************************************************************************
 * Function:        void ProcessIO(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        
 *
 * Note:            None
 *****************************************************************************/
void ProcessIO(void)
{
	unsigned char usb_idx = 0;		// index of USB buffer

    // poll programming switch
    if (!PROG_SWITCH_pin)   // active low
        Pk2Status.ButtonPressed = 1;

    // UART Mode service
    if (Pk2Status.UARTMode)
    {
        UARTModeService();
    }

    if (HIDRxReport(inbuffer, 64) > 0)  // USB receive buffer has data
	{
		do
		{
			switch(inbuffer[usb_idx])        // parse buffer for commands			
			{
                case NO_OPERATION:          // Do nothing
                    // format:      0x5A
                    usb_idx++;
                    break;

                case GETVERSION:			// Get firmware version
					// format: 		0x76
					// response:	<major><minor><dot>
					SendFWVersionUSB();
                    usb_idx++;
                    break;

                case BOOTMODE:				// Enter Bootloader mode
					// format: 		0x42
					// response:	-
					EnterBootloader();
                    break;

                case ENTER_UART_MODE:       // Puts the firmware in UART Mode
                    // format:      0xB3 <BaudValueL><BaudValueH>
                    //                   BaudValue = 65536 – [((1/BAUD) – 3e-6) / 1.67e-7]
					// response:	-
                    usb_idx++;
                    EnterUARTMode(&usb_idx);
                    break; 

                case EXIT_UART_MODE:        // Exits the firmware from UART Mode
                    // format:      0xB4
					// response:	-
                    usb_idx++;
                    ExitUARTMode();
                    break; 

                case WR_INTERNAL_EE:        // write bytes to PIC18F2550 EEPROM
                    // format:      0xB1 <address><datalength><data1><data2>....<dataN>
                    //                   N = 32 Max
					// response:	-
                    usb_idx++;
                    WriteInternalEEPROM(&usb_idx);
                    break; 

                case RD_INTERNAL_EE:        // read bytes from PIC18F2550 EEPROM
                    // format:      0xB2 <address><datalength>
                    //                   N = 32 Max
					// response:	<data1><data2>....<dataN>
                    usb_idx++;
                    ReadInternalEEPROM(&usb_idx);
                    break; 

                case SETVDD:
                    // format:      0xA0 <CCPL><CCPH><VDDLim>
                    //      CCPH:CCPL = ((Vdd * 32) + 10.5) << 6     where Vdd is desired voltage
                    //      VDDLim = (Vfault / 5) * 255              where Vdd < VFault is error
                    // response:    -
                    CalAndSetCCP1(inbuffer[usb_idx+2], inbuffer[usb_idx+1]);
                    VddVppLevels.VddThreshold = CalThresholdByte(inbuffer[usb_idx+3]);          // Set error threshold
                    usb_idx += 4;
                    break;                 

                case SETVPP:
                    // format:      0xA1 <CCPR2L><VPPADC><VPPLim>
                    //      CCPR2L = duty cycle.  Generally = 0x40
                    //      VPPADC = Vpp * 18.61        where Vpp is desired voltage.
                    //      VPPlim = Vfault * 18.61              where Vdd < VFault is error
                    // response:    -
					Vpp_PWM.CCPRSetPoint = inbuffer[usb_idx+1];
					Vpp_PWM.UppperLimit = CalThresholdByte(inbuffer[usb_idx+2])+1;
					Vpp_PWM.LowerLimit = Vpp_PWM.UppperLimit - 2;   
                    VddVppLevels.VppThreshold = CalThresholdByte(inbuffer[usb_idx+3]);
                    usb_idx += 4;
                    break;  

                case SET_VOLTAGE_CALS:
                    // format:      0xB0 <adc_calfactorL><adc_calfactorH><vdd_offset><calfactor>
                    //      CCPH:CCPL = (((CCP >> 6) + vdd_offset) * vdd_calfactor) >> 7
                    //      CalibratedResult = (ADRES * adc_calfactor) >> 8
                    // response:    -
					VoltageCalibration.adc_calfactor = (inbuffer[usb_idx+1] & 0xFF);
					VoltageCalibration.adc_calfactor += (inbuffer[usb_idx+2] * 0x100);
					VoltageCalibration.vdd_offset = inbuffer[usb_idx+3];   
                    VoltageCalibration.vdd_calfactor = inbuffer[usb_idx+4];
                    SaveCalFactorsToEE();
                    usb_idx += 5;
                    break;  

                case READ_STATUS:
                    SendStatusUSB();
                    usb_idx++;
                    break;  

                case READ_VOLTAGES:
                    SendVddVppUSB();
                    usb_idx++;
                    break;     

				case DOWNLOAD_SCRIPT:		// Store a script in the Script Buffer
					// format:		0xA4 <Script#><ScriptLengthN><Script1><Script2>....<ScriptN>
					// response:	-
					usb_idx++; 				// point to Script#
					StoreScriptInBuffer(&usb_idx);
					break;	

                case RUN_SCRIPT:            // run a script from the script buffer
                    // format:      0xA5 <Script#><iterations>
					// response:	-
                    usb_idx++;
                    RunScript(inbuffer[usb_idx], inbuffer[usb_idx + 1]);
                    usb_idx+=2;
                    break;  

                case EXECUTE_SCRIPT:        // immediately executes the included script
                    // format:      0xA6 <ScriptLengthN><Script1><Script2>....<ScriptN>
					// response:	-
                    usb_idx+=1; // points to length byte.
                    ScriptEngine(&inbuffer[usb_idx + 1], inbuffer[usb_idx]);
                    usb_idx += (inbuffer[usb_idx] + 1);
                    break;  

                case CLR_DOWNLOAD_BUFFER:   // empties the download buffer
                    // format:      0xA7
					// response:	-
                    downloadbuf_mgmt.write_index = 0;   // init buffer to enmpty
                    downloadbuf_mgmt.read_index = 0;
                    downloadbuf_mgmt.used_bytes = 0;
                    usb_idx++;
                    break;  

                case DOWNLOAD_DATA:         // add data to download buffer
                    // format:      0xA8 <datalength><data1><data2>....<dataN>
					// response:	-
                    usb_idx++;
                    WriteDownloadDataBuffer(&usb_idx);
                    break;  

                case CLR_UPLOAD_BUFFER:   // empties the upload buffer
                    // format:      0xA9
					// response:	-
                    uploadbuf_mgmt.write_index = 0;   // init buffer to enmpty
                    uploadbuf_mgmt.read_index = 0;
                    uploadbuf_mgmt.used_bytes = 0;
                    usb_idx++;
                    break;  

                case UPLOAD_DATA:       // reads data from upload buffer   
                    // format:      0xAA
                    // response:    <DataLengthN><data1><data2>....<dataN>
                    ReadUploadDataBuffer();
                    usb_idx++;
                    break;  

                case CLR_SCRIPT_BUFFER:
                    // format:      0xAB
					// response:	-
                    ClearScriptTable();
                    usb_idx++;
                    break; 

                case UPLOAD_DATA_NOLEN:   // reads data from upload buffer   
                    // format:      0xAC
                    // response:    <data1><data2>....<dataN>
                    ReadUploadDataBufferNoLength();
                    usb_idx++;
                    break;  

                case RESET:
                    // format:      0xAE
					// response:	-
                    Reset();
                    usb_idx++;
                    break; 

                case SCRIPT_BUFFER_CHKSM:
                    SendScriptChecksumsUSB();
                    usb_idx++;
                    break; 
	
				default:					// End of Buffer or unrecognized command
					usb_idx = 64;			// Stop processing.
			} // end switch
		} while (usb_idx < 64); // end DO

	} // end if (HIDRxReport(inbuffer, 64) > 0)
    
} // end void ProcessIO(void)

/******************************************************************************
 * Function:        void UARTModeService(void)
 *
 * Overview:        Checks for received bytes, to put into the upload buffer
 *                  & checks for data in the download buffer to be sent.
 *
 * PreCondition:    None
 *
 * Input:           UARTStatus struct
 *
 * Output:          UploadBuffer & Download Buffers affected, TMR3 int affected.
 * 
 * Side Effects:    None
 *
 * Note:            
 *****************************************************************************/
void UARTModeService(void)
{
    unsigned char newValue;

    if (UARTStatus.NewRX)
    {
        do {
        newValue = UARTStatus.NewRX;
        UARTStatus.NewRX = 0;
        WriteUploadBuffer(UARTStatus.LastRXByte);
        if (newValue > 1)
        {
            WriteUploadBuffer(UARTStatus.LastRXByte2);
        }
        if (newValue > 2)
        {
            WriteUploadBuffer(UARTStatus.LastRXByte3);
        }
        } while (UARTStatus.NewRX);
    }
    if ((downloadbuf_mgmt.used_bytes) && (PIE2bits.TMR3IE == 0))
    { // start transmission if there is data and a TX is not in progress.
        UARTStatus.TXByte = ReadDownloadBuffer();
        PIR2bits.TMR3IF = 1;
        PIE2bits.TMR3IE = 1;
    }

}

/******************************************************************************
 * Function:        void EnterUARTMode(unsigned char *usbindex)
 *
 * Overview:        Puts the PICkit 2 into UART Mode, where it functions as a
 *                  logic level UART.  ICSPCLK = TX, ICSPDAT = RX
 *
 *                  UART Mode is determined by Pk2Status.UARTMode value.
 *
 * PreCondition:    None
 *
 * Input:           *usbindex - index to start address in USB buffer
 *
 * Output:          Pk2Status.UARTMode = 1
 * 
 * Side Effects:    VppVddADC interrupts are disabled - no V error detection
 *                  The Script Engine will not execute scripts
 *                  ICSPCLK, ICSPDAT, AUX, and VPP pin settings are affected
 *                  Timer1 is reconfigured and used
 *                  Timer3 is used.
 *
 * Note:            The Comparator and Voltage Reference Modules are both used.
 *****************************************************************************/
void EnterUARTMode(unsigned char *usbindex)
{
    INTCONbits.GIE = 0;     // interrupts off

    // Stop ADC interrupts
    VppVddADCTMR1_Stop();

    // init UARTStatus values
    UARTStatus.TimerBaudLoadL = inbuffer[(*usbindex)++];// overhead about 3us per bit.
    UARTStatus.TimerBaudLoadH = inbuffer[(*usbindex)++];
    UARTStatus.RXbits = 0;
    UARTStatus.RXbyte = 0;
    UARTStatus.TXbits = 0;
    UARTStatus.TXByte = 0;
    UARTStatus.LastRXByte = 0;
    UARTStatus.NewRX = 0;

    // Set UART mode bit
    Pk2Status.UARTMode = 1;

    // Setup IO.  ICSPCLK = TX, ICSPDAT = RX
/*    tris_ICSPCLK = 0;   // output
    ICSPCLK_out = 1;    // idle
    tris_ICSPDAT = 1;   // input*/

/*    tris_AUX = 1;       // tri-state
    Vpp_ON = 0;         // VPP off
    MCLR_TGT = 0;       // release*/

    // Setup Comparator & Voltage Reference
    CMCON = 0x2E;       // C2 inverted, C2 Vin- = RA2 (C1 Vin- = RA3) Mode = 110
    CVRCON = 0xA5;      // CVref = 2.0V @ Vdd = 5.0V
    Delay10TCYx(12);    // delay 10us for comparator mode change / CVref settling time

    // Setup Timers
    PIE1bits.TMR1IE = 0;    // RX timer
    PIE2bits.TMR3IE = 0;    // TX timer
    T1CON = 0x91;           // 1:2 prescale, running
    T3CON = 0x91;           // 1:2 prescale, running

    // enable interrupts
    PIR2bits.CMIF = 0;  // clear flag
    PIE2bits.CMIE = 1;  // enable comparator ints.
    

    INTCONbits.GIE = 1;     // interrupts on
}

/******************************************************************************
 * Function:        void ExitUARTMode(void)
 *
 * Overview:        Exits the PICkit 2 from UART Mode, back into normal script
 *                  execution mode for programming operations.
 *
 *                  UART Mode is determined by Pk2Status.UARTMode value.
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          Pk2Status.UARTMode = 0
 * 
 * Side Effects:    VppVddADC interrupts re-enabled
 *                  ICSPCLK, ICSPDAT, and AUX pins are set to inputs
 *                  VPP pin settings are not altered
 *                  Timer1 is reconfigured and used
 *                  Timer3 is turned off.
 *
 * Note:            The Comparator and Voltage Reference Modules are both
 *                  turned off.
 *****************************************************************************/
void ExitUARTMode(void)
{
    INTCONbits.GIE = 0;     // interrupts off

    // clear mode bit
    Pk2Status.UARTMode = 0;
    
    // disabled used interrupts
    PIE2bits.CMIE = 0;
    PIE1bits.TMR1IE = 0;
    PIE2bits.TMR3IE = 0;

    // shut off timers
    T1CON = 0x00;
    T3CON = 0x00;

    // shut off comparators and voltage referece
    CMCON = 0x07;                   // Comparators off
    CVRCON = 0x00;                  // VR off

    // ICSPCLK to input
//    tris_ICSPCLK = 1;

    // restart ADC monitoring
 //   VppVddADCTmr1_Start();

    INTCONbits.GIE = 1;     // interrupts on
}

/******************************************************************************
 * Function:        void ReadInternalEEPROM(unsigned char *usbindex)
 *
 * Overview:        Writes a given # of bytes into the internal MCU EEPROM.
 *
 * PreCondition:    None
 *
 * Input:           *usbindex - index to start address in USB buffer
 *
 * Output:          Transmits HID Tx report with data.
 * 
 * Side Effects:    None
 *
 * Note:            If the length byte is > 32, only the first 32 bytes are 
 *                  Read.
 *****************************************************************************/
void ReadInternalEEPROM(unsigned char *usbindex)
{
    unsigned int i, numbytes;
    unsigned char ee_address;

    ee_address = inbuffer[(*usbindex)++];   // starting address.
    numbytes = inbuffer[(*usbindex)++] & 0xFF;   // i= # bytes data (length)

    if (numbytes  > 32)     // more than allowed # bytes
    {
        numbytes = 32;
    }
    if (numbytes == 0)
    {
        return;
    }

    for (i = 0; i < numbytes; i++)
    {
	    outbuffer[i] = EE_ReadByte(ee_address++);
    }

    // transmit data
    while(mHIDTxIsBusy()){}         // blocking
    HIDTxReport(outbuffer, 64);
    USBDriverService();
} 

/******************************************************************************
 * Function:        void WriteInternalEEPROM(unsigned char *usbindex)
 *
 * Overview:        Writes a given # of bytes into the internal MCU EEPROM.
 *
 * PreCondition:    None
 *
 * Input:           *usbindex - index to start address of data in USB buffer
 *
 * Output:          Internal EEPROM - updated with new data
 *
 * Side Effects:    None
 *
 * Note:            If the length byte is > 32, only the first 32 bytes are 
 *                  written.
 *****************************************************************************/
void WriteInternalEEPROM(unsigned char *usbindex)
{
    unsigned int i, numbytes;
    unsigned char ee_address;

    ee_address = inbuffer[(*usbindex)++];   // starting address.
    numbytes = inbuffer[(*usbindex)++] & 0xFF;   // i= # bytes data (length)

    if (numbytes  > 32)     // more than allowed # bytes
    {
        numbytes = 32;
    }
    if (numbytes == 0)
    {
        return;
    }

    for (i = 0; i < numbytes; i++)
    {
        EE_WriteByte(ee_address++, inbuffer[(*usbindex)++]);
    }
}

/******************************************************************************
 * Function:        void CalAndSetCCP1(unsigned char ccp1_upper, 
 *                                                     unsigned char ccp1_lower)
 *
 * Overview:        Calibrates the 10-bit left-justified CCP1 value in the two
 *                  argument bytes, and loads it into CCPR1L & CCP1CON
 *                  CalResult = (((CCP1 >> 6) + VoltageCalibration.vdd_offset)
 *                                 * VoltageCalibration.vdd_calfactor) >> 7
 *
 * PreCondition:    None
 *
 * Input:           ccp1_upper - CCPR1L value, ccp1_lower 2 LSbs in bits 7:6
 *
 * Output:          sets CCPR1L and CCP1CON to calibrated 10-bit value.
 *
 * Side Effects:    None
 *
 * Note:            VoltageCalibration.vdd_offset is signed
 *****************************************************************************/
void CalAndSetCCP1(unsigned char ccp1_upper, unsigned char ccp1_lower)
{

    signed long short cal_value = (ccp1_upper * 0x100) + ccp1_lower;
    cal_value >>= 6;
    cal_value += VoltageCalibration.vdd_offset;
    cal_value *= VoltageCalibration.vdd_calfactor;
    cal_value >>= 1;
    CCPR1L = (unsigned char) (cal_value >> 8);
    cal_value &= 0xFF;
    CCP1CON = (CCP1CON & 0xCF) | (cal_value >> 2);

}

/******************************************************************************
 * Function:        unsigned int CalADCWord(unsigned int rawValue)
 *
 * Overview:        Calibrates the given word with cal factors in
 *                  VoltageCalibration.
 *                  CalResult = 
 *                      (rawValue * VoltageCalibration.adc_calfactor) >> 8
 *
 * PreCondition:    None
 *
 * Input:           VoltageCalibration struct, rawValue byte
 *
 * Output:          returns calibrated value.
 *
 * Side Effects:    None
 *
 * Note:            Performs "Reverse Calibration" to threshold value
 *                  matches actual ADC value.
 *****************************************************************************/
unsigned int CalADCWord(unsigned int rawValue)
{

    unsigned long cal_value = rawValue;
    cal_value *= VoltageCalibration.adc_calfactor;
    cal_value >>= 8;
    if (cal_value > 0xFFFF)
        cal_value = 0xFFFF;

    return (unsigned int) cal_value;
}


/******************************************************************************
 * Function:        unsigned char CalThresholdByte(unsigned char rawValue)
 *
 * Overview:        Calibrates the given byte with cal factors in
 *                  VoltageCalibration.
 *                  CalResult = (rawValue/VoltageCalibration.adc_calfactor) * 256
 *
 * PreCondition:    None
 *
 * Input:           VoltageCalibration struct, rawValue byte
 *
 * Output:          returns calibrated value.
 *
 * Side Effects:    None
 *
 * Note:            Performs "Reverse Calibration" to threshold value
 *                  matches actual ADC value.
 *****************************************************************************/
unsigned char CalThresholdByte(unsigned char rawValue)
{
    unsigned int inverse_cal = 0x0200 - VoltageCalibration.adc_calfactor;
    inverse_cal *= rawValue;
    inverse_cal >>= 8;

    return (unsigned char) inverse_cal;
}

/******************************************************************************
 * Function:        void SaveCalFactorsToEE(void)
 *
 * Overview:        Saves voltage calibration factors to EEPROM.
 *
 * PreCondition:    None
 *
 * Input:           VoltageCalibration struct.
 *
 * Output:          Stores data in 4 bytes of EE Memory
 *
 * Side Effects:    None
 *
 * Note:            None
 *****************************************************************************/
void SaveCalFactorsToEE(void)
{
    EE_WriteByte(ADC_CAL_L, (unsigned char)VoltageCalibration.adc_calfactor);
    EE_WriteByte(ADC_CAL_H, (unsigned char)(VoltageCalibration.adc_calfactor >> 8));
    EE_WriteByte(CPP_OFFSET, VoltageCalibration.vdd_offset);
    EE_WriteByte(CPP_CAL, VoltageCalibration.vdd_calfactor);
}

/******************************************************************************
 * Function:        void ReadCalFactorsFromEE(void)
 *
 * Overview:        Reads voltage calibration factors from EEPROM.
 *
 * PreCondition:    None
 *
 * Input:           VoltageCalibration struct.
 *
 * Output:          Reads data in 4 bytes of EE Memory into VoltageCalibration
 *                  Struct.
 *
 * Side Effects:    None
 *
 * Note:            None
 *****************************************************************************/
void ReadCalFactorsFromEE(void)
{
    VoltageCalibration.adc_calfactor = (EE_ReadByte(ADC_CAL_L) & 0xFF);
    VoltageCalibration.adc_calfactor += (EE_ReadByte(ADC_CAL_H) * 0x100);
    VoltageCalibration.vdd_offset = EE_ReadByte(CPP_OFFSET);
    VoltageCalibration.vdd_calfactor = EE_ReadByte(CPP_CAL);
}

/******************************************************************************
 * Function:        void SendScriptChecksumsUSB(void)
 *
 * Overview:        Calculates and responds with checksums of the Script Buffer.
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          Transmits HID Tx report with 4 bytes.
 *                      First 2 bytes are a 16-bit sum of the lengths in the 
 *                           ScriptTable
 *                      Second 2 are 16-bit sum of all used script bytes.
 *
 * Side Effects:    Stops and restarts Timer1/ADC voltage monitor.
 *
 * Note:            None
 *****************************************************************************/
void SendScriptChecksumsUSB(void)
{
    int length_checksum = 0;
    int buffer_checksum = 0;
    int i;
    
    for (i = 0; i < SCRIPT_ENTRIES; i++)
    {
        length_checksum += ScriptTable[i].Length;
    }

    for (i = 0; i < length_checksum; i++)
    {
        buffer_checksum += *(uc_ScriptBuf_ptr + i);
    }

    outbuffer[0] = (length_checksum & 0xFF);
    outbuffer[1] = (length_checksum >> 8);
	outbuffer[2] = (buffer_checksum & 0xFF);;
	outbuffer[3] = (buffer_checksum >> 8);;
    // transmit conversion results
    while(mHIDTxIsBusy()){}         // blocking
    HIDTxReport(outbuffer, 64);
    USBDriverService();
}

/******************************************************************************
 * Function:        void SendVddVppUSB(void)
 *
 * Overview:        ADC converts VDD and VPP voltages and send results via USB.
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          Transmits HID Tx report with 4 bytes.
 *
 * Side Effects:    Stops and restarts Timer1/ADC voltage monitor.
 *
 * Note:            None
 *****************************************************************************/
void SendVddVppUSB(void)
{
    unsigned int adc_result;    

    VppVddADCTMR1_Stop();
    
    ADCConvert(ADC_CH1_VDD);
    adc_result = (ADRESH * 0x100) + ADRESL;
    adc_result = CalADCWord(adc_result);
	outbuffer[0] = (unsigned char) adc_result;
	outbuffer[1] = (unsigned char) (adc_result >> 8);

    ADCConvert(ADC_CH0_VPP);
    adc_result = (ADRESH * 0x100) + ADRESL;
    adc_result = CalADCWord(adc_result);
	outbuffer[2] = (unsigned char) adc_result;
	outbuffer[3] = (unsigned char) (adc_result >> 8);
    // transmit conversion results
    while(mHIDTxIsBusy()){}         // blocking
    HIDTxReport(outbuffer, 64);
    USBDriverService();

    VppVddADCTmr1_Start();
} // end void SendVddVppUSB(void)


/******************************************************************************
 * Function:        void ADCConvert(unsigned char channel)
 *
 * Overview:        ADC converts the given channel.
 *
 * PreCondition:    None
 *
 * Input:           channel - channel to convert- bits must follow ADCON0
 *
 * Output:          conversion results in ADRESH and ADRESL
 *
 * Side Effects:    Uses ADC.  Leaves ADIF asserted.
 *
 * Note:            Expected to use default ADC settings from VppVddADCTmr1_Start()
 *                  May be changed by the calling routine if others are desired or to
 *                  enabled AN2-AN4 as analog pins.
 *****************************************************************************/
void ADCConvert(unsigned char channel)
{
    ADCON0 = (channel + 1);     // set channel and turn ADC on.
    ADCON0bits.GO = 1;          // begin conversion
    
    while (ADCON0bits.GO)       // wait for it to complete
    {
    }

}// void ADCConvert(unsigned char channel)


/******************************************************************************
 * Function:        void ReadUploadDataBuffer(void)
 *
 * Overview:        Sends data from upload data buffer over USB.
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          Transmits HID Tx report with data length and data.
 *
 * Side Effects:    None
 *
 * Note:            None
 *****************************************************************************/
void ReadUploadDataBuffer(void)
{
    unsigned char i, length;

    length = uploadbuf_mgmt.used_bytes;
    if (length > (BUF_SIZE - 1))        // limited to # bytes in USB report - length byte
    {
        length = (BUF_SIZE - 1);
    }

	outbuffer[0] = length;
    for (i = 1; i<= length; i++)
    {
	    outbuffer[i] = uc_upload_buffer[uploadbuf_mgmt.read_index++];
        if (uploadbuf_mgmt.read_index >= UPLOAD_SIZE)  // manage buffer wrap.
        {
            uploadbuf_mgmt.read_index = 0;
        }

    }

    uploadbuf_mgmt.used_bytes -= length;    // read out this many bytes.

    // transmit data
    while(mHIDTxIsBusy()){}         // blocking
    HIDTxReport(outbuffer, 64);
    USBDriverService();
} // end void ReadUploadDataBuffer(void)


/******************************************************************************
 * Function:        void ReadUploadDataBufferNoLength(void)
 *
 * Overview:        Sends data from upload data buffer over USB,
 *                  but does not add a length byte.
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          Transmits HID Tx report with data only.
 *
 * Side Effects:    None
 *
 * Note:            None
 *****************************************************************************/
void ReadUploadDataBufferNoLength(void)
{
    unsigned char i, length;

    length = uploadbuf_mgmt.used_bytes;
    if (length > (BUF_SIZE))        // limited to # bytes in USB report
    {
        length = (BUF_SIZE);
    }

    for (i = 0; i < length; i++)
    {
	    outbuffer[i] = uc_upload_buffer[uploadbuf_mgmt.read_index++];
        if (uploadbuf_mgmt.read_index >= UPLOAD_SIZE)  // manage buffer wrap.
        {
            uploadbuf_mgmt.read_index = 0;
        }

    }

    uploadbuf_mgmt.used_bytes -= length;    // read out this many bytes.

    // transmit data
    while(mHIDTxIsBusy()){}         // blocking
    HIDTxReport(outbuffer, 64);
    USBDriverService();
} // end void ReadUploadDataBufferNoLength(void)


/******************************************************************************
 * Function:        void RunScript(unsigned char scriptnumber, unsigned char repeat)
 *
 * Overview:        Runs a given script# from the Script Buffer "repeat" number of
 *                  times.
 *
 * PreCondition:    Must be a valid script in the script buffer
 *
 * Input:           scriptnumber = # of script to run
 *                  repeat = how many times to run the script
 *
 * Output:          Pk2Status.EmptyScript set if no script at given script#
 *
 * Side Effects:    Dependent on script being run.
 *
 * Note:            None
 *****************************************************************************/
void RunScript(unsigned char scriptnumber, unsigned char repeat)
{
    // check for valid script #
    if ((scriptnumber >= SCRIPT_ENTRIES) || (ScriptTable[scriptnumber].Length == 0))
    {
        Pk2Status.EmptyScript = 1;  // set error
        return;
    }

    do
    {
        ScriptEngine((uc_ScriptBuf_ptr + ScriptTable[scriptnumber].StartIndex) , ScriptTable[scriptnumber].Length);
        repeat--;
    } while (repeat > 0);

} // end void RunScript(unsigned char scriptnumber, unsigned char repeat)


/******************************************************************************
 * Function:        void ClearScriptTable(void)
 *
 * Overview:        Clears Script buffer by setting all Script Table length entries to zero.
 *
 * PreCondition:    None
 *
 * Input:           None.
 *
 * Output:          ScriptTable[x].Length = 0 for all valid x.
 *
 * Side Effects:    None
 *
 * Note:            None
 *****************************************************************************/
void ClearScriptTable(void)
{
    unsigned char i;

	for (i=0; i < SCRIPT_ENTRIES; i++) // init script table to empty.
	{
		ScriptTable[i].Length = 0;
	}
} // end void ClearScriptTable(void)

/******************************************************************************
 * Function:        void WriteDownloadDataBuffer(unsigned char *usbindex)
 *
 * Overview:        Writes a given # of bytes into the data download buffer.
 *
 * PreCondition:    None
 *
 * Input:           *usbindex - index to length of data in USB buffer
 *
 * Output:          uc_download_buffer[] - updated with new data
 *                  downloadbuf_mgmt.write_index - incremented by length of data stored.
 *                  downloadbuf_mgmt.used_bytes - incremented by length of data stored. 
 *                  Pk2Status.DownloadOvrFlow - set if data length > remaining buffer
 *
 * Side Effects:    None
 *
 * Note:            None
 *****************************************************************************/
void WriteDownloadDataBuffer(unsigned char *usbindex)
{
    unsigned int i, numbytes;

    numbytes = inbuffer[(*usbindex)++] & 0xFF;   // i= # bytes data (length)

    if ((numbytes + downloadbuf_mgmt.used_bytes)  > DOWNLOAD_SIZE)     // not enough room for data
    {
        Pk2Status.DownloadOvrFlow = 1;
        return;
    }

    for (i = 0; i < numbytes; i++)
    {
        uc_download_buffer[downloadbuf_mgmt.write_index++] = inbuffer[(*usbindex)++];
        if (downloadbuf_mgmt.write_index >= DOWNLOAD_SIZE) // handle index wrap
        {
            downloadbuf_mgmt.write_index = 0;
        }
        downloadbuf_mgmt.used_bytes++;  // used another byte.
    }
} // end void WriteDownloadDataBuffer(unsigned char *usbindex)

/******************************************************************************
 * Function:        void StoreScriptInBuffer(unsigned char *usbindex)
 *
 * Overview:        Stores the script from USB buffer into Script Buffer & updates
 *                  the Script Table.
 *                  Prior script at the given script # is deleted and all following
 *                  scripts are moved up.  New script is appended at end.
 *
 * PreCondition:    None
 *
 * Input:           *usbindex - index to script # byte in USB buffer
 *
 * Output:          uc_script_buffer[] - updated
 *                  ScriptTable[] - updated
 *                  Pk2Status.ScriptBufOvrFlow - set if script length > remaining buffer
 *
 * Side Effects:    None
 *
 * Note:            None
 *****************************************************************************/
void StoreScriptInBuffer(unsigned char *usbindex)
{
	int i;
	int LengthOfAllScripts;
	int Temp_1, Temp_2;

	Temp_1 = inbuffer[(*usbindex)+1];  	// Length of new script

	// First, make sure script length is valid
	if (Temp_1 > SCRIPT_MAXLEN)
	{
		Pk2Status.ScriptBufOvrFlow = 1;		// set error - script longer than max allowed
		return;
	}

	Temp_2 = inbuffer[*usbindex];		// Script# of new script 

	// calculate length of all scripts.
	LengthOfAllScripts = 0;
	for (i=0; i < SCRIPT_ENTRIES; i++)
	{
		LengthOfAllScripts += ScriptTable[i].Length;
	}
	LengthOfAllScripts -= ScriptTable[Temp_2].Length;	// don't count length of script being replaced
	if (Temp_1 > (SCRIPTBUFSPACE-LengthOfAllScripts)) // if there isn't enough room
	{
		Pk2Status.ScriptBufOvrFlow = 1;		// set error - not enough room in script buffer
		return;
	}	

	// Next, make sure script# is valid
	if (Temp_2 > (SCRIPT_ENTRIES-1))    // 0-31 valid
	{
		Pk2Status.ScriptBufOvrFlow = 1;		// set error - script# invalid
		return;
	}


	if (ScriptTable[Temp_2].Length != 0)  // If a script exists in that location
	{
		// Move space created by deleting existing script to end of buffer.
		Temp_1 = (SCRIPTBUFSPACE - ScriptTable[Temp_2].Length) - 1;  // last copy location.
		for (i=ScriptTable[Temp_2].StartIndex; i < Temp_1; i++)
		{
			*(uc_ScriptBuf_ptr + i) = *(uc_ScriptBuf_ptr + ScriptTable[Temp_2].Length + i);
		}
		// update script table entries
		for (i=0; i < SCRIPT_ENTRIES; i++)
		{
			if (ScriptTable[i].StartIndex > ScriptTable[Temp_2].StartIndex) // if script is in moved section
			{
				ScriptTable[i].StartIndex -= ScriptTable[Temp_2].Length;  // adjust by amount moved
			} 
		}
	}

	// Store new script at end of buffer
	ScriptTable[Temp_2].Length = inbuffer[(*usbindex)+1];	// update Script Table Entry with new length.
	ScriptTable[Temp_2].StartIndex = LengthOfAllScripts;    // update entry with new index at end of buffer.
	*usbindex += 2;	// point to first byte of new script in USB buffer.
	for (i = 0; i < ScriptTable[Temp_2].Length; i++)
	{
		*(uc_ScriptBuf_ptr + LengthOfAllScripts + i) = 	inbuffer[(*usbindex)++];	
	}  

} // end void StoreScriptInBuffer(unsigned char *usbindex)

/******************************************************************************
 * Function:        void EnterBootloader(void)
 *
 * Overview:        Resets the 2550 into bootloader mode.
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    Erases Flash location 0x7FFE then resets the 2550.
 *
 * Note:            None
 *****************************************************************************/
void EnterBootloader(void)
{
    INTCONbits.GIE = 0;             	// shut down global interrupts	
	USBSoftDetach();
	// erase location 0x7FFE
	EECON1 = 0b10000100;                // Setup writes: EEPGD=1, WREN=1
	*((rom far char *) 0x7FFE) = 0;
	EECON2 = 0x55;                      // Start Write
	EECON2 = 0xAA;
	EECON1bits.WR = 1;

	Delay10KTCYx(78);					// delay (~65 ms @ 48 MHz))
    Reset();
} // end void EnterBootloader(void)


/******************************************************************************
 * Function:        void SendStatusUSB(void)
 *
 * Overview:        Sends READ_STATUS response over USB.
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          Transmits HID Tx report with Pk2Status.
 *
 * Side Effects:    None
 *
 * Note:            None
 *****************************************************************************/
void SendStatusUSB(void)
{
    Pk2Status.StatusLow &= 0xF0;    // clear bits to be tested
//    if (Vpp_ON_pin)         // active high
        Pk2Status.VppOn = 1;
 //   if (MCLR_TGT_pin)       // active high
        Pk2Status.VppGNDOn = 1;
 //   if (!Vdd_TGT_P_pin)     // active low
        Pk2Status.VddOn = 1;
 //   if (Vdd_TGT_N_pin)      // active high
        Pk2Status.VddGNDOn = 1;

	outbuffer[0] = Pk2Status.StatusLow;
	outbuffer[1] = Pk2Status.StatusHigh;

    // Now that it's in the USB buffer, clear errors & flags
    Pk2Status.StatusLow &= 0x8F;
    Pk2Status.StatusHigh &= 0x00;
    BUSY_LED = 0;                   // ensure it stops blinking at off.

    // transmit status
    while(mHIDTxIsBusy()){}         // blocking
    HIDTxReport(outbuffer, 64);
    USBDriverService();
} // end void SendStatusUSB(void)

/******************************************************************************
 * Function:        void SendFWVersionUSB(void)
 *
 * Overview:        Sends firmware version over USB.
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          Transmits HID Tx report with 3-byte version #.
 *
 * Side Effects:    None
 *
 * Note:            None
 *****************************************************************************/
void SendFWVersionUSB(void)
{
	outbuffer[0] = MAJORVERSION;
	outbuffer[1] = MINORVERSION;
	outbuffer[2] = DOTVERSION;

    // transmit version number    
    while(mHIDTxIsBusy()){}         // blocking
    HIDTxReport(outbuffer, 64);
    USBDriverService();

} // end void SendFWVersionUSB(void)


/******************************************************************************
 * Function:        void EnsureBOROff(void)
 *
 * Overview:        Reads CONFIG2L.  If any BOR bits are set, it re-writes
 *                  CONFIG2L with BOR disabled.
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          May affect CONFIG2L BOREN1/0 value(s).
 *
 * Side Effects:    None
 *
 * Note:            None
 *****************************************************************************/
void EnsureBOROff(void)
{
    char cfgval;
    rom far char *cfg2l = cfg2l_address;

    cfgval = *cfg2l;
    
    if ((cfgval & cfg2l_mask) > 0 )
    {
        EECON1 = 0b11000100;        // Setup write: EEPGD=1, CFGS=1, WREN=1
        cfgval &= ~cfg2l_mask;       // clear BOREN bits
        *cfg2l = cfgval;
        EECON2 = 0x55;              // rewrite config word.
        EECON2 = 0xAA;
        EECON1bits.WR = 1;
        
    }

    TBLPTRU = 0;

}

//#####################################################################################################
//#####################################################################################################
//#####################################################################################################
//#######                                                                                       #######
//#######                                    SCRIPT ENGINE                                      #######
//#######                                                                                       #######
//#####################################################################################################
//#####################################################################################################
//#####################################################################################################

/******************************************************************************
 * Function:        void ScriptEngine(unsigned char *scriptstart_ptr, unsigned char scriptlength)
 *
 * Overview:        Executes the script pointed to by scriptstart_ptr from the Script Buffer
 *					Aborts if a control byte attempts to use a byte from an empty Download buffer
 *                  or store a byte in a full Upload Buffer.  Will not execute if 
 *                  Pk2Status.StatusHigh != 0 (a script error exists.)
 *
 * PreCondition:    None
 *
 * Input:           *scriptstart_ptr - Pointer to start of script
 *                  scriptlength - length of script
 *
 * Output:          uc_downloadbuf_read - advanced by the number of bytes read.
 *                  uc_upload_buffer[] - new data may be stored
 *                  uc_uploadbuf_write - advance by number of bytes written.
 *                  Pk2Status.StatusHigh - set if script error occurs
 *
 * Side Effects:    Uses Timer0.
 *
 * Note:            None
 *****************************************************************************/
void ScriptEngine(unsigned char *scriptstart_ptr, unsigned char scriptlength)
{
	unsigned char scriptindex = 0;
	unsigned char temp_byte;
	unsigned char loopcount, loopindex, loopbufferindex;
    unsigned int loopbuffercount;
    unsigned char *SFR_ptr;
	BOOL loopactive = 0;
    BOOL loopbufferactive = 0;

    INTCONbits.T0IE = 0; // ensure Timer0 interrupt diabled.
    T0CON = 0x07;       // 16-bit timer, 1:256 prescale.

	if ((scriptlength == 0) || (scriptlength > SCRIPT_MAXLEN))
	{
		Pk2Status.EmptyScript = 1;		// set error - script length out of bounds
		return;		
	}

	while (((Pk2Status.StatusHigh & STATUSHI_ERRMASK) == 0) && (scriptindex < scriptlength))
	{
		switch (*(scriptstart_ptr + scriptindex))
		{
            // NOTE : More frequently used cases are placed near the end of the switch-case
            // as they are compared first, to speed up execution.
			case VDD_ON:
//				Vdd_TGT_P = 0;
				scriptindex++;
				break;

			case VDD_OFF:
//				Vdd_TGT_P = 1;
				scriptindex++;
				break;

			case VDD_GND_ON:
//				Vdd_TGT_N = 1;
				scriptindex++;
				break;

			case VDD_GND_OFF:
//				Vdd_TGT_N = 0;
				scriptindex++;
				break;

			case VPP_ON:
//				Vpp_ON = 1;
				scriptindex++;
				break;

			case VPP_OFF:
//				Vpp_ON = 0;
				scriptindex++;
				break;

			case VPP_PWM_ON:
//				CCP2CON = VppPWM_Enable;
				scriptindex++;
				break;

			case VPP_PWM_OFF:
//				CCP2CON = VppPWM_Disable;
//                Vpp_PUMP = 0;               // pass VDD
				scriptindex++;
				break;

			case MCLR_GND_ON:
//				MCLR_TGT = 1;
				scriptindex++;
				break;

			case MCLR_GND_OFF:
//				MCLR_TGT = 0;
				scriptindex++;
				break;

			case BUSY_LED_ON:
				BUSY_LED = 1;
				scriptindex++;
				break;

			case BUSY_LED_OFF:
				BUSY_LED = 0;
				scriptindex++;
				break;

			case DELAY_LONG:
				scriptindex++;
				LongDelay(*(scriptstart_ptr + scriptindex));
				scriptindex++;
				break;

			case SET_ICSP_PINS:
				scriptindex++;
				icsp_pins = *(scriptstart_ptr + scriptindex);
				SetICSP_PinStates(icsp_pins);
				scriptindex++;
				break;

			case SET_ICSP_SPEED:
				scriptindex++;
                icsp_baud = *(scriptstart_ptr + scriptindex);
				scriptindex++;
				break;

			case LOOPBUFFER:
				if (loopbufferactive)
                {
                    loopbuffercount--;
                    if (loopbuffercount == 0)
                    {
                        loopbufferactive = 0;
                        scriptindex+=2;
                        break;    
                    }
                    scriptindex = loopbufferindex;
                    break;
                }
				loopbufferindex = scriptindex - *(scriptstart_ptr + scriptindex + 1);
                loopbuffercount = (unsigned int) ReadDownloadBuffer();  // low byte
                loopbuffercount += (256 * ReadDownloadBuffer());        // upper byte
                if (loopbuffercount == 0)
                { // value of "zero" 0x0000 means no loops.
                    scriptindex+=2;
                    break;
                }
                loopbufferactive = 1;
                scriptindex = loopbufferindex;
				break;

			case READ_BYTE:
				ShiftBitsInICSP(8);
				scriptindex++;
				break;

			case READ_BITS:
				scriptindex++;
				ShiftBitsInICSP(*(scriptstart_ptr + scriptindex));
				scriptindex++;
				break;

			case LOOP:
				if (loopactive)
                {
                    loopcount--;
                    if (loopcount == 0)
                    {
                        loopactive = 0;
                        scriptindex+=3;
                        break;    
                    }
                    scriptindex = loopindex;
                    break;
                }
                loopactive = 1;
				loopindex = scriptindex - *(scriptstart_ptr + scriptindex + 1);
                loopcount = *(scriptstart_ptr + scriptindex + 2);
                scriptindex = loopindex;
				break;

			case DELAY_SHORT:
				scriptindex++;
				ShortDelay(*(scriptstart_ptr + scriptindex));
				scriptindex++;
				break;

			case READ_BITS_BUFFER:
                scriptindex++;
				WriteUploadBuffer(ShiftBitsInICSP(*(scriptstart_ptr + scriptindex)));
				scriptindex++;
				break;

			case WRITE_BITS_BUFFER:
				scriptindex++;
                ShiftBitsOutICSP(ReadDownloadBuffer(), *(scriptstart_ptr + scriptindex));
				scriptindex++;
				break;

			case READ_BYTE_BUFFER:
				WriteUploadBuffer(ShiftBitsInICSP(8));
				scriptindex++;
				break;

			case WRITE_BYTE_LITERAL:
				scriptindex++;
                ShiftBitsOutICSP(*(scriptstart_ptr + scriptindex), 8);
				scriptindex++;
				break;

			case WRITE_BITS_LITERAL:
                scriptindex++;
                ShiftBitsOutICSP(*(scriptstart_ptr + scriptindex + 1), *(scriptstart_ptr + scriptindex));
				scriptindex+=2;
				break;

			case WRITE_BYTE_BUFFER:
                ShiftBitsOutICSP(ReadDownloadBuffer(), 8);
				scriptindex++;
				break;

			default:
                if (*(scriptstart_ptr + scriptindex) >= WRITE_BITS_BUF_HLD)
                {
    				switch (*(scriptstart_ptr + scriptindex))
                    {
                        case CONST_WRITE_DL:
                            scriptindex++;
                            WriteByteDownloadBuffer(*(scriptstart_ptr + scriptindex));
                            scriptindex++;
    				        break; 
    
                        case POP_DOWNLOAD:
                            ReadDownloadBuffer();
    				        scriptindex++;
    				        break;    
    
                        case RD2_BITS_BUFFER: 
                            scriptindex++;
            				WriteUploadBuffer(ShiftBitsInPIC24(*(scriptstart_ptr + scriptindex)));
            				scriptindex++;
            				break;   
    
    					case WRITE_BITS_LIT_HLD:
    		                scriptindex++;
    		                ShiftBitsOutICSPHold(*(scriptstart_ptr + scriptindex + 1), *(scriptstart_ptr + scriptindex));
    						scriptindex+=2;
    						break;
    		
    					case WRITE_BITS_BUF_HLD:
    						scriptindex++;
    		                ShiftBitsOutICSPHold(ReadDownloadBuffer(), *(scriptstart_ptr + scriptindex));
    						scriptindex++;
    						break;                
    
                        case ICSP_STATES_BUFFER:
                            WriteUploadBuffer(GetICSP_PinStates());
    				        scriptindex++;
    				        break;
                            
                        case IF_EQ_GOTO:
                            temp_byte = uc_upload_buffer[uploadbuf_mgmt.write_index - 1]; // last byte written
                            if (temp_byte == *(scriptstart_ptr + scriptindex + 1))
                            {
    				            scriptindex = scriptindex + (signed char)*(scriptstart_ptr + scriptindex + 2);                       
                            }
                            else
                            {
    				            scriptindex+=3;
                            }
    			        	break;
    
                        case IF_GT_GOTO:
                            temp_byte = uc_upload_buffer[uploadbuf_mgmt.write_index - 1]; // last byte written
                            if (temp_byte > *(scriptstart_ptr + scriptindex + 1))
                            {
    				            scriptindex = scriptindex + (signed char)*(scriptstart_ptr + scriptindex + 2);                       
                            }
                            else
                            {
    				            scriptindex+=3;
                            }
    			        	break;
    
                        case GOTO_INDEX:
    				        scriptindex = scriptindex + (signed char)*(scriptstart_ptr + scriptindex + 1);                       
    			        	break;
    
                        case POKE_SFR:
    				        scriptindex++;
                            SFR_ptr = (unsigned char *)0x0F00 + *(scriptstart_ptr + scriptindex++);
                            *SFR_ptr = *(scriptstart_ptr + scriptindex++);
    			        	break;
    
                        case PEEK_SFR:
    				        scriptindex++;
                            SFR_ptr = (unsigned char *)0x0F00 + *(scriptstart_ptr + scriptindex);
                            WriteUploadBuffer(*SFR_ptr);
    				        scriptindex++;
    			        	break;
    
                        case WRITE_BUFBYTE_W:
                            scriptindex++;
                            ShiftBitsOutICSP(0, 4); // six code
                            ShiftBitsOutICSP(*(scriptstart_ptr + scriptindex), 4); // W nibble
                            ShiftBitsOutICSP(ReadDownloadBuffer(), 8); // literal LSB
                            ShiftBitsOutICSP(0, 8); // literal MSB
                            ShiftBitsOutICSP(0x2, 4); // opcode
    				        scriptindex++;
    			        	break;
    
                        case WRITE_BUFWORD_W:
                            scriptindex++;
                            ShiftBitsOutICSP(0, 4); // six code
                            ShiftBitsOutICSP(*(scriptstart_ptr + scriptindex), 4); // W nibble
                            ShiftBitsOutICSP(ReadDownloadBuffer(), 8); // literal LSB
                            ShiftBitsOutICSP(ReadDownloadBuffer(), 8); // literal MSB
                            ShiftBitsOutICSP(0x2, 4); // opcode
    				        scriptindex++;
    			        	break;
    
    					case VISI24:
    						scriptindex++;
    						ShiftBitsOutICSP(1, 4);
    						ShiftBitsOutICSP(0, 8);
    						WriteUploadBuffer(ShiftBitsInPIC24(8));
    						WriteUploadBuffer(ShiftBitsInPIC24(8));
    						break;
    
    					case NOP24:
    						scriptindex++;
    						ShiftBitsOutICSP(0, 4);
    						ShiftBitsOutICSP(0, 8);
    						ShiftBitsOutICSP(0, 8);
    						ShiftBitsOutICSP(0, 8);
    						break;
    
    					case COREINST18:
    						scriptindex++;
    						ShiftBitsOutICSP(0, 4);
    						ShiftBitsOutICSP(*(scriptstart_ptr + scriptindex++), 8);
    						ShiftBitsOutICSP(*(scriptstart_ptr + scriptindex++), 8);
    						break;
    
    					case COREINST24:
    						scriptindex++;
    						ShiftBitsOutICSP(0, 4);
    						ShiftBitsOutICSP(*(scriptstart_ptr + scriptindex++), 8);
    						ShiftBitsOutICSP(*(scriptstart_ptr + scriptindex++), 8);
    						ShiftBitsOutICSP(*(scriptstart_ptr + scriptindex++), 8);
    						break;
    
                        case ICDSLAVE_RX:
                            scriptindex++;
                            WriteUploadBuffer(ICDSlave_Receive());
                            break;
    
                        case ICDSLAVE_TX_LIT:
                            scriptindex++;
                            ICDSlave_transmit(*(scriptstart_ptr + scriptindex++));
                            break;
    
                        case ICDSLAVE_TX_BUF:
                            scriptindex++;
                            ICDSlave_transmit(ReadDownloadBuffer());
                            break;
    
    					case RD2_BYTE_BUFFER:
    						scriptindex++;
    						WriteUploadBuffer(ShiftBitsInPIC24(8));
    						break;
        
                        case EXIT_SCRIPT:
                        default:
                            scriptindex = scriptlength;
                    } // end switch case
                }
                else
                {
                    switch (*(scriptstart_ptr + scriptindex))
                    {
            			case SET_AUX:
            				scriptindex++;
            				aux_pin = *(scriptstart_ptr + scriptindex);
            				SetAUX_PinState(aux_pin);
            				scriptindex++;
            				break;

                        case AUX_STATE_BUFFER:
                            WriteUploadBuffer(GetAUX_PinState());
    				        scriptindex++;
    				        break;

                        case I2C_START:
                            I2C_Start();
    				        scriptindex++;
    				        break;

                        case I2C_STOP:
                            I2C_Stop();
    				        scriptindex++;
    				        break;

            			case I2C_WR_BYTE_LIT:
            				scriptindex++;
                            I2C_Write(*(scriptstart_ptr + scriptindex));
            				scriptindex++;
            				break;

            			case I2C_WR_BYTE_BUF:
            				scriptindex++;
                            I2C_Write(ReadDownloadBuffer());
            				break;

                        case I2C_RD_BYTE_ACK:
            				WriteUploadBuffer(I2C_Read(ACK_BYTE));
            				scriptindex++;
            				break;

                        case I2C_RD_BYTE_NACK:
            				WriteUploadBuffer(I2C_Read(NO_ACK_BYTE));
            				scriptindex++;
            				break;

                        case SPI_WR_BYTE_LIT:
                            scriptindex++;
                            SPI_ReadWrite(*(scriptstart_ptr + scriptindex));
            				scriptindex++;
                            break;

                        case SPI_WR_BYTE_BUF:
                            scriptindex++;
                            SPI_ReadWrite(ReadDownloadBuffer());
            				break;

                        case SPI_RD_BYTE_BUF:
            				WriteUploadBuffer(SPI_ReadWrite(0));
            				scriptindex++;
            				break;

                        case SPI_RDWR_BYTE_LIT:
                            scriptindex++;
                            WriteUploadBuffer(SPI_ReadWrite(*(scriptstart_ptr + scriptindex)));
            				scriptindex++;
                            break;

                        case SPI_RDWR_BYTE_BUF:
                            scriptindex++;
                            WriteUploadBuffer(SPI_ReadWrite(ReadDownloadBuffer()));
            				scriptindex++;
                            break;

                        default:
                            scriptindex = scriptlength;
                    } // end switch case
                } //end if else
		} // end switch-case
	} // end;

} //end void ScriptEngine(unsigned char *scriptstart_ptr, unsigned char scriptlength)

/******************************************************************************
 * Function:        void WriteByteDownloadBuffer (unsigned char DataByte)
 *
 * Overview:        Puts a byte in the download buffer
 *
 * PreCondition:    None
 *
 * Input:           DataByte - Byte to be put at Write pointer
 *
 * Output:          write pointer and used_bytes updated.
 *
 * Side Effects:    
 *
 * Note:            None
 *****************************************************************************/
void WriteByteDownloadBuffer(unsigned char DataByte)
{
        uc_download_buffer[downloadbuf_mgmt.write_index++] = DataByte;
        if (downloadbuf_mgmt.write_index >= DOWNLOAD_SIZE) // handle index wrap
        {
            downloadbuf_mgmt.write_index = 0;
        }
        downloadbuf_mgmt.used_bytes++;  // used another byte.
}

/******************************************************************************
 * Function:        void ICDSlave_transmit (unsigned char TransmitByte)
 *
 * Overview:        Handles handshake and slave transmission of a byte to debug exec.
 *
 * PreCondition:    None
 *
 * Input:           TransmitByte - byte to be clocked out by DE
 *
 * Output:          Pk2Status.ICDTimeOut if eight bits not clocked out in 500ms
 *
 * Side Effects:    Shuts off interrupts during execution - leaves ICDDATA pin as output.
 *
 * Note:            None
 *****************************************************************************/
void ICDSlave_transmit (unsigned char TransmitByte)
{
    //VppVddADCTMR1_Stop();           // Stop ADC - no interrupts
    INTCONbits.GIE = 0;             // disable all interrupts
    T1CONbits.TMR1ON = 0;           // pause ADC timer
    TMR0H = 0xA4;                   // 500ms till timer rolls over
    TMR0L = 0x73;
    INTCONbits.T0IF = 0;            // clear int flag
    T0CONbits.TMR0ON = 1;           // start timer

    while (!ICSPCLK_in && !INTCONbits.T0IF); // wait for clock high handshake or timeout
    if (!INTCONbits.T0IF)           // if timeout, skip this
    {
        ICSPDAT_out = 0;            // data output low
        tris_ICSPDAT = 0;
        Delay100TCYx(96);           // delay 800us to ensure DE sees it at slow speed.
        ICSPDAT_out = 1;            // data output high
        while (ICSPCLK_in && !INTCONbits.T0IF); // wait for clock to fall or timeout
        // leave data output
        if (!INTCONbits.T0IF)           // if timeout, skip this  
        {
            asm_temp1 = TransmitByte;
            _asm
             TXBit7H:
                btfsc   INTCON, 3, 0    // test for timeout
                bra     TXTimeoutExit 
                btfss   PORTA, 3, 0     // wait for clock to go high
                bra     TXBit7H
                bcf     LATA, 2, 0      // data low unless bit high
                btfsc   ASM_TEMP1_RAM, 7, 0
                bsf     LATA, 2, 0      // data high
             TXBit7L:
                btfsc   PORTA, 3, 0     // wait for clock to go low
                bra     TXBit7L

             TXBit6H:
                btfsc   INTCON, 3, 0    // test for timeout
                bra     TXTimeoutExit 
                btfss   PORTA, 3, 0     // wait for clock to go high
                bra     TXBit6H
                bcf     LATA, 2, 0      // data low unless bit high
                btfsc   ASM_TEMP1_RAM, 6, 0
                bsf     LATA, 2, 0      // data high
             TXBit6L:
                btfsc   PORTA, 3, 0     // wait for clock to go low
                bra     TXBit6L

             TXBit5H:
                btfsc   INTCON, 3, 0    // test for timeout
                bra     TXTimeoutExit 
                btfss   PORTA, 3, 0     // wait for clock to go high
                bra     TXBit5H
                bcf     LATA, 2, 0      // data low unless bit high
                btfsc   ASM_TEMP1_RAM, 5, 0
                bsf     LATA, 2, 0      // data high
             TXBit5L:
                btfsc   PORTA, 3, 0     // wait for clock to go low
                bra     TXBit5L

             TXBit4H:
                btfsc   INTCON, 3, 0    // test for timeout
                bra     TXTimeoutExit 
                btfss   PORTA, 3, 0     // wait for clock to go high
                bra     TXBit4H
                bcf     LATA, 2, 0      // data low unless bit high
                btfsc   ASM_TEMP1_RAM, 4, 0
                bsf     LATA, 2, 0      // data high
             TXBit4L:
                btfsc   PORTA, 3, 0     // wait for clock to go low
                bra     TXBit4L

             TXBit3H:
                btfsc   INTCON, 3, 0    // test for timeout
                bra     TXTimeoutExit 
                btfss   PORTA, 3, 0     // wait for clock to go high
                bra     TXBit3H
                bcf     LATA, 2, 0      // data low unless bit high
                btfsc   ASM_TEMP1_RAM, 3, 0
                bsf     LATA, 2, 0      // data high
             TXBit3L:
                btfsc   PORTA, 3, 0     // wait for clock to go low
                bra     TXBit3L

             TXBit2H:
                btfsc   INTCON, 3, 0    // test for timeout
                bra     TXTimeoutExit 
                btfss   PORTA, 3, 0     // wait for clock to go high
                bra     TXBit2H
                bcf     LATA, 2, 0      // data low unless bit high
                btfsc   ASM_TEMP1_RAM, 2, 0
                bsf     LATA, 2, 0      // data high
             TXBit2L:
                btfsc   PORTA, 3, 0     // wait for clock to go low
                bra     TXBit2L

             TXBit1H:
                btfsc   INTCON, 3, 0    // test for timeout
                bra     TXTimeoutExit 
                btfss   PORTA, 3, 0     // wait for clock to go high
                bra     TXBit1H
                bcf     LATA, 2, 0      // data low unless bit high
                btfsc   ASM_TEMP1_RAM, 1, 0
                bsf     LATA, 2, 0      // data high
             TXBit1L:
                btfsc   PORTA, 3, 0     // wait for clock to go low
                bra     TXBit1L

             TXBit0H:
                btfsc   INTCON, 3, 0    // test for timeout
                bra     TXTimeoutExit 
                btfss   PORTA, 3, 0     // wait for clock to go high
                bra     TXBit0H
                bcf     LATA, 2, 0      // data low unless bit high
                btfsc   ASM_TEMP1_RAM, 0, 0
                bsf     LATA, 2, 0      // data high
             TXBit0L:
                btfsc   PORTA, 3, 0     // wait for clock to go low
                bra     TXBit0L

             TXTimeoutExit:
                nop
            _endasm   
        }      
    }
    if (INTCONbits.T0IF)
        Pk2Status.ICDTimeOut = 1;   // Timeout error.
    T0CONbits.TMR0ON = 0;           // shut off timer
    INTCONbits.GIE = 1;             // re-enable all interrupts
    T1CONbits.TMR1ON = 1;           // restart ADC timer
    //VppVddADCTmr1_Start();          // restore ADC operation.
    //tris_ICSPDAT = 1;               // data back to input
            // - Leave Data as output or slow target clocks may miss last bit.

    return;
} // end void ICDSlave_transmit (unsigned char TransmitByte)


/******************************************************************************
 * Function:        unsigned char ICDSlave_Receive (void)
 *
 * Overview:        Handles handshake and slave reception of a byte from debug exec.
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          Pk2Status.ICDTimeOut if eight bits not received in 500ms
 *                  Returns byte value received.
 *
 * Side Effects:    Shuts off interrupts during execution
 *
 * Note:            None
 *****************************************************************************/
unsigned char ICDSlave_Receive (void)
{
    //VppVddADCTMR1_Stop();           // Stop ADC - no interrupts
    T1CONbits.TMR1ON = 0;           // pause ADC timer
    INTCONbits.GIE = 0;             // disable all interrupts
    TMR0H = 0xA4;                   // 500ms till timer rolls over
    TMR0L = 0x73;
    INTCONbits.T0IF = 0;            // clear int flag
    T0CONbits.TMR0ON = 1;           // start timer

    while (!ICSPCLK_in && !INTCONbits.T0IF); // wait for clock high handshake or timeout
    if (!INTCONbits.T0IF)           // if timeout, skip this
    {
        ICSPDAT_out = 0;            // data output low
        tris_ICSPDAT = 0;
        Delay100TCYx(96);           // delay 800us to ensure DE sees it at slow speed.
        ICSPDAT_out = 1;            // data output high
        while (ICSPCLK_in && !INTCONbits.T0IF); // wait for clock to fall or timeout
        tris_ICSPDAT = 1;           // data back to input
        if (!INTCONbits.T0IF)           // if timeout, skip this  
        {
            asm_temp1 = 0;
            _asm
             RXBit7H:
                btfsc   INTCON, 3, 0    // test for timeout
                bra     RXTimeoutExit 
                btfss   PORTA, 3, 0     // wait for clock to go high
                bra     RXBit7H
             RXBit7L:
                btfsc   PORTA, 3, 0     // wait for clock to go low
                bra     RXBit7L
                btfsc   PORTA, 2, 0     // test data pin
                bsf     ASM_TEMP1_RAM, 7, 0 // set bit if data high

             RXBit6H:
                btfsc   INTCON, 3, 0    // test for timeout
                bra     RXTimeoutExit 
                btfss   PORTA, 3, 0     // wait for clock to go high
                bra     RXBit6H
             RXBit6L:
                btfsc   PORTA, 3, 0     // wait for clock to go low
                bra     RXBit6L
                btfsc   PORTA, 2, 0     // test data pin
                bsf     ASM_TEMP1_RAM, 6, 0 // set bit if data high

             RXBit5H:
                btfsc   INTCON, 3, 0    // test for timeout
                bra     RXTimeoutExit 
                btfss   PORTA, 3, 0     // wait for clock to go high
                bra     RXBit5H
             RXBit5L:
                btfsc   PORTA, 3, 0     // wait for clock to go low
                bra     RXBit5L
                btfsc   PORTA, 2, 0     // test data pin
                bsf     ASM_TEMP1_RAM, 5, 0 // set bit if data high

             RXBit4H:
                btfsc   INTCON, 3, 0    // test for timeout
                bra     RXTimeoutExit 
                btfss   PORTA, 3, 0     // wait for clock to go high
                bra     RXBit4H
             RXBit4L:
                btfsc   PORTA, 3, 0     // wait for clock to go low
                bra     RXBit4L
                btfsc   PORTA, 2, 0     // test data pin
                bsf     ASM_TEMP1_RAM, 4, 0 // set bit if data high

             RXBit3H:
                btfsc   INTCON, 3, 0    // test for timeout
                bra     RXTimeoutExit 
                btfss   PORTA, 3, 0     // wait for clock to go high
                bra     RXBit3H
             RXBit3L:
                btfsc   PORTA, 3, 0     // wait for clock to go low
                bra     RXBit3L
                btfsc   PORTA, 2, 0     // test data pin
                bsf     ASM_TEMP1_RAM, 3, 0 // set bit if data high

             RXBit2H:
                btfsc   INTCON, 3, 0    // test for timeout
                bra     RXTimeoutExit 
                btfss   PORTA, 3, 0     // wait for clock to go high
                bra     RXBit2H
             RXBit2L:
                btfsc   PORTA, 3, 0     // wait for clock to go low
                bra     RXBit2L
                btfsc   PORTA, 2, 0     // test data pin
                bsf     ASM_TEMP1_RAM, 2, 0 // set bit if data high

             RXBit1H:
                btfsc   INTCON, 3, 0    // test for timeout
                bra     RXTimeoutExit 
                btfss   PORTA, 3, 0     // wait for clock to go high
                bra     RXBit1H
             RXBit1L:
                btfsc   PORTA, 3, 0     // wait for clock to go low
                bra     RXBit1L
                btfsc   PORTA, 2, 0     // test data pin
                bsf     ASM_TEMP1_RAM, 1, 0 // set bit if data high

             RXBit0H:
                btfsc   INTCON, 3, 0    // test for timeout
                bra     RXTimeoutExit 
                btfss   PORTA, 3, 0     // wait for clock to go high
                bra     RXBit0H
             RXBit0L:
                btfsc   PORTA, 3, 0     // wait for clock to go low
                bra     RXBit0L
                btfsc   PORTA, 2, 0     // test data pin
                bsf     ASM_TEMP1_RAM, 0, 0 // set bit if data high

             RXTimeoutExit:
                nop
            _endasm   
        }      
    }

    // Set the data line high to try to prevent floating around being seen as a handshake
    ICSPDAT_out = 1;            // data output high
    tris_ICSPDAT = 0;


    if (INTCONbits.T0IF)
        Pk2Status.ICDTimeOut = 1;   // Timeout error.
    T0CONbits.TMR0ON = 0;           // shut off timer
    INTCONbits.GIE = 1;             // re-enable all interrupts
    T1CONbits.TMR1ON = 1;           // restart ADC timer
    //VppVddADCTmr1_Start();          // restore ADC operation.

    return asm_temp1;
} // end unsigned char ICDSlave_Receive (void)


/******************************************************************************
 * Function:        void LongDelay(unsigned char count)
 *
 * Overview:        Delays in increments of 5.46ms * count.
 * 
 * PreCondition:    None
 *
 * Input:           count - units of delay (5.46ms each)
 *
 * Output:          None.
 *
 * Side Effects:    Uses Timer0
 *
 * Note:            Uses Timer0 to allow more accurate timing with interrupts.
 *                  
 *****************************************************************************/
void LongDelay(unsigned char count)
{
    INTCONbits.T0IF = 0; //clear flag
    TMR0H = 0 - count;   
    
    TMR0L = 0;
    T0CONbits.TMR0ON = 1;
    while (INTCONbits.T0IF == 0)    // wait for rollover
    {
    }
    T0CONbits.TMR0ON = 0;   // shut off  
}


/******************************************************************************
 * Function:        void ShortDelay(unsigned char count)
 *
 * Overview:        Delays in increments of 42.7us * count.
 * 
 * PreCondition:    None
 *
 * Input:           count - units of delay (42.7us each)
 *
 * Output:          None.
 *
 * Side Effects:    Uses Timer0
 *
 * Note:            Uses Timer0 to allow more accurate timing with interrupts.
 *                  
 *****************************************************************************/
void ShortDelay(unsigned char count)
{
    INTCONbits.T0IF = 0; //clear flag
    TMR0H = 0xFF;        // rolls over with when lower byte does

    // an inherent delay of about 12us exists between script entities.
    // round off with fudge factor to get about 21uS inherent delay
    Delay10TCYx(8);    

    if (count == 1)
    {
        return;     // just inherent delay + fudge factor. 
    }
    else
    {
        count--;        
    }        
    
    TMR0L = 0 - count;
    T0CONbits.TMR0ON = 1;
    while (INTCONbits.T0IF == 0)    // wait for rollover
    {
    }
    T0CONbits.TMR0ON = 0;   // shut off            
}

/******************************************************************************
 * Function:        unsigned char ShiftBitsInPIC24(unsigned char numbits)
 *
 * Overview:        Shifts in up to a byte of data.  Shifts in LSB first.
 *                  If less than 8 bits, return byte is right justified.
 *                  If full, sets error Pk2Status.UpLoadFull
 *                  Data is latched on Rising Edge of clock
 * PreCondition:    None
 *
 * Input:           numbits - # bits to shift in (max 8)
 *
 * Output:          returns bits right-justified.
 *
 * Side Effects:    Advances upload buffer write pointer, if err Pk2Status.StatusHigh != 0
 *
 * Note:            Assumes ICSPCLK is output.  Sets ICSPDAT to input then restores
 *                  previous state.
 *****************************************************************************/
unsigned char ShiftBitsInPIC24(unsigned char numbits)
{
	//BOOL interrupts_on = 0;
    char i;

	tris_ICSPDAT = 1;       // set input

	//if (INTCONbits.GIE == 1)
	//	interrupts_on = 1;
	//INTCONbits.GIE = 0;			// uninterruptable routine

	asm_temp1 = 0;              // bits get shifted in here.
    asm_temp2 = numbits;

    if (icsp_baud < 2)
    {
    	_asm
           READ8LOOPF:
    		btfsc	PORTA, 2, 0
    		bsf		ASM_TEMP1_RAM, 0, 0     // set bit 0 so gets shifted into MSB
    		bsf		LATA, 3, 0
            nop
            nop
            nop
            bcf     LATA, 3, 0
            nop
            rrncf   ASM_TEMP1_RAM, 1, 0
            decfsz  ASM_TEMP2_RAM, 1, 0
            bra     READ8LOOPF
    	_endasm
    }
    else
    {
        asm_temp3 = icsp_baud - 1;
    	_asm
           READLOOPS:
            movf    ASM_TEMP3_RAM, 0, 0    // delay 6 cycles per count
    		btfsc	PORTA, 2, 0
    		bsf		ASM_TEMP1_RAM, 0, 0     // set bit 0 so gets shifted into MSB
    		bsf		LATA, 3, 0
            nop
            nop
           RDELAYLOOPHI:
            nop
            nop
            nop
            decfsz  WREG, 1, 0
            bra     RDELAYLOOPHI
            bcf     LATA, 3, 0
            movf    ASM_TEMP3_RAM, 0, 0    // delay 6 cycles per count
           RDELAYLOOPLO:
            nop
            nop
            nop
            decfsz  WREG, 1, 0
            bra     RDELAYLOOPLO
            rrncf   ASM_TEMP1_RAM, 1, 0
            decfsz  ASM_TEMP2_RAM, 1, 0
            bra     READLOOPS
    	_endasm
    }


    asm_temp1 >>= (8 - numbits);    // right justify

	if (!(icsp_pins & 0x02))
			tris_ICSPDAT = 0;

	//if (interrupts_on == 1)		// turn interrupts back on if enabled.	
		//INTCONbits.GIE = 1;

    return asm_temp1;
}

/******************************************************************************
 * Function:        unsigned char ShiftBitsInICSP(unsigned char numbits)
 *
 * Overview:        Shifts in up to a byte of data.  Shifts in LSB first.
 *                  If less than 8 bits, return byte is right justified.
 *                  If full, sets error Pk2Status.UpLoadFull
 * PreCondition:    None
 *
 * Input:           numbits - # bits to shift in (max 8)
 *
 * Output:          returns bits right-justified.
 *
 * Side Effects:    Advances upload buffer write pointer, if err Pk2Status.StatusHigh != 0
 *
 * Note:            Assumes ICSPCLK is output.  Sets ICSPDAT to input then restores
 *                  previous state.
 *****************************************************************************/
unsigned char ShiftBitsInICSP(unsigned char numbits)
{
	//BOOL interrupts_on = 0;
    char i;

	tris_ICSPDAT = 1;       // set input

	//if (INTCONbits.GIE == 1)
	//	interrupts_on = 1;
	//INTCONbits.GIE = 0;			// uninterruptable routine

	asm_temp1 = 0;              // bits get shifted in here.
    asm_temp2 = numbits;

    if (icsp_baud < 2)
    {
    	_asm
           READ8LOOPF:
    		bsf		LATA, 3, 0
            nop
            nop
            nop
    		btfsc	PORTA, 2, 0
    		bsf		ASM_TEMP1_RAM, 0, 0     // set bit 0 so gets shifted into MSB
            bcf     LATA, 3, 0
            nop
            rrncf   ASM_TEMP1_RAM, 1, 0
            decfsz  ASM_TEMP2_RAM, 1, 0
            bra     READ8LOOPF
    	_endasm
    }
    else
    {
        asm_temp3 = icsp_baud - 1;
    	_asm
           READLOOPS:
    		bsf		LATA, 3, 0
            nop
            nop
            movf    ASM_TEMP3_RAM, 0, 0    // delay 6 cycles per count
           RDELAYLOOPHI:
            nop
            nop
            nop
            decfsz  WREG, 1, 0
            bra     RDELAYLOOPHI
    		btfsc	PORTA, 2, 0
    		bsf		ASM_TEMP1_RAM, 0, 0     // set bit 0 so gets shifted into MSB
            bcf     LATA, 3, 0
            movf    ASM_TEMP3_RAM, 0, 0    // delay 6 cycles per count
           RDELAYLOOPLO:
            nop
            nop
            nop
            decfsz  WREG, 1, 0
            bra     RDELAYLOOPLO
            rrncf   ASM_TEMP1_RAM, 1, 0
            decfsz  ASM_TEMP2_RAM, 1, 0
            bra     READLOOPS
    	_endasm
    }


    asm_temp1 >>= (8 - numbits);    // right justify

	if (!(icsp_pins & 0x02))
			tris_ICSPDAT = 0;

	//if (interrupts_on == 1)		// turn interrupts back on if enabled.	
		//INTCONbits.GIE = 1;

    return asm_temp1;
}

/******************************************************************************
 * Function:        void WriteUploadBuffer(unsigned char byte2write)
 *
 * Overview:        Attempts to write a byte to the upload buffer.
 *                  If full, sets error Pk2Status.UpLoadFull
 * PreCondition:    None
 *
 * Input:           byte2write - byte to be written
 *
 * Output:          uc_upload_buffer - byte written to end of buffer.
 *
 * Side Effects:    Advances download buffer write pointer, if err Pk2Status.StatusHigh != 0
 *
 * Note:            None
 *****************************************************************************/
void WriteUploadBuffer(unsigned char byte2write)
{

    if ((uploadbuf_mgmt.used_bytes + 1) > UPLOAD_SIZE)     // not enough room for data
    {
        Pk2Status.UpLoadFull = 1;
        return;
    }

    uc_upload_buffer[uploadbuf_mgmt.write_index++] = byte2write;
    if (uploadbuf_mgmt.write_index >= UPLOAD_SIZE) // handle index wrap
    {
        uploadbuf_mgmt.write_index = 0;
    }
    uploadbuf_mgmt.used_bytes++;  // used another byte.
}

/******************************************************************************
 * Function:        unsigned char ReadDownloadBuffer(void)
 *
 * Overview:        Attempts to pull a byte from the Download Buffer.
 *                  If empty, sets error Pk2Status.DownloadEmpty
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          Returns byte from top of buffer.
 *
 * Side Effects:    Advances download buffer read pointer, if err Pk2Status.StatusHigh != 0
 *
 * Note:            None
 *****************************************************************************/
unsigned char ReadDownloadBuffer(void)
{
    unsigned char readbyte;

    if (downloadbuf_mgmt.used_bytes == 0)
    {
        Pk2Status.DownloadEmpty = 1;
        return 0;
    } 

    readbyte = uc_download_buffer[downloadbuf_mgmt.read_index++];
    downloadbuf_mgmt.used_bytes--;        // just removed a byte.
    if (downloadbuf_mgmt.read_index >= DOWNLOAD_SIZE)   // circular buffer - handle wrap.
        downloadbuf_mgmt.read_index = 0; 

    return  readbyte;  

}

/******************************************************************************
 * Function:        void ShiftBitsOutICSP(unsigned char outputbyte, char numbits)
 *
 * Overview:        Shifts the given # bits out on the ICSP pins 
 *
 * PreCondition:    None
 *
 * Input:           outputbyte - byte to be shifted out LSB first
 *
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Note:            Assumes ICSP pins are already set to outputs.
 *****************************************************************************/
void ShiftBitsOutICSP(unsigned char outputbyte, char numbits)
{
	//BOOL interrupts_on = 0;
    char i;

	//if (INTCONbits.GIE == 1)
	//	interrupts_on = 1;
	//INTCONbits.GIE = 0;			// uninterruptable routine

	asm_temp1 = outputbyte;
    asm_temp2 = numbits;

    if (icsp_baud < 2)
    {
    	_asm
           WRITE8LOOPF:
    		btfss	ASM_TEMP1_RAM, 0, 0
    		bcf		LATA, 2, 0 
    		btfsc	ASM_TEMP1_RAM, 0, 0
    		bsf		LATA, 2, 0 
            nop
    		bsf		LATA, 3, 0
			nop
            bcf     LATA, 3, 0
            rrncf   ASM_TEMP1_RAM, 1, 0
            decfsz  ASM_TEMP2_RAM, 1, 0
            bra     WRITE8LOOPF
    	_endasm
    }
    else
    {
        asm_temp3 = icsp_baud - 1;
    	_asm
           WRITE8LOOPS:
    		btfss	ASM_TEMP1_RAM, 0, 0
    		bcf		LATA, 2, 0 
    		btfsc	ASM_TEMP1_RAM, 0, 0
    		bsf		LATA, 2, 0 
            movf    ASM_TEMP3_RAM, 0, 0    // delay 6 cycles per count
           DELAYLOOPHI:
            nop
            nop
            nop
            decfsz  WREG, 1, 0
            bra     DELAYLOOPHI
    		bsf		LATA, 3, 0
            movf    ASM_TEMP3_RAM, 0, 0    // delay 6 cycles per count
           DELAYLOOPLO:
            nop
            nop
            nop
            decfsz  WREG, 1, 0
            bra     DELAYLOOPLO
            bcf     LATA, 3, 0
            rrncf   ASM_TEMP1_RAM, 1, 0
            decfsz  ASM_TEMP2_RAM, 1, 0
            bra     WRITE8LOOPS
    	_endasm
    }

	//if (interrupts_on == 1)		// turn interrupts back on if enabled.	
	//	INTCONbits.GIE = 1;

}


/******************************************************************************
 * Function:        void ShiftBitsOutICSPHold(unsigned char outputbyte, char numbits)
 *
 * Overview:        Shifts the given # bits out on the ICSP pins 
 *                  Differs from ShiftBitsOutICSP in that the instead of
 *                  Setting data, delay, clock high, delay, clock low
 *                  This routine works as
 *                  Setting data, clock high, delay, clock low, delay
 *
 * PreCondition:    None
 *
 * Input:           outputbyte - byte to be shifted out LSB first
 *
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Note:            Assumes ICSP pins are already set to outputs.
 *****************************************************************************/
void ShiftBitsOutICSPHold(unsigned char outputbyte, char numbits)
{
	//BOOL interrupts_on = 0;
    char i;

	//if (INTCONbits.GIE == 1)
	//	interrupts_on = 1;
	//INTCONbits.GIE = 0;			// uninterruptable routine

	asm_temp1 = outputbyte;
    asm_temp2 = numbits;

    if (icsp_baud < 2)
    {
    	_asm
           WRITE8LOOPF:
    		btfss	ASM_TEMP1_RAM, 0, 0
    		bcf		LATA, 2, 0 
    		btfsc	ASM_TEMP1_RAM, 0, 0
    		bsf		LATA, 2, 0 
    		bsf		LATA, 3, 0
			nop
            bcf     LATA, 3, 0
			nop
            rrncf   ASM_TEMP1_RAM, 1, 0
            decfsz  ASM_TEMP2_RAM, 1, 0
            bra     WRITE8LOOPF
    	_endasm
    }
    else
    {
        asm_temp3 = icsp_baud - 1;
    	_asm
           WRITE8LOOPS:
    		btfss	ASM_TEMP1_RAM, 0, 0
    		bcf		LATA, 2, 0 
    		btfsc	ASM_TEMP1_RAM, 0, 0
    		bsf		LATA, 2, 0 
            movf    ASM_TEMP3_RAM, 0, 0    // delay 6 cycles per count
            bsf     LATA, 3, 0
           DELAYLOOPHI:
            nop
            nop
            nop
            decfsz  WREG, 1, 0
            bra     DELAYLOOPHI
    		bcf		LATA, 3, 0
            movf    ASM_TEMP3_RAM, 0, 0    // delay 6 cycles per count
           DELAYLOOPLO:
            nop
            nop
            nop
            decfsz  WREG, 1, 0
            bra     DELAYLOOPLO
            rrncf   ASM_TEMP1_RAM, 1, 0
            decfsz  ASM_TEMP2_RAM, 1, 0
            bra     WRITE8LOOPS
    	_endasm
    }

	//if (interrupts_on == 1)		// turn interrupts back on if enabled.	
	//	INTCONbits.GIE = 1;

}


/******************************************************************************
 * Function:        void SetICSP_PinStates(unsigned char icsp_byte)
 *
 * Overview:        Sets the value and direction of the ICSP pins.
 *
 * PreCondition:    None
 *
 * Input:           icsp_byte - byte formated
 *   					<7 – 4> unused
 *   					<3> PGD logic level
 *   					<2> PGC logic level
 *   					<1> 1= PGD input, 0= output
 *    					<0> 1= PGC input, 0= output
 *
 *
 * Output:          Affects bits in TRISA and LATA
 *
 * Side Effects:    None
 *
 * Note:            None
 *****************************************************************************/
void SetICSP_PinStates(unsigned char icsp_byte)
{
				// set ISCPCLK latch
				if (icsp_byte & 0x04)
					ICSPCLK_out = 1;	
				else
					ICSPCLK_out = 0;
				// set ISCDAT latch
				if (icsp_byte & 0x08)
					ICSPDAT_out = 1;	
				else
					ICSPDAT_out = 0;

				// set ISCPCLK direction
				if (icsp_byte & 0x01)
					tris_ICSPCLK = 1;	
				else
					tris_ICSPCLK = 0;
				// set ISCDAT direction
				if (icsp_byte & 0x02)
					tris_ICSPDAT = 1;	
				else
					tris_ICSPDAT = 0;
}

/******************************************************************************
 * Function:        unsigned char GetICSP_PinStates(void)
 *
 * Overview:        Gets the values of the ICSP pins.
 *
 * PreCondition:    None
 *
 * Input:           icsp_byte - byte formated
 *   					<7 – 4> unused
 *   					<3> PGD logic level
 *   					<2> PGC logic level
 *
 *
 * Output:          returns a byte with bits:
 *                      <1> PGD state
 *    					<0> PGC state
 *
 * Side Effects:    None
 *
 * Note:            None
 *****************************************************************************/
unsigned char GetICSP_PinStates(void)
{
    unsigned char states = 0;

    if (ICSPDAT_in == 1)
    {
        states |= 0x02;
    }
    if (ICSPCLK_in == 1)
    {
        states |= 0x01;
    }

    return states;
}

/******************************************************************************
 * Function:        void SetAUX_PinState(unsigned char aux_byte)
 *
 * Overview:        Sets the value and direction of the AUX pin.
 *
 * PreCondition:    None
 *
 * Input:           aux_byte - byte formated
 *   					<7 – 2> unused
 *   					<1> AUX logic level
 *    					<0> 1= AUX input, 0= output
 *
 *
 * Output:          Affects bits in TRISA and LATA
 *
 * Side Effects:    None
 *
 * Note:            None
 *****************************************************************************/
void SetAUX_PinState(unsigned char aux_byte)
{
				// set AUX latch
				if (aux_byte & 0x02)
					AUX = 1;	
				else
					AUX = 0;

				// set AUX direction
				if (aux_byte & 0x01)
					tris_AUX = 1;	
				else
					tris_AUX = 0;
}

/******************************************************************************
 * Function:        unsigned char GetAUX_PinState(void)
 *
 * Overview:        Gets the values of the ICSP pins.
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          returns a byte with bits:
 *    					<1> AUX state
 *
 * Side Effects:    None
 *
 * Note:            None
 *****************************************************************************/
unsigned char GetAUX_PinState(void)
{
    unsigned char states = 0;

    if (AUX_in == 1)
    {
        states |= 0x01;
    }

    return states;
}


/******************************************************************************
 * Function:        void I2C_Start(void)
 *
 * Overview:        Creates I2C Start condition with PGC = SCL and AUX = SDA.
 *
 * PreCondition:    PGC = output, AUX = input
 *
 * Input:           None
 *
 * Output:          Affects bits in TRISA and LATA
 *
 * Side Effects:    None
 *
 * Note:            None
 *****************************************************************************/
void I2C_Start(void)
{
    ICSPCLK_out = 1;            // SCL high
    Delay10TCYx(6);             // delay 5us
    AUX = 0;                    // ensure LAT bit is zero
    tris_AUX = 0;               // SDA low
    Delay10TCYx(6);             // delay 5us
    ICSPCLK_out = 0;            // SCL Low  
    Delay10TCYx(6);             // delay 5us
    tris_AUX = 1;               // SDA released
}

/******************************************************************************
 * Function:        void I2C_Stop(void)
 *
 * Overview:        Creates I2C Stop condition with PGC = SCL and AUX = SDA.
 *
 * PreCondition:    PGC = output, AUX = input
 *
 * Input:           None
 *
 * Output:          Affects bits in TRISA and LATA
 *
 * Side Effects:    None
 *
 * Note:            None
 *****************************************************************************/
void I2C_Stop(void)
{
    ICSPCLK_out = 0;            // SCL low
    Delay10TCYx(6);             // delay 5us
    AUX = 0;                    // ensure LAT bit is zero
    tris_AUX = 0;               // SDA low
    Delay10TCYx(6);             // delay 5us
    ICSPCLK_out = 1;            // SCL high 
    Delay10TCYx(6);             // delay 5us
    tris_AUX = 1;               // SDA released
}

/******************************************************************************
 * Function:        void I2C_Write(unsigned char outputbyte)
 *
 * Overview:        Clocks out a byte with PGC = SCL and AUX = SDA.
 *                  Checks for ACK
 *
 * PreCondition:    PGC = output, AUX = input
 *
 * Input:           outputbyte = byte to be written MSB first
 *
 * Output:          Affects bits in TRISA and LATA
 *                  Pk2Status.ICDTimeOut set if NACK received
 *
 * Side Effects:    None
 *
 * Note:            None
 *****************************************************************************/
void I2C_Write(unsigned char outputbyte)
{
	asm_temp1 = outputbyte;
    asm_temp2 = 8;

    AUX = 0;                    // ensure LAT bit is zero

    if (icsp_baud < 2)
    { // 400kHz
    	_asm
           WRITE8LOOPF:
    		btfss	ASM_TEMP1_RAM, 7, 0
    		bcf		TRISA, 4, 0 
    		btfsc	ASM_TEMP1_RAM, 7, 0
    		bsf		TRISA, 4, 0 
            nop
            nop
            nop   
            nop
            nop
            nop                           // 583ns setup time
    		bsf		LATA, 3, 0            // clock high time 833ns (10 inst)
			nop
            nop
            nop
            nop
            nop
            nop
            nop
            nop
            nop
            bcf     LATA, 3, 0            // clock low time 1667ns (20 inst)
			nop
            nop
            nop
            nop
            nop
            rlncf   ASM_TEMP1_RAM, 1, 0
            decfsz  ASM_TEMP2_RAM, 1, 0
            bra     WRITE8LOOPF
			bsf		TRISA, 4, 0           // release SDA for ACK
            nop
            nop
            nop
            nop
            nop
            nop
            nop
            nop
            nop
            nop                          
    		bsf		LATA, 3, 0            // ACK clock
			nop
            nop
            nop
            nop
            nop
            nop
            nop
            btfsc   PORTA, 4, 0
            bsf     ASM_TEMP2_RAM, 0, 0   // bit is ACK value. 
            bcf     LATA, 3, 0
                       
    	_endasm
    }
    else
    { // 100kHz at icsp_baud = 4
        asm_temp3 = (icsp_baud << 1) - 1;
    	_asm
           WRITE8LOOPS:
    		btfss	ASM_TEMP1_RAM, 7, 0
    		bcf		TRISA, 4, 0 
    		btfsc	ASM_TEMP1_RAM, 7, 0
    		bsf		TRISA, 4, 0 
            movf    ASM_TEMP3_RAM, 0, 0    // delay 10 cycles per count
           DELAYLOOPLO:
            nop
            nop   
            nop
			nop
            nop
            nop
            nop
            decfsz  WREG, 1, 0
            bra     DELAYLOOPLO
    		bsf		LATA, 3, 0            
            movf    ASM_TEMP3_RAM, 0, 0    // delay 5 cycles per count
            nop
            nop
            nop
            nop
           DELAYLOOPHI:
            nop
            nop
            decfsz  WREG, 1, 0
            bra     DELAYLOOPHI  
            bcf     LATA, 3, 0            
            nop
            rlncf   ASM_TEMP1_RAM, 1, 0
            decfsz  ASM_TEMP2_RAM, 1, 0
            bra     WRITE8LOOPS
			bsf		TRISA, 4, 0            // release SDA for ACK
            movf    ASM_TEMP3_RAM, 0, 0    // delay 10 cycles per count
            nop
            nop   
            nop
			nop
           DELAYLOOPACKLO:
            nop
            nop   
            nop
			nop
            nop
            nop
            nop
            decfsz  WREG, 1, 0
            bra     DELAYLOOPACKLO
    		bsf		LATA, 3, 0             // ACK clock
            movf    ASM_TEMP3_RAM, 0, 0    // delay 5 cycles per count
            nop
            nop
           DELAYLOOPACKHI:
            nop
            nop
            decfsz  WREG, 1, 0
            bra     DELAYLOOPACKHI  
            btfsc   PORTA, 4, 0
            bsf     ASM_TEMP2_RAM, 0, 0    // bit is ACK value. 
            bcf     LATA, 3, 0             // ACK clock low

    	_endasm
    }

    if (asm_temp2 > 0)
    {// NACK received
        Pk2Status.ICDTimeOut = 1;
    }

}


/******************************************************************************
 * Function:        unsigned char I2C_Read(unsigned char giveack)
 *
 * Overview:        Clocks in a byte with PGC = SCL and AUX = SDA.
 *                  Provides and ACK for the byte if giveack is 0.
 *
 * PreCondition:    PGC = output, AUX = input
 *
 * Input:           giveack - ACK the byte if 0, else NACK
 *
 * Output:          Affects bits in TRISA and LATA
 *                  returns byte read MSB first.
 *
 * Side Effects:    None
 *
 * Note:            None
 *****************************************************************************/
unsigned char I2C_Read(unsigned char giveack)
{
	asm_temp1 = 0;
    asm_temp2 = 8;
    asm_temp3 = 1;  // default speed setting for ACK

    tris_AUX = 1;                         // ensure TRIS bit is input

    if (icsp_baud < 2)
    { // 400kHz
    	_asm
           WRITE8LOOPF:
    		rlncf   ASM_TEMP1_RAM, 1, 0
    		nop
    		nop
    		nop
            nop
            nop
            nop   
            nop
            nop
            nop                           // 583ns setup time
    		bsf		LATA, 3, 0            // clock high time 833ns (10 inst)
			nop
            nop
            nop
            nop
            nop
            nop
            nop
            btfsc   PORTA, 4, 0           // Get bit
            bsf     ASM_TEMP1_RAM, 0, 0
            bcf     LATA, 3, 0            // clock low time 1667ns (20 inst)
			nop
            nop
            nop
            nop
            nop
            nop
            decfsz  ASM_TEMP2_RAM, 1, 0
            bra     WRITE8LOOPF                       
    	_endasm
    }
    else
    { // 100kHz at icsp_baud = 4
        asm_temp3 = (icsp_baud << 1) - 1;
    	_asm
           WRITE8LOOPS:
    		rlncf   ASM_TEMP1_RAM, 1, 0
    		nop 
    		nop
    		nop
            movf    ASM_TEMP3_RAM, 0, 0    // delay 10 cycles per count
           DELAYLOOPLO:
            nop
            nop   
            nop
			nop
            nop
            nop
            nop
            decfsz  WREG, 1, 0
            bra     DELAYLOOPLO
    		bsf		LATA, 3, 0            
            movf    ASM_TEMP3_RAM, 0, 0    // delay 5 cycles per count
            nop
            nop
           DELAYLOOPHI:
            nop
            nop
            decfsz  WREG, 1, 0
            bra     DELAYLOOPHI  
            btfsc   PORTA, 4, 0           // Get bit
            bsf     ASM_TEMP1_RAM, 0, 0
            bcf     LATA, 3, 0            
            nop
            nop
            decfsz  ASM_TEMP2_RAM, 1, 0
            bra     WRITE8LOOPS
			nop
            movf    ASM_TEMP3_RAM, 0, 0    // delay 10 cycles per count
            nop
            nop   
            nop
			nop
           DELAYLOOPACKLO:
            nop
            nop   
            nop
			nop
            nop
            nop
            nop
            decfsz  WREG, 1, 0
            bra     DELAYLOOPACKLO

    	_endasm
    }

    // send ACK/NACK
    AUX = 0; // ensure LAT bit is clear
    if (giveack == 0)
    {
        tris_AUX = 0;   // output zero for ACK
    }

    _asm
            movf    ASM_TEMP3_RAM, 0, 0    // delay 10 cycles per count
            nop
            nop   
            nop
			nop
           DELAYLOOPACKLO:
            nop
            nop   
            nop
			nop
            nop
            nop
            nop
            decfsz  WREG, 1, 0
            bra     DELAYLOOPACKLO
    		bsf		LATA, 3, 0             // ACK clock
            movf    ASM_TEMP3_RAM, 0, 0    // delay 5 cycles per count
            nop
            nop
           DELAYLOOPACKHI:
            nop
            nop
            decfsz  WREG, 1, 0
            bra     DELAYLOOPACKHI  
            nop
            nop 
            bcf     LATA, 3, 0             // ACK clock low
            nop
            nop
            nop
            nop
  	_endasm

    tris_AUX = 1;       // back to input
    return asm_temp1;
}

/******************************************************************************
 * Function:        unsigned char SPI_ReadWrite(unsigned char outputbyte)
 *
 * Overview:        Shifts outputbyte out on the AUX pin with PGC as SCK
 *                  At the same time, bits on PGD are shifted in and
 *                  returned as the read value. 
 *
 * PreCondition:    PGC, AUX = output ; PGD = input
 *
 * Input:           outputbyte - byte to be shifted out MSb first on AUX
 *
 *
 * Output:          returns the byte shifted in MSb first from PGD
 *
 * Side Effects:    None
 *
 * Note:            Assumes ICSP pins are already set to outputs.
 *****************************************************************************/
unsigned char SPI_ReadWrite(unsigned char outputbyte)
{
	//BOOL interrupts_on = 0;
    char i;

	//if (INTCONbits.GIE == 1)
	//	interrupts_on = 1;
	//INTCONbits.GIE = 0;			// uninterruptable routine

	asm_temp1 = outputbyte;         // read byte is shifted in here as well
    asm_temp2 = 8;

    if (icsp_baud < 2)
    {
    	_asm
           WRITE8LOOPF:
    		btfss	ASM_TEMP1_RAM, 7, 0
    		bcf		LATA, 4, 0 
    		btfsc	ASM_TEMP1_RAM, 7, 0
    		bsf		LATA, 4, 0 
    		bsf		LATA, 3, 0
            rlncf   ASM_TEMP1_RAM, 1, 0
            bcf     ASM_TEMP1_RAM, 0, 0
            btfsc   PORTA, 2, 0
            bsf     ASM_TEMP1_RAM, 0, 0
            bcf     LATA, 3, 0
            decfsz  ASM_TEMP2_RAM, 1, 0
            bra     WRITE8LOOPF
    	_endasm
    }
    else
    {
        asm_temp3 = icsp_baud - 1;
    	_asm
           WRITE8LOOPS:
    		btfss	ASM_TEMP1_RAM, 7, 0
    		bcf		LATA, 4, 0 
    		btfsc	ASM_TEMP1_RAM, 7, 0
    		bsf		LATA, 4, 0 
            movf    ASM_TEMP3_RAM, 0, 0    // delay 6 cycles per count
           DELAYLOOPHI:
            nop
            nop
            nop
            decfsz  WREG, 1, 0
            bra     DELAYLOOPHI
    		bsf		LATA, 3, 0
            rlncf   ASM_TEMP1_RAM, 1, 0
            bcf     ASM_TEMP1_RAM, 0, 0
            btfsc   PORTA, 2, 0
            bsf     ASM_TEMP1_RAM, 0, 0
            movf    ASM_TEMP3_RAM, 0, 0    // delay 6 cycles per count
           DELAYLOOPLO:
            nop
            nop
            nop
            decfsz  WREG, 1, 0
            bra     DELAYLOOPLO
            bcf     LATA, 3, 0
            decfsz  ASM_TEMP2_RAM, 1, 0
            bra     WRITE8LOOPS
    	_endasm
    }

	//if (interrupts_on == 1)		// turn interrupts back on if enabled.	
	//	INTCONbits.GIE = 1;
    
    AUX = 0;        // leave low

    return asm_temp1;

}

/******************************************************************************
 * Function:        void EE_WriteByte(unsigned char byte_address, unsigned char write_byte)
 *
 * Overview:        Writes value write_byte to the internal EEPROM address byte_address. 
 *
 * PreCondition:    none
 *
 * Input:           byte_address - EEPROM address to be written to
 *                  write_byte - byte value to write to EEPROM
 *
 * Output:          Specified EE byte address is written with given value.
 *
 * Side Effects:    Interrupts are disabled during EE write sequence.
 *
 * Note:            
 *****************************************************************************/
void EE_WriteByte(unsigned char byte_address, unsigned char write_byte)
{
    BOOL interrupts_on = 0;

    EEADR = byte_address;
    EEDATA = write_byte;
    EECON1 = 0;
    EECON1bits.WREN = 1;

	if (INTCONbits.GIE == 1)
		interrupts_on = 1;
	INTCONbits.GIE = 0;			// uninterruptable routine

    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;

    if (interrupts_on == 1)		// turn interrupts back on if enabled.	
		INTCONbits.GIE = 1;

    while (EECON1bits.WR == 1); // wait for write to complete
}

/******************************************************************************
 * Function:        unsigned char EE_ReadByte(unsigned char byte_address)
 *
 * Overview:        Reads a byte from EEPROM at the given address. 
 *
 * PreCondition:    none
 *
 * Input:           byte_address - EEPROM address to be read from
 *
 * Output:          Returns value of EE byte at byte_address
 *
 * Side Effects:    None.
 *
 * Note:            
 *****************************************************************************/
unsigned char EE_ReadByte(unsigned char byte_address)
{
    EEADR = byte_address;
    EECON1 = 0;
    EECON1bits.RD = 1;
    return EEDATA;

}

/** EOF pickit.c *********************************************************/
