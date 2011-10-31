/*
 * ERIKA Enterprise Basic - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2011  Evidence Srl
 *
 * This file is part of ERIKA Enterprise Basic.
 *
 * ERIKA Enterprise Basic is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * ERIKA Enterprise Basic is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with ERIKA Enterprise Basic; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 */

#include "stdafx.h"
#include "mcp2200.h"

#include <stdio.h> 
#include <stdlib.h> 
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

#ifdef _DEBUG
FILE * fmcp2200_debug;
#endif

static const unsigned int mcp2200_VID = 0x04D8; //VID for MCP2200
static const unsigned int mcp2200_PID = 0x00DF; //PID for MCP2200

static const bool OFF                = 0;
static const bool ON                 = 1;
static const unsigned int  TOGGLE    = 3;
static const unsigned int  BLINKSLOW = 4;
static const unsigned int  BLINKFAST = 5;

//(Prototyping the DLL's function) Use "stdcall” calling convention 
typedef void (__stdcall * pICFUNC0)(unsigned int, unsigned int); 
typedef bool (__stdcall * pICFUNC1)(unsigned int);
typedef bool (__stdcall * pICFUNC2)(unsigned int, unsigned int *);
typedef bool (__stdcall * pICFUNC3)(unsigned char, unsigned long, unsigned int, unsigned int, bool, bool, bool);
typedef bool (__stdcall * pICFUNC4)(void);
typedef bool (__stdcall * pICFUNC5)(unsigned char);
typedef bool (__stdcall * pICFUNC6)(unsigned long);

static HMODULE DLL_handle;
static bool correct_init;

static pICFUNC0 DLL_InitMCP2200;
static pICFUNC4 DLL_IsConnected;
static pICFUNC1 DLL_SetPin;
static pICFUNC1 DLL_ClearPin;
static pICFUNC2 DLL_ReadPin;
static pICFUNC3 DLL_ConfigureMCP2200;
static pICFUNC5 DLL_ConfigureIO;
static pICFUNC6 DLL_fnSetBaudRate;
static pICFUNC1 DLL_fnRxLED;
static pICFUNC1 DLL_fnTxLED;

static enum MCP2200_Init_Result
{
    INIT_FAIL,
    NOT_CONNECTED_INIT,
    COMPLETE_INIT
};

static enum MCP2200_Init_Result init_mcp2200()
{
    //STEP 1: Get handle to DLL– Path and name (Ex. C:\\SimpleIO-UM.dll) or just name
    // if in working directory (put this in the quotes) 
    DLL_handle = (HMODULE)::GetModuleHandle(TEXT("SimpleIO-UM.dll"));

    //Print result of LoadLibrary call 
	if( DLL_handle == NULL ) //If it is null, LoadLibrary call failed 
	{ 
		DWORD error = ::GetLastError(); 
		print_debug("Loading of the DLL failed\n"); 
		print_debug("The error was: %d%s", error, "\n\n");
        flush_debug();
		return INIT_FAIL; 
	} 
	else 
    { 
		print_debug("DLL has been loaded\n\n"); 
	} 

	//STEP 2: Get pointer to the function in the DLL 
    DLL_InitMCP2200      = (pICFUNC0)::GetProcAddress(DLL_handle, "InitMCP2200");
    DLL_IsConnected      = (pICFUNC4)::GetProcAddress(DLL_handle, "IsConnected");
    DLL_ConfigureMCP2200 = (pICFUNC3)::GetProcAddress(DLL_handle, "ConfigureMCP2200");
    DLL_ConfigureIO      = (pICFUNC5)::GetProcAddress(DLL_handle, "ConfigureIO");
	DLL_SetPin           = (pICFUNC1)::GetProcAddress(DLL_handle, "SetPin"); 
	DLL_ClearPin         = (pICFUNC1)::GetProcAddress(DLL_handle, "ClearPin");
    DLL_ReadPin          = (pICFUNC2)::GetProcAddress(DLL_handle, "ReadPin");
    DLL_fnSetBaudRate    = (pICFUNC6)::GetProcAddress(DLL_handle, "fnSetBaudRate");
    DLL_fnRxLED          = (pICFUNC1)::GetProcAddress(DLL_handle, "fnRxLED");   
    DLL_fnTxLED          = (pICFUNC1)::GetProcAddress(DLL_handle, "fnTxLED");
    

    //STE5 3: Call the DLL function through the prototype name given in step 3 
	//Initialize the MCP2200 – NOTE: Must be plugged in when program is ran 
	DLL_InitMCP2200(mcp2200_VID, mcp2200_PID);
	print_debug("The MCP2200 was successfully initialized.\n");

    //Check connection status. 
	bool connected = DLL_IsConnected();
    if(connected)
    {
        print_debug("The device is CONNECTED\n\n");
    } 
    else 
    {
        print_debug("The device is DISCONNECTED\n\n");
    }
    flush_debug();
    return (connected)? COMPLETE_INIT: NOT_CONNECTED_INIT;
}

static void reset_gateway()
{
    HMODULE DLL_searial_handle = (HMODULE)::GetModuleHandle(TEXT("lib_serial_gateway.dll"));
    if(!DLL_searial_handle)
    {
        print_debug("lib_easylab_gateway.dll not found. Nothing to do");
        return;
    }
    typedef HWND (__stdcall * pGetGatewayHwnd)(void);
    pGetGatewayHwnd DLL_GetGatewayHwnd = (pGetGatewayHwnd)::GetProcAddress(DLL_searial_handle, "GetGatewayHwnd");
    
    if(!DLL_GetGatewayHwnd){
        print_debug("Invalid get gateway function name!");
        return;
    }

    HWND GatewayHwnd = DLL_GetGatewayHwnd();
    if(!GatewayHwnd){
        print_debug("No Gateway actived. nothing to do.");
        return;
    }
    
    UINT resetMsgId = ::RegisterWindowMessage("Reset Easylab Gateway");
    if (!resetMsgId)
    {
        print_debug("Invalid Reset Easylab Gateway message ID!");
        return;
    }
    ::PostMessage(GatewayHwnd, resetMsgId, 0, 0);
}

static void init(scicos_block *block)
{
	//Enable Debug File if neeeded;
    start_debug();
    //init mcp2200 component
    enum MCP2200_Init_Result init_result = init_mcp2200();
    correct_init = (init_result == COMPLETE_INIT);

    if(correct_init){
        //Init MCP2200 input pins (consequently output pins)
        unsigned long baudrate = (unsigned long)rpar(block, 0);
        unsigned char ioMap = 0;
        const int nPinIn = GetNout(block);
        for(int i = 0; i < nPinIn; ++i)
            ioMap |= 1 << ipar(block, i);
        /* 
            ConfigureMCP2200(unsigned char IOMap,
	            unsigned long BaudRateParam,
			    unsigned int RxLEDMode,
			    unsigned int TxLEDMode,
		        bool FLOW,
		        bool ULOAD,
		        bool SSPND) 
        */
        correct_init = DLL_ConfigureMCP2200(ioMap, baudrate, BLINKSLOW, 
            BLINKSLOW, OFF, OFF, OFF);
    }
    if(correct_init)
        reset_gateway();
}

static void inout(scicos_block *block)
{
    //pin In & pin Out are configured sequentially as ipar (integer parameters)
    if(correct_init)
    {
        const int nPinIn = GetNout(block);
        
        /* Read Pin In */
        for(int i = 0; i < nPinIn; ++i)
        {
            unsigned int pinIn  = ipar(block, i);

            unsigned int pinInValue;
            if(DLL_ReadPin(pinIn, &pinInValue))
            {
                //Set the block output value
                output(block, i, 0) = pinInValue;
            } else {
                print_debug("WARNING! Problem encountered reading pin in:%d", pinIn);
            }
        }
        /* Write Pins Out */
        const int pinTotal = GetNipar(block);
        for(int i = nPinIn; i < pinTotal; ++i)
        {
            unsigned int pinOut = ipar(block, i);
            unsigned int inPort = i - nPinIn;
            //Read correspondingly input port and act accordingly
            if(input(block, inPort, 0) > 0.5)
              {
                if(!DLL_SetPin(pinOut))
                    print_debug("WARNING! Problem encountered setting pin out:%d", pinOut);
            } else {
                if(!DLL_ClearPin(pinOut))
                    print_debug("WARNING! Problem encountered clearing pin out:%d", pinOut);
            }
        }
        flush_debug();
    }
}

static void end(scicos_block *block)
{
	/* End the debug if needed */
    stop_debug();
    correct_init = false;
}

MCP2200_LIB_API void mcp2200_block(scicos_block *block,int flag)
{
    if (flag == OutputUpdate){
        /* set output */
        inout(block);
    }
    else if (flag == Ending){
        /* termination */ 
        end(block);
    }
    else if (flag == Initialization){
        /* initialisation */
        init(block);
    }
}



