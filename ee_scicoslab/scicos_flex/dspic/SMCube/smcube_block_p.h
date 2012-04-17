#ifndef SMCUBE_BLOCK_P_H
#define SMCUBE_BLOCK_P_H

/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2012 Evidence S.r.l.
 *
 * This file is part of ERIKA Enterprise.
 *
 * ERIKA Enterprise is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation, 
 * (with a special exception described below).
 *
 * Linking this code statically or dynamically with other modules is
 * making a combined work based on this code.  Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * As a special exception, the copyright holders of this library give you
 * permission to link this code with independent modules to produce an
 * executable, regardless of the license terms of these independent
 * modules, and to copy and distribute the resulting executable under
 * terms of your choice, provided that you also meet, for each linked
 * independent module, the terms and conditions of the license of that
 * module.  An independent module is a module which is not derived from
 * or based on this library.  If you modify this code, you may extend
 * this exception to your version of the code, but you are not
 * obligated to do so.  If you do not wish to do so, delete this
 * exception statement from your version.
 *
 * ERIKA Enterprise is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with ERIKA Enterprise; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 * ###*E*### */

#include <stdlib.h>
#include <stdio.h>

#include "scicos/scicos_block4.h"

#include "../common/data_model.h"

#define SNPRINTF snprintf

#if defined(_WIN32) && defined(_MSC_VER)
#undef SNPRINTF
#define SNPRINTF _snprintf
#endif

#define SCICOS_INT8		5
#define SCICOS_INT16	4
#define SCICOS_INT32	3
#define SCICOS_UINT8	8
#define SCICOS_UINT16	7
#define SCICOS_UINT32	6
#define SCICOS_REAL		1

/* Declarations */
static void build_from_scicos_types(int* scicos_types, int size, dm_item_type** dm_types);
static char* int_to_string(int val);
static char* build_channel_name(const char* base_channel_name, const char* block_index);
static char** build_engine_parameters(const char *engine_file, int background,
							   const char *channel_name, const char* block_index,
							   int* size);
static void clean_engine_parameters(char** parameters, int size);
static void assign_input_data(struct dm_elem* input, scicos_block* block);
static void assign_output_data(struct dm_elem* output, scicos_block* block);
static char* get_string(scicos_block* block, int base, int length);
static char* trim_string(const char* str);
static int file_exists(const char* file_name, const char* perm);
static char* get_engine_exe(const char* path, const char* exe_name, const char* folder_sep);

/* Definitions */
void build_from_scicos_types(int* scicos_types, int size, dm_item_type** dm_types)
{
	int i;
	*dm_types = (dm_item_type*) malloc(size * sizeof(dm_item_type));
	for (i = 0; i < size; ++i) {
		switch(scicos_types[i]) {
			case SCICOS_INT8:
				(*dm_types)[i] = DM_INT8;
				break;
			case SCICOS_INT16:
				(*dm_types)[i] = DM_INT16;
				break;
			case SCICOS_INT32:
				(*dm_types)[i] = DM_INT32;
				break;
			case SCICOS_UINT8:
				(*dm_types)[i] = DM_UINT8;
				break;
			case SCICOS_UINT16:
				(*dm_types)[i] = DM_UINT16;
				break;
			case SCICOS_UINT32:
				(*dm_types)[i] = DM_UINT32;
				break;
			case SCICOS_REAL:
				(*dm_types)[i] = DM_REAL;
				break;
			default:
				break;
		}
	}
}

char* int_to_string(int val)
{
	char* sdigits = 0;
	int digits = 0;
	int tmp = val;
	do {
		++digits;
		tmp /= 10;
	} while(tmp != 0);
	sdigits = (char*)malloc(digits + 1);
	SNPRINTF(sdigits, digits + 1, "%d", val);
	return sdigits;
}

char* build_channel_name(const char* base_channel_name, const char* block_index)
{
	char* name = 0;
	name = (char*)malloc(strlen(base_channel_name) + strlen(block_index) + 1);
	strcpy(name, base_channel_name);
	strcat(name, block_index);
	return name;
}

char** build_engine_parameters(const char *engine_file, int background,
							   const char *channel_name, const char* block_index,
							   int* size)
{
	int bg = 0;
	char** parameters = 0;
	*size = 6;
	if (background) {
		*size = *size + 1;
		bg = 1;
	}
	parameters = malloc(*size * sizeof(char*));
	parameters[0] = malloc(strlen("-simulate") + 1);
	strcpy(parameters[0], "-simulate");
	if (background) {
		parameters[bg] = malloc(strlen("-bgsim") + 1);
		strcpy(parameters[bg], "-bgsim");
	}
	parameters[bg+1] = malloc(strlen("-channel") + 1);
	strcpy(parameters[bg+1], "-channel");
	parameters[bg+2] = malloc(strlen(channel_name) + 1);
	strcpy(parameters[bg+2], channel_name);
	parameters[bg+3] = malloc(strlen("-id") + 1);
	strcpy(parameters[bg+3], "-id");
	parameters[bg+4] = malloc(strlen(block_index) + 1);
	strcpy(parameters[bg+4], block_index);
	parameters[bg+5] = malloc(strlen(engine_file) + 1);
	strcpy(parameters[bg+5], engine_file);
	return parameters;
}

void clean_engine_parameters(char **parameters, int size)
{
	int i;
	for(i = 0; i < size; ++i) {
		free(parameters[i]);
	}
	free(parameters);
}

void assign_input_data(struct dm_elem *input, scicos_block* block)
{
	int i, value_size, offset;
	for(i = 0; i < input->description.size; ++i) {
		if (dm_get_offset_and_size(input, i, 0, &offset, &value_size)) {
			memcpy((char*)input->data_ptr + offset, 
				(const char*)block->inptr[i], 
				value_size * input->description.items[i].multiplicity);
		}
	}
}

void assign_output_data(struct dm_elem *output, scicos_block* block)
{
	int i, value_size, offset;
	for(i = 0; i < output->description.size; ++i) {
		if (dm_get_offset_and_size(output, i, 0, &offset, &value_size)) {
			memcpy((char*)block->outptr[i], 
				(const char*)output->data_ptr + offset, 
				value_size * output->description.items[i].multiplicity);
		}
	}
}

char* get_string(scicos_block* block, int base, int length)
{
	int i;
	char c;
	char* string;
	string = (char*)malloc(length+1);
	strcpy(string,"");
	for (i=0; i<length; ++i) {
		c = block->ipar[base+i];
		strncat(string,&c,1);
	}
	return string;
}

char* trim_string(const char* str)
{
	int size;
	int l, r;
	char* res = 0;
	if (str == 0) {
		return 0;
	}
	size = strlen(str);
	l = 0;
	r = size-1;
	while(l < size && isspace(str[l])) {
		++l;
	}
	while(r >= 0 && isspace(str[r])) {
		--r;
	}
	if (l <= r+1) {
		res = (char*)malloc(r-l+2);
		res[r-l+1] = '\0';
		strncpy(res, str+l, r-l+1);
	} else {
		res = (char*)malloc(1);
		res[0] = '\0';
	}
	return res;
}

int file_exists(const char* file_name, const char* perm)
{
	FILE* f = fopen(file_name, perm);
	if (f) {
		fclose(f);
		return 1;
	}
	return 0;
}

char* get_engine_exe(const char* path, const char* exe_name, const char* folder_sep)
{
	char* res = 0;
	int end = strlen(path)-1;
	int len = strlen(exe_name) + 2;/*folder separator + end string*/
	while (end >= 0 && (path[end] == '/' || path[end] == '\\')) {
		--end;
	}
	len += end + 1;
	res = (char*) malloc(len);
	if (res) {
		strncpy(res, path, end+1);
		strncpy(res+end+1, folder_sep, 1);
		strncpy(res+end+2, exe_name, strlen(exe_name));
		res[len-1] = '\0';
	}
	return res;
}


#endif
