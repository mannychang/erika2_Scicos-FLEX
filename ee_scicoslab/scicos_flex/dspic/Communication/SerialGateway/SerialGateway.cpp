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
#include "SerialGateway.h"
#include <Psapi.h>
#include <sstream>

typedef std::basic_string<TCHAR> tstring;
typedef std::basic_stringstream<TCHAR> tstringstream;

#ifdef _DEBUG
FILE * fserial_gateway_debug;
#endif

static PROCESS_INFORMATION gatewayProcessInfo;
static bool gatewayStarted;
static HWND gatewayHwnd;

static tstring GetGatewayPath()
{
    HMODULE hModule = ::GetModuleHandle(_T("lib_serial_gateway.dll"));
    TCHAR dllPath[MAX_PATH];
    ::GetModuleFileName(hModule, dllPath, MAX_PATH);
    tstring gatewayPath(dllPath);
    
    for(int i = 0; i < 5; ++i)
    {
        /* erase tralling directory name s in path */
        gatewayPath.erase(gatewayPath.find_last_of('\\'), gatewayPath.length());
    }

    gatewayPath += _T("\\EasylabSerialUDPGateway\\EasylabSerialUDPGateway.exe");
    return gatewayPath;
}

static bool startGateway(int commPort, int sendingPort, int receivingPort)
{
    tstringstream parameters;
    //* The first parameter needs to be the exe itself */
    parameters << _T("EasylabSerialUDPGateway.exe --COM=") << commPort << _T(" --sending-port=") 
        << sendingPort << _T(" --receiving-port=") << receivingPort << std::endl;
    tstring gatewayPath = GetGatewayPath();
    
    const tstring& parametersStr = parameters.str();
    /* CreateProcessW can modify parameters. So I use a temporary buffer */
    TCHAR cmdLine[MAX_PATH] = {};
    _tcscpy_s(cmdLine, parametersStr.c_str());


    /* CreateProcess API initialization: 
       I could have wrote STARTUPINFO siStartupInfo = {sizeof(siStartupInfo)};
       but it would have been less readable
    */ 
    STARTUPINFO siStartupInfo = {};
    siStartupInfo.cb = sizeof(siStartupInfo); 

    bool processStarted = ::CreateProcess(gatewayPath.c_str(), 
        cmdLine, 0, 0, false, CREATE_DEFAULT_ERROR_MODE, 0, 0, &siStartupInfo, &gatewayProcessInfo); 
    

    return processStarted;
}

static void init(scicos_block *block)
{
	/* Enable Debug File if neeeded */
    start_debug();
    gatewayStarted = startGateway(ipar(block, 0), ipar(block, 1), ipar(block, 2));
}

static void end(scicos_block *block)
{
    /* Close gateway */
    if(gatewayStarted)
    {
        HWND localGatewayHWND = GetGatewayHwnd();
        ::PostMessage(localGatewayHWND, WM_CLOSE, 0, 0);
        /* I try to wait signal from the process */
        if(::WaitForSingleObject(gatewayProcessInfo.hProcess, 5000) != WAIT_OBJECT_0)
            ::TerminateProcess(gatewayProcessInfo.hProcess, 0); 
    }
    
    /* Release handles */
    ::CloseHandle(gatewayProcessInfo.hProcess); 
    ::CloseHandle(gatewayProcessInfo.hThread);
    
    /* Clean Up */
    memset(&gatewayProcessInfo, 0, sizeof(gatewayProcessInfo));
    gatewayHwnd = NULL;
    gatewayStarted = false;

    /* End the debug if needed */
    stop_debug();
}

static BOOL CALLBACK GetGatewayHwndProc(__in  HWND hwnd, __in  LPARAM lParam)
{
    BOOL retVal = TRUE;
    DWORD processId = (DWORD)lParam;
    DWORD dwId;
    ::GetWindowThreadProcessId(hwnd, &dwId);
    
    if(dwId == processId)
    {
        gatewayHwnd = hwnd;
        retVal = FALSE;
    }
    return retVal;
}

static void FindGatewayHwnd()
{
    // Get the list of process identifiers.
    DWORD procesIds[4096], cbNeeded, cProcesses;

    if (!::EnumProcesses(procesIds, sizeof(procesIds), &cbNeeded))
        return;

    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);
    TCHAR processPath[MAX_PATH] = {};

    // Print the name and process identifier for each process.
    for (unsigned int i = 0; i < cProcesses; i++)
    {
        if(procesIds[i] != 0)
        {
            // Get a HANDLE to the process.
            DWORD processId = procesIds[i];
            HANDLE hProcess = ::OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
            if(hProcess) {
                ::GetModuleFileNameEx(hProcess, NULL, processPath, MAX_PATH);
                tstring processPathStr(processPath);
                size_t bPos;
                if(!processPathStr.empty() && ((bPos = processPathStr.find_last_of('\\')) != tstring::npos)) {
                    processPathStr = processPathStr.substr(bPos, processPathStr.length());
                    if(processPathStr == _T("\\EasylabSerialUDPGateway.exe")) {
                        ::EnumWindows(GetGatewayHwndProc , (LPARAM)processId);
                        break;
                    }
                }
            }
        }
    }
}

SERIAL_GATEWAY_LIB_API HWND GetGatewayHwnd(void)
{
    HWND localGatewayHwnd = NULL;
    if(!gatewayHwnd) {
        if(gatewayProcessInfo.dwProcessId > 0) {
            ::EnumThreadWindows(gatewayProcessInfo.dwThreadId, GetGatewayHwndProc,
                gatewayProcessInfo.dwProcessId);
            localGatewayHwnd = gatewayHwnd;
        }
        else {
            FindGatewayHwnd();
            localGatewayHwnd = gatewayHwnd;
            gatewayHwnd = NULL;
        }
    }
    return localGatewayHwnd;
}

SERIAL_GATEWAY_LIB_API void serial_gateway_block(scicos_block *block,int flag)
{
    if (flag == Initialization)
    {
        /* initialisation */
        init(block);
    } else if (flag == Ending){
        /* termination */ 
        end(block);
    }
}