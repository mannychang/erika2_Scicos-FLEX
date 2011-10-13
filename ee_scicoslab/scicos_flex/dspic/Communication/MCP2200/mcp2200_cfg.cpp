/*
 * ERIKA Enterprise Basic - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2007  Evidence Srl
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

using namespace std;

static void init(scicos_block *block)
{
	//Variables 
	int userSelection = 0; //User input when requested for main menu 
	static unsigned int mcp2200_VID = 0x04D8; //VID for MCP2200
    static unsigned int mcp2200_PID = 0x00DF; //PID for MCP2200 
	bool connectedStatus = false; //Connection status of MCP2200 
	unsigned int result = 0; //Result of function calls with uint returned 
	//STEP 1: Get handle to DLL– Path and name (Ex. C:\\SimpleIO-UM.dll) or just name
    // if in working directory (put this in the quotes) 
	HINSTANCE DLL_handle = LoadLibrary(TEXT("SimpleIO-UM.dll")); 
	//Print result of LoadLibrary call 
	if( DLL_handle == NULL ) //If it is null, LoadLibrary call failed 
	{ 
		DWORD error = GetLastError(); 
		cout << "Loading of the DLL failed\n"; 
		cout << "The error was: " << error << "\n\n"; 
		return; 
	} 
	else { 
		cout << "DLL has been loaded\n\n"; 
	} 
	//STEP 2: Get pointer to the function in the DLL 
	FARPROC lpfnGetProcessID0 = GetProcAddress(HMODULE (DLL_handle),"InitMCP2200"); 
	FARPROC lpfnGetProcessID1 = GetProcAddress(HMODULE (DLL_handle),"IsConnected"); 
	//STEP 3: Define the Function in the DLL for reuse. 
	//(Prototyping the DLL's function) Use "stdcall” calling convention 
	typedef void (__stdcall * pICFUNC0)(unsigned int, unsigned int); 
	typedef bool (__stdcall * pICFUNC1)(); 
	//Same step as above. 
	pICFUNC0 DLL_InitMCP2200 = pICFUNC0(lpfnGetProcessID0); 
	pICFUNC1 DLL_IsConnected = pICFUNC1(lpfnGetProcessID1); 
	//STEP 4: Call the DLL function through the prototype name given in step 3 
	//Initialize the MCP2200 – NOTE: Must be plugged in when program is ran 
	DLL_InitMCP2200(mcp2200_VID, mcp2200_PID); 
	cout << "The MCP2200 was successfully initialized.\n"; 
	//Check connection status. 
	connectedStatus = DLL_IsConnected(); 
	if(connectedStatus == true) 
		cout <<"The device is CONNECTED"; 
	else 
		cout << "The device is DISCONNECTED"; 
	cout << "\n\n"; 
	//STEP 5: Release the DLL 
	FreeLibrary(DLL_handle); 
	return;
}

static void inout(scicos_block *block)
{
	// to do: display the value in the figure
}

static void end(scicos_block *block)
{
	// to do: figure termination procedure
}

MCP2200_LIB_API void mcp2200_cfg(scicos_block *block,int flag)
{
  if (flag==OutputUpdate){
    /* set output */
    inout(block);
  }
  else if (flag==Ending){
    /* termination */ 
    end(block);
  }
  else if (flag ==Initialization){
    /* initialisation */
    init(block);
  }
}


