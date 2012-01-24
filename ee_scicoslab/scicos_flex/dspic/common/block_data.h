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
 
#ifndef BLOCK_DATA_H
#define BLOCK_DATA_H

#include "array_container.h"
#include "comm_channel.h"
#include "process_utils.h"
#include "data_model.h"

typedef struct block_data_
{
	struct process_data process_;
	struct comm_channel channel_;
	int exists_;
	struct dm_elem input_;
	struct dm_elem output_;
	unsigned int output_events_;
}block_data;

#define TYPE block_data

ARRAY_TYPE(TYPE)

#define BLOCK_INSTANCE(INSTANCE) \
	ARRAY_INSTANCE(TYPE, INSTANCE)

#define BLOCK_INIT(INSTANCE) \
	ARRAY_INIT(TYPE, INSTANCE)

#define BLOCK_ERASE(INSTANCE) \
	ARRAY_ERASE(TYPE, INSTANCE)

#define BLOCK_REMOVE(INSTANCE, POS) \
	ARRAY_REMOVE(TYPE, INSTANCE, POS)

#define BLOCK_SIZE(INSTANCE) \
	ARRAY_SIZE(TYPE, INSTANCE)

#define BLOCK_PUSH(INSTANCE, VALUE) \
	ARRAY_PUSH_BACK(TYPE, INSTANCE, VALUE)

#define BLOCK_POP(INSTANCE) \
	ARRAY_POP_BACK(TYPE, INSTANCE)

#define BLOCK_AT(INSTANCE, POS) \
	ARRAY_AT(TYPE, INSTANCE, POS)

#define BLOCK_AT_PTR(INSTANCE, POS) \
	ARRAY_AT_PTR(TYPE, INSTANCE, POS)

#endif
