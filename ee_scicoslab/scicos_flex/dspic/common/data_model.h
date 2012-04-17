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

typedef enum {
	DM_INT8 = 0,
	DM_INT16,
	DM_INT32,
	DM_UINT8,
	DM_UINT16,
	DM_UINT32,
	DM_REAL,
	DM_N_TYPES
}dm_item_type; 

int dm_types_size[DM_N_TYPES];
int dm_type_max_size;

struct dm_item {
	dm_item_type type;
	int multiplicity;
};

struct dm_elem_descr {
	struct dm_item* items;
	int size; // number of items
};

struct dm_elem {
	void* data_ptr;
	int data_size;
	struct dm_elem_descr description;
};

void dm_init(struct dm_elem* data);

// [WARNING] Use the "dm_max_type_size" to store enough bytes for value
int dm_get_value( // [return] The size of the selected type or zero if fails  
				 const struct dm_elem* data, // [in]
				 int index, // [in] item index
				 int pos, // [in] array position of the item
				 void* value, // [out] value of the selected item at "pos"
				 int size); // [in] size of "value"

int dm_set_value( // [return] A value greather than zero if succeed or zero if fails  
				 struct dm_elem* data, // [inout]
				 int index, // [in] item index
				 int pos, // [in] array position of the item
				 const void* value, // [in] value to be written at the selected item at "pos"
				 int size); // [in] size of "value"

 void dm_create_elem(struct dm_elem* data, // [inout]
					 dm_item_type* types, // [in] the array of items types
					 int* multiplicities, // [in] the array of items multiplicities
					 int size); // [in] the number of items

#endif

