/* _MCP2200_H_ */

#ifndef _MCP2200_H_
#define _MCP2200_H_

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32 
#include <Winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#endif
#pragma comment(lib,"LibScilab.lib")


extern "C"{
#include "scicos_block4.h"
}

#ifdef MCP2200_LIB_EXPORTS
#define MCP2200_LIB_API	__declspec(dllexport) 
#else
#define MCP2200_LIB_API	__declspec(dllimport) 
#endif

/*** Definitions ***/
/* This macro should be used to read block inputs 
 * i: is the index to select the line
 * j: is the index to select the element of the i-th line 
 */
#define input(i,j)  ((double *)block->inptr[i])[j]   

/* This macro should be used to write block outputs 
 * i: is the index to select the line
 * j: is the index to select the element of the i-th line 
 */
#define output(i,j)  ((double *)block->outptr[i])[j]

/* This macro should be used to read integer-type parameters */
#define ipar(i) (block->ipar[i])

/* This macro should be used to read integer-type parameters */
#define rpar(i) (block->rpar[i])

/*** Global variables ***/
extern FILE* fusc_mcp;      /* File to be used for debug purpose */


/* MCP2200 API */
/*
	Summary:
		void SimpleIOClass::InitMCP2200(unsigned int VendorID, unsigned int ProductID)
		bool SimpleIOClass::IsConnected()
		bool SimpleIOClass::ConfigureMCP2200(unsigned char IOMap,
			unsigned long BaudRateParam,
			unsigned int RxLEDMode,
			unsigned int TxLEDMode,
		bool FLOW,
		bool ULOAD,
		bool SSPND)
		bool SimpleIOClass::SetPin(unsigned int pin)
		bool SimpleIOClass::ClearPin(unsigned int pin)
			int SimpleIOClass::ReadPinValue(unsigned int pin)
		bool SimpleIOClass::ReadPin(unsigned int pin,
			unsigned int *returnvalue)
		bool SimpleIOClass::WritePort(unsigned int portValue)
		bool SimpleIOClass::ReadPort(unsigned int *returnvalue)
			int SimpleIOClass::ReadPortValue()
			int SimpleIOClass::SelectDevice(unsigned int uiDeviceNo)
			int SimpleIOClass::GetSelectedDevice()
			unsigned int SimpleIOClass::GetNoOfDevices()
		void SimpleIOClass::GetDeviceInfo(unsigned int uiDeviceNo, LPSTR strOutput)
		void SimpleIOClass::GetSelectedDeviceInfo(LPSTR strOutput)
		int SimpleIOClass::ReadEEPROM(unsigned int uiEEPAddress)
		int SimpleIOClass::WriteEEPROM(unsigned int uiEEPAddress,
			unsigned char ucValue)
	While ConfigureMCP2200 configures the device with one call, it may also be configured one parameter at a time:
		bool SimpleIOClass::fnRxLED(unsigned int mode)
		bool SimpleIOClass::fnTxLED(unsigned int mode)
		bool SimpleIOClass::fnHardwareFlowControl(unsigned int onOff)
		bool SimpleIOClass::fnULoad(unsigned int onOff)
		bool SimpleIOClass::fnSuspend(unsigned int onOff)
		bool SimpleIOClass::fnSetBaudRate(unsigned long BaudRateParam)
		bool SimpleIOClass::ConfigureIO(unsigned char IOMap)
		bool SimpleIOClass::ConfigureIoDefaultOutput(unsigned char ucIoMap,
			unsigned char ucDefValue)
	Constants:
		const unsigned int OFF = 0;
		const unsigned int ON = 1;
		const unsigned int TOGGLE = 3;
		const unsigned int BLINKSLOW = 4;
		const unsigned int BLINKFAST = 5;
*/

#endif /* _MCP2200_H_ */
