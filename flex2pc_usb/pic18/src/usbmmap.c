/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.0
 *
 *********************************************************************
 * FileName:        usbmmap.c
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
 * Rawin Rojvanit       7/10/04     Original.
 ********************************************************************/

/** I N C L U D E S **********************************************************/
#include "system\typedefs.h"
#include "system\usb\usb.h"

/** U S B  G L O B A L  V A R I A B L E S ************************************/
#pragma udata
byte usb_device_state;          // Device States: DETACHED, ATTACHED, ...
USB_DEVICE_STATUS usb_stat;     // Global USB flags
byte usb_active_cfg;            // Value of current configuration
byte usb_alt_intf[MAX_NUM_INT]; // Array to keep track of the current alternate
                                // setting for each interface ID

/** U S B  F I X E D  L O C A T I O N  V A R I A B L E S *********************/
#pragma udata usbram4=0x400     //See Linker Script,usb4:0x400-0x4FF(256-byte)

/******************************************************************************
 * Section A: Buffer Descriptor Table
 * - 0x400 - 0x4FF(max)
 * - MAX_EP_NUMBER is defined in autofiles\usbcfg.h
 * - BDT data type is defined in system\usb\usbmmap.h
 *****************************************************************************/

#if(0 <= MAX_EP_NUMBER)
volatile far BDT ep0Bo;         //Endpoint #0 BD Out
volatile far BDT ep0Bi;         //Endpoint #0 BD In
#endif

#if(1 <= MAX_EP_NUMBER)
volatile far BDT ep1Bo;         //Endpoint #1 BD Out
volatile far BDT ep1Bi;         //Endpoint #1 BD In
#endif

#if(2 <= MAX_EP_NUMBER)
volatile far BDT ep2Bo;         //Endpoint #2 BD Out
volatile far BDT ep2Bi;         //Endpoint #2 BD In
#endif

#if(3 <= MAX_EP_NUMBER)
volatile far BDT ep3Bo;         //Endpoint #3 BD Out
volatile far BDT ep3Bi;         //Endpoint #3 BD In
#endif

#if(4 <= MAX_EP_NUMBER)
volatile far BDT ep4Bo;         //Endpoint #4 BD Out
volatile far BDT ep4Bi;         //Endpoint #4 BD In
#endif

#if(5 <= MAX_EP_NUMBER)
volatile far BDT ep5Bo;         //Endpoint #5 BD Out
volatile far BDT ep5Bi;         //Endpoint #5 BD In
#endif

#if(6 <= MAX_EP_NUMBER)
volatile far BDT ep6Bo;         //Endpoint #6 BD Out
volatile far BDT ep6Bi;         //Endpoint #6 BD In
#endif

#if(7 <= MAX_EP_NUMBER)
volatile far BDT ep7Bo;         //Endpoint #7 BD Out
volatile far BDT ep7Bi;         //Endpoint #7 BD In
#endif

#if(8 <= MAX_EP_NUMBER)
volatile far BDT ep8Bo;         //Endpoint #8 BD Out
volatile far BDT ep8Bi;         //Endpoint #8 BD In
#endif

#if(9 <= MAX_EP_NUMBER)
volatile far BDT ep9Bo;         //Endpoint #9 BD Out
volatile far BDT ep9Bi;         //Endpoint #9 BD In
#endif

#if(10 <= MAX_EP_NUMBER)
volatile far BDT ep10Bo;        //Endpoint #10 BD Out
volatile far BDT ep10Bi;        //Endpoint #10 BD In
#endif

#if(11 <= MAX_EP_NUMBER)
volatile far BDT ep11Bo;        //Endpoint #11 BD Out
volatile far BDT ep11Bi;        //Endpoint #11 BD In
#endif

#if(12 <= MAX_EP_NUMBER)
volatile far BDT ep12Bo;        //Endpoint #12 BD Out
volatile far BDT ep12Bi;        //Endpoint #12 BD In
#endif

#if(13 <= MAX_EP_NUMBER)
volatile far BDT ep13Bo;        //Endpoint #13 BD Out
volatile far BDT ep13Bi;        //Endpoint #13 BD In
#endif

#if(14 <= MAX_EP_NUMBER)
volatile far BDT ep14Bo;        //Endpoint #14 BD Out
volatile far BDT ep14Bi;        //Endpoint #14 BD In
#endif

#if(15 <= MAX_EP_NUMBER)
volatile far BDT ep15Bo;        //Endpoint #15 BD Out
volatile far BDT ep15Bi;        //Endpoint #15 BD In
#endif

/******************************************************************************
 * Section B: EP0 Buffer Space
 ******************************************************************************
 * - Two buffer areas are defined:
 *
 *   A. CTRL_TRF_SETUP
 *      - Size = EP0_BUFF_SIZE as defined in autofiles\usbcfg.h
 *      - Detailed data structure allows direct adddressing of bits and bytes.
 *
 *   B. CTRL_TRF_DATA
 *      - Size = EP0_BUFF_SIZE as defined in autofiles\usbcfg.h
 *      - Data structure allows direct adddressing of the first 8 bytes.
 *
 * - Both data types are defined in system\usb\usbdefs\usbdefs_ep0_buff.h
 *****************************************************************************/
volatile far CTRL_TRF_SETUP SetupPkt;
volatile far CTRL_TRF_DATA CtrlTrfData;

/******************************************************************************
 * Section C: HID Buffer
 ******************************************************************************
 *
 *****************************************************************************/
#if defined(USB_USE_HID)
volatile far unsigned char hid_report_out[HID_INT_OUT_EP_SIZE];
volatile far unsigned char hid_report_in[HID_INT_IN_EP_SIZE];
#endif

/******************************************************************************
 * Section D: CDC Buffer
 ******************************************************************************
 *
 *****************************************************************************/
#pragma udata usbram5a=0x500    //See Linker Script,usb5:0x500-0x5DF(224-byte)
#if defined(USB_USE_CDC)
volatile far unsigned char cdc_notice[CDC_INT_EP_SIZE];
volatile far unsigned char cdc_data_rx[CDC_BULK_OUT_EP_SIZE];
volatile far unsigned char cdc_data_tx[CDC_BULK_IN_EP_SIZE];
#endif
#pragma udata

/******************************************************************************
 * Section E: MSD Buffer
 ******************************************************************************
 *
 *****************************************************************************/
#pragma udata usbram5b=0x5E0    //See Linker Script,usb5:0x5E0-0x5FF(32-byte)
#if defined(USB_USE_MSD)
volatile far CB;                //MSD Cmd Block Wrapper & Cmd Status Wrapper
#endif
#pragma udata

#pragma udata usbram6=0x600     //See Linker Script,usb6:0x600-0x6FF(256-byte)
#if defined(USB_USE_MSD)
volatile far unsigned char msd_data_block0[MSD_BULK_DATA_EP_SIZE];
volatile far unsigned char msd_data_block1[MSD_BULK_DATA_EP_SIZE];
volatile far unsigned char msd_data_block2[MSD_BULK_DATA_EP_SIZE];
volatile far unsigned char msd_data_block3[MSD_BULK_DATA_EP_SIZE];
#endif
#pragma udata

#pragma udata usbram7=0x700     //See Linker Script,usb7:0x700-0x7FF(256-byte)

#if defined(USB_USE_MSD)
volatile far unsigned char msd_data_block4[MSD_BULK_DATA_EP_SIZE];
volatile far unsigned char msd_data_block5[MSD_BULK_DATA_EP_SIZE];
volatile far unsigned char msd_data_block6[MSD_BULK_DATA_EP_SIZE];
volatile far unsigned char msd_data_block7[MSD_BULK_DATA_EP_SIZE];
#endif

#pragma udata

/** EOF usbmmap.c ************************************************************/
