// MakeTempFilenameDLL.cpp : Defines the entry point for the DLL application.

#include "stdafx.h"
#include "dll_rs232.h"

using namespace std;

/* File to be used for debug purpose */
FILE* fuscrs = fopen("rs232_log.txt","w");
SerialPort sci;

/* Enable serial debug */
#define _SERIAL_PORT_DEBUG_
//#undef  _SERIAL_PORT_DEBUG_

#ifdef _SERIAL_PORT_DEBUG_
  #define DBG_PRINT_ERR(x)	myfprintf(fuscrs,"%d error -> GetLastError()=%d\n", x, GetLastError())
#else
  #define DBG_PRINT_ERR(x)    {int i=x;}
#endif

/* ...to do... 
#ifdef RS232_OVERLAPPED_MODE
  OVERLAPPED osReader = {0};
  OVERLAPPED osWrite = {0};
#endif
*/

SerialPort::SerialPort():handle(INVALID_HANDLE_VALUE)
{
}

bool SerialPort::open(const char *comname, ParityType parity, 
		FlowType flow, long int baudrate,
		char databits, char stopbit,DWORD RxBufferSize)
{		

	if(is_open())
	{
		if (!CloseHandle(handle)) 
		{
			DBG_PRINT_ERR(0);
			return false;
		}
	}
	string t=string("\\\\.\\")+string(comname);
	
	/*
	#ifdef RS232_OVERLAPPED_MODE
	// Create the overlapped event. Must be closed before exiting
	// to avoid a handle leak.
	osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (osReader.hEvent == NULL){
		// Error creating overlapped event; abort.
		DBG_PRINT_ERR(1);
		return false;
	}
	// Create this write operation's OVERLAPPED structure's hEvent.
    osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (osWrite.hEvent == NULL) {
		// Error creating overlapped event; abort.
		DBG_PRINT_ERR(1);
		return false;
	}
	handle = CreateFile(t.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	#endif
    */
    
	// RS232, NOT OVERLAPPED APPROACH
	handle = CreateFile(t.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);

	if (handle == INVALID_HANDLE_VALUE) {
		DBG_PRINT_ERR(2);
		return false;
	}

	DCB dcb = {0};
	dcb.DCBlength = sizeof(dcb);
	if(!GetCommState(handle, &dcb)) {
		DBG_PRINT_ERR(3);
		return false;
	}

	COMMPROP comprop;
	if (!GetCommProperties(handle, &comprop)) {
		DBG_PRINT_ERR(4);
		return false;
	}

	if (!SetupComm(handle, RxBufferSize, SERIAL_MAX_TX_BUF)) {
		DBG_PRINT_ERR(5);
		return false;
	}

	if (!GetCommProperties(handle, &comprop)) {
		DBG_PRINT_ERR(6);
		return false;
	}

	dcb.BaudRate = baudrate;
	dcb.ByteSize = databits;
	dcb.StopBits = stopbit;

	if (stopbit == 1)
		dcb.StopBits = ONESTOPBIT;
	if (stopbit == 2)
		dcb.StopBits = TWOSTOPBITS;

	switch(parity)
	{
	case ParityNone:
		dcb.fParity = false;
		dcb.Parity = NOPARITY;
		break;
	case ParityOdd:
		dcb.fParity = true;
		dcb.Parity = ODDPARITY;
		break;
	case ParityEven:
		dcb.fParity = true;
		dcb.Parity = EVENPARITY;
		break;
	}

	switch(flow)
	{
	case FlowNone:
		dcb.fNull = FALSE;
		dcb.fOutxCtsFlow = FALSE;
		dcb.fOutxDsrFlow = FALSE;
		dcb.fDtrControl  = DTR_CONTROL_DISABLE;
		dcb.fRtsControl  = RTS_CONTROL_DISABLE;
		dcb.fInX = dcb.fOutX = FALSE;
		break;
	case FlowHardware:
		dcb.fNull = FALSE;
		dcb.fOutxCtsFlow = TRUE;
		dcb.fOutxDsrFlow = TRUE;
		dcb.fDtrControl  = DTR_CONTROL_ENABLE;
		dcb.fRtsControl  = RTS_CONTROL_ENABLE;
		break;
	case FlowXonXoff: 
		dcb.fInX = dcb.fOutX = TRUE;
		dcb.XonChar  = 0;
		dcb.XoffChar = 0;
		dcb.XonLim   = (WORD) 65536;
		dcb.XoffLim  = (WORD) 65536;
		dcb.fTXContinueOnXoff = false;
		break;
	}

	if(!SetCommState(handle, &dcb)){
		DBG_PRINT_ERR(7);
		return false;
	}

	COMMTIMEOUTS timeouts = {0};
	timeouts.ReadIntervalTimeout = READ_INTERVAL_TIMEOUT;
	timeouts.ReadTotalTimeoutMultiplier = READ_TOTAL_TIMEOUT_MULTIPLIER;
	timeouts.ReadTotalTimeoutConstant = READ_TOTAL_TIMEOUT_CONSTANT;
	timeouts.WriteTotalTimeoutMultiplier = WRITE_TOTAL_TIMEOUT_MULTIPLIER;
	timeouts.WriteTotalTimeoutConstant = WRITE_TOTAL_TIMEOUT_CONSTANT;

	if (!SetCommTimeouts(handle, &timeouts)) {
		DBG_PRINT_ERR(8);
		return false;
	}

	return true;
}

int SerialPort::read(unsigned char *buf, int len)
{
	if(handle == INVALID_HANDLE_VALUE)
		return -1;

	DWORD bytesread = 0;
	
	#ifdef RS232_OVERLAPPED_MODE
	if (!ReadFile(handle, buf, len, &bytesread, &osReader)) {
		if (GetLastError() != ERROR_IO_PENDING) {
			DBG_PRINT_ERR(9);
			return -1;
		}
		else
			bytesread = 0;
	}
    #else
    if (!ReadFile(handle, buf, len, &bytesread, NULL)) {
		DBG_PRINT_ERR(9);
		return -1;
	}
    #endif

	return static_cast<int> (bytesread);
}

int SerialPort::write(unsigned char *buf, int len)
{
	if(handle == INVALID_HANDLE_VALUE)
		return -1;

	DWORD byteswritten = 0;
	
	#ifdef RS232_OVERLAPPED_MODE
	if (!WriteFile(handle,buf,len,&byteswritten,&osWrite)) {
		if (GetLastError() != ERROR_IO_PENDING) { 
			// WriteFile failed, but isn't delayed. Report error and abort.
			DBG_PRINT_ERR(10);
			return -1;
		}
		else
			byteswritten = 0;
	}
    #else
	if (!WriteFile(handle, buf, len, &byteswritten, NULL)) {
		DBG_PRINT_ERR(10);
		return -1;
	}
	#endif

	return static_cast<int> (byteswritten);
}

void SerialPort::purge()
{
	if(handle == INVALID_HANDLE_VALUE)
		return;

	if (!PurgeComm(handle, PURGE_RXCLEAR))
		DBG_PRINT_ERR(11);

}

bool SerialPort::close()
{
	if (!is_open())
		return true;
	if (!CloseHandle(handle)) {
		DBG_PRINT_ERR(12);
		return false;
	}

	handle = INVALID_HANDLE_VALUE;
	return true;
}


bool SerialPort::setReadTimeOut(int ms)
{
	if(handle == INVALID_HANDLE_VALUE)
		return false;

	COMMTIMEOUTS timeouts;

	if(!GetCommTimeouts(handle,&timeouts)) {
		DBG_PRINT_ERR(13);
		return false;
	}

	timeouts.ReadIntervalTimeout=0;
	timeouts.ReadTotalTimeoutMultiplier=0;
	timeouts.ReadTotalTimeoutConstant=ms;

	if(!SetCommTimeouts(handle,&timeouts)){
		DBG_PRINT_ERR(14);
		return false;
	}

	return true;
}


SerialPort::~SerialPort()
{
	close();
}










