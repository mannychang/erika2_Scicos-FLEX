/* _MCP2200_H_ */

#ifndef _MCP2200_H_
#define _MCP2200_H_

#ifdef MCP2200_LIB_EXPORTS
#define MCP2200_LIB_API	__declspec(dllexport) 
#else
#define MCP2200_LIB_API	__declspec(dllimport) 
#endif

/* find Scilab lib*/
//#pragma comment(lib,"LibScilab.lib")

/* To make this header compatible with c sources */
#ifdef __cplusplus
extern "C"{
#endif
#include "scicos_block4.h"
MCP2200_LIB_API void mcp2200_block(scicos_block *block,int flag);
#ifdef __cplusplus
}
#endif


/*** Definitions ***/
/* This macro should be used to read block inputs 
 * i: is the index to select the line
 * j: is the index to select the element of the i-th line 
 */
#define input(block,i,j)  ((double *)block->inptr[i])[j]   

/* This macro should be used to write block outputs 
 * i: is the index to select the line
 * j: is the index to select the element of the i-th line 
 */
#define output(block,i,j)  ((double *)block->outptr[i])[j]

/* This macro should be used to read integer-type parameters */
#define ipar(block,i) (block->ipar[i])

/* This macro should be used to read integer-type parameters */
#define rpar(block,i) (block->rpar[i])


/*** Global variables ***/
#ifdef _DEBUG
extern FILE* fmcp2200_debug;      /* File to be used for debug purpose */
#define start_debug() ((fmcp2200_debug = _fsopen("mcp2220_debug.log", "w", _SH_DENYNO)) != NULL)
#define print_debug(...) fprintf(fmcp2200_debug, __VA_ARGS__)
#define flush_debug() fflush(fmcp2200_debug)
#define stop_debug() fclose(fmcp2200_debug)
#else
#define start_debug() (0)
#define print_debug(...) (0)
#define flush_debug() (0)
#define stop_debug() (0)
#endif

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
