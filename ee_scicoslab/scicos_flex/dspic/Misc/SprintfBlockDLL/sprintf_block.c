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

#include "../MiscBlockDLL/miscsim.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>



#define IPAR_N_ARGUMENTS 0
#define IPAR_SIZES 1

typedef enum {
	spf_parameter_type_int = 3,
	spf_parameter_type_unsigned = 6,
	spf_parameter_type_double = 1,
	spf_parameter_type_string = -1
}spf_parameter_type;

static void spf_generic(char* dst, int size, const char* fmt, void* param, 
				 spf_parameter_type type)
{
	int n;
	assert(size >= 0);
	switch (type){
		case spf_parameter_type_int:
			n = _snprintf(dst, size, fmt, *(int*)param);
			break;
		case spf_parameter_type_unsigned:
			n = _snprintf(dst, size, fmt, *(unsigned*)param);
			break;
		case spf_parameter_type_double:
			n = _snprintf(dst, size, fmt, *(double*)param);
			break;
		case spf_parameter_type_string:
			n = _snprintf(dst, size, fmt);
			break;
		default:
			n = 0;
	}
	if (n >= 0 && n < size) {
		memset(dst+n, ' ', size - n);
	}
}

static void spf_build(char* dst, int size, const char** fmts, void** params, 
			   int* sizes, int* types, int arguments)
{
	int i, j, cur, is_string;
	for (i = 0, j = 0, cur = 0; i < arguments; ++i) {
		if (cur + sizes[i] >= size) {
			break;
		}
		is_string = (types[i] == spf_parameter_type_string);
		spf_generic(dst + cur, sizes[i], fmts[i], 
			(!is_string ? params[j] : NULL), types[i]);
		if (!is_string) {
			++j;
		}
		cur += sizes[i];
	}
	dst[(cur < size?cur:size-1)] = '\0';
}

static void initialize(scicos_block* block)
{
	int arguments = (GetIparPtrs(block))[IPAR_N_ARGUMENTS];
	int nipar = block->nipar;
	if (IPAR_SIZES + 2 * arguments > nipar) {
		Coserror("Invalid ipar size");
	}
}

static void update(scicos_block* block)
{
	char* dst = Getint8OutPortPtrs(block, 1);
	int size = GetOutPortSize(block,1,2);
	int arguments = (GetIparPtrs(block))[IPAR_N_ARGUMENTS];
	const char** fmts = (const char**)block->oparptr;
	void** params = block->inptr;
	int* sizes = &(GetIparPtrs(block))[IPAR_SIZES];
	int* types = &(GetIparPtrs(block))[IPAR_SIZES+arguments];
	spf_build(dst, size, fmts, params, sizes, types, arguments);
}

static void terminate(scicos_block* block)
{
	
}

void EXPORT_SHARED_LIB sprintf_block(scicos_block* block, int flag)
{

	switch (flag) {
	case Initialization:
		initialize(block);
		break;
	case OutputUpdate:
		update(block);
		break;
	case Ending:
		terminate(block);
		break;
	}
}