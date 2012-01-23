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
 
#ifndef DATA_MODEL_H
#define DATA_MODEL_H

#define INT8_TYPE 0
#define INT16_TYPE 1
#define INT32_TYPE 2
#define FLOAT_TYPE 3
#define DOUBLE_TYPE 4
#define TYPES_LEN 5

int dm_types_size[TYPES_LEN];

struct dm_elem_descr
{
	unsigned char* types_;
	int size_; 
};

struct dm_elem
{
	void* data_;
	int size_;
	struct dm_elem_descr description_;
};

void dm_init(struct dm_elem* data);

int dm_get_value(const struct dm_elem* data, int index, unsigned char* type,
				 void** value);

void dm_set_value(struct dm_elem* data, int index, const void* value,
				  int size);

void dm_create_elem_descr(struct dm_elem_descr* descr, unsigned char* types,
						  int size);

void dm_erase_elem_descr(struct dm_elem_descr* descr);

void dm_create_elem(struct dm_elem* data,unsigned char* types, int size);

void dm_erase_elem(struct dm_elem* data);

void dm_create_types(unsigned char** types_out, const char* types, int size);

void dm_erase_types(unsigned char** types_out);

#endif

