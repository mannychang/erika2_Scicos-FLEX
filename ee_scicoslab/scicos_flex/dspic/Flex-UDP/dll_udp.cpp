// MakeTempFilenameDLL.cpp : Defines the entry point for the DLL application.

#include "stdafx.h"
#include "dll_udp.h"

/* File to be used for debug purpose */
FILE* fuscrs = fopen("logrs.txt","w");

/*** Global variables ***/
volatile sockaddr_in dest_addr; /* remote ip address */
volatile UDPStruct UDPrs;       /* socket used to send/receive */
volatile UDPStruct* sock_str_ptr_rs = &UDPrs; /* UDP socket pointer */












