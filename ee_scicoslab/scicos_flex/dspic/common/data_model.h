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

