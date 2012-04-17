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

#include "data_model.h"
#if defined(_MSC_VER)
	typedef __int32 int32_t;
	typedef __int16 int16_t;
	typedef __int8 int8_t;
	typedef unsigned __int32 uint32_t;
	typedef unsigned __int16 uint16_t;
	typedef unsigned __int8 uint8_t;
#elif defined(__GNUC__) && defined(__INT8_TYPE__) 
	#include <stdint.h>
#else
	#error "Compiler not supported"
#endif
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define RET_OK 1
#define RET_FAIL 1

int dm_types_size[] = {sizeof(int8_t), sizeof(int16_t), sizeof(int32_t),
					   sizeof(uint8_t), sizeof(uint16_t), sizeof(uint32_t),
					   sizeof(double)};

int dm_type_max_size = (sizeof(double) > sizeof(int32_t) ? sizeof(double) : sizeof(int32_t));

/* Private */
static void create_elem_descr(struct dm_elem_descr* descr, dm_item_type* types, int* multiplicities, int size);
static void erase_elem_descr(struct dm_elem_descr* descr);
static int get_type_size(dm_item_type type);

/* Public */
void dm_init(struct dm_elem* data)
{
	data->data_ptr = NULL;
	data->data_size = 0;
	data->description.items = NULL;
	data->description.size = 0;
}

int dm_get_value(const struct dm_elem* data, int index, int pos, void* value, int size)
{
	int offset, value_size, result = RET_OK;
	assert(data);
	if (dm_get_offset_and_size(data, index, pos, &offset, &value_size)) {
		/* check if value has enough space for result */
		if (value_size <= size) {
			memcpy(value, (const char*)data->data_ptr+offset, value_size);
		} else {
			result = RET_FAIL;
		}
	} else {
		result = RET_FAIL;
	}
	if (!result) {
		value_size = 0;
	}
	return value_size;
}

int dm_set_value(struct dm_elem* data, int index, int pos, const void* value, int size)
{
	int offset, value_size, result = RET_OK;
	assert(data);
	if (dm_get_offset_and_size(data, index, pos, &offset, &value_size)) {
		/* check if value has enough space for result */
		if (value_size == size) {
			memcpy((char*)data->data_ptr+offset, value, value_size);
		} else {
			result = RET_FAIL;
		}
	} else {
		result = RET_FAIL;
	}
	return result;
}

void dm_create_elem(struct dm_elem *data, dm_item_type* types, int* multiplicities, int size)
{
	int i, data_size = 0;
	create_elem_descr(&data->description, types, multiplicities, size);
	for(i = 0; i < size; ++i) {
		data_size += get_type_size(data->description.items[i].type) *
			data->description.items[i].multiplicity;
	}
	if (data_size > 0) {
		data->data_ptr = malloc(data_size);
		data->data_size = data_size;
	} else {
		data->data_ptr = NULL;
		data->data_size = 0;
	}
}

void dm_erase_elem(struct dm_elem *data)
{
	erase_elem_descr(&data->description);
	free(data->data_ptr);
	data->data_ptr = NULL;
	data->data_size = 0;
}

int dm_get_offset_and_size(const struct dm_elem* data, int index, int pos, int* offset, int* size)
{
	int i, value_size, offset_value = 0;
	struct dm_item* selected_item = NULL;
	/* Check for index and pos bounds */
	if ((index < 0 || index >= data->description.size) || 
		(pos < 0 || pos >= data->description.items[index].multiplicity)) {
		return RET_FAIL;
	}
	/* move offset to selected item */
	for (i = 0; i < index; ++i) {
		offset_value += get_type_size(data->description.items[i].type) * 
			data->description.items[i].multiplicity;
	}
	selected_item = &(data->description.items[index]);
	value_size = get_type_size(selected_item->type);

	/* move offset to the value position of the selected item */
	offset_value += pos * value_size;
	/* check for offset out of bound */
	assert(offset_value+value_size <= data->data_size);
	*offset = offset_value;
	*size = value_size;
	return RET_OK;
}

/* PRIVATE */
void create_elem_descr(struct dm_elem_descr* descr, dm_item_type* types, int* multiplicities, int size)
{	
	int i, n;
	if (size > 0) {
		descr->items = (struct dm_item*) malloc(size * sizeof(struct dm_item));
		descr->size = size;
		for (i = 0, n = descr->size; i < n; ++i) {
			descr->items[i].type = types[i];
			descr->items[i].multiplicity = multiplicities[i];
		}
	}
}

void erase_elem_descr(struct dm_elem_descr* descr)
{
	free(descr->items);
	descr->items = NULL;
	descr->size = 0;
}

int get_type_size(dm_item_type type)
{
	assert(type < (sizeof(dm_types_size)/sizeof(int)));
	return dm_types_size[type];
}
