/*
 * ERIKA Enterprise Basic - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2010-2012  Evidence Srl
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

#ifndef _miscsim_h_
#define _miscsim_h_

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32 
#include <Winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#endif
#pragma comment(lib,"LibScilab.lib")

#include <scicos/scicos_block4.h>

#ifdef _WIN32
	#define EXPORT_SHARED_LIB __declspec(dllexport)
#else
	#define EXPORT_SHARED_LIB
#endif

/*** Definitions ***/

/* Define type codes */
#define SCITYPE_FLOAT   1
#define SCITYPE_COMPLEX 2
#define SCITYPE_INT32   3
#define SCITYPE_INT16   4
#define SCITYPE_INT8    5
#define SCITYPE_UINT32  6
#define SCITYPE_UINT16  7
#define SCITYPE_UINT8   8

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
extern FILE* fusc_flexsim;      /* File to be used for debug purpose */

#endif /* _miscsim_h_ */
