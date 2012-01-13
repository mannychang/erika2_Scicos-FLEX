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
