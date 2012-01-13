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
 
#ifndef ARRAY_CONTAINER_H
#define ARRAY_CONTAINER_H

#include <stdlib.h>
#include <string.h>


#define INITIAL_ALLOCATION (1)
#define NEW_ALLOCATION_INC (1)

#define ARRAY_TYPE(TYPE_NAME) \
typedef struct _container_##TYPE_NAME \
{ \
	TYPE_NAME* ptr_; \
	int items_; \
	int allocated_; \
}container_##TYPE_NAME; \
void container_init_##TYPE_NAME(container_##TYPE_NAME* c) \
{ \
	c->items_ = 0; \
	c->allocated_ = 0; \
	c->ptr_ = 0; \
} \
void container_erase_##TYPE_NAME(container_##TYPE_NAME* c) \
{ \
	free(c->ptr_); \
	c->ptr_ = 0; \
	c->items_ = 0; \
	c->allocated_ = 0; \
} \
void container_remove_##TYPE_NAME(container_##TYPE_NAME* c, int pos) \
{ \
	if (pos < 0 || pos > c->items_ - 1) \
	{ \
		return; \
	} \
	if (pos != c->items_ - 1) \
	{ \
		memmove(c->ptr_+pos, c->ptr_+pos+1, (c->items_ - pos - 1) * sizeof(TYPE_NAME)); \
	} \
	--c->items_; \
} \
int container_size_##TYPE_NAME(container_##TYPE_NAME* c) \
{ \
	return c->items_; \
} \
int container_push_back_##TYPE_NAME(container_##TYPE_NAME* c, TYPE_NAME t) \
{ \
	void* tmp; \
	int new_allocation = 0;  \
	if (c->items_ == c->allocated_) \
	{ \
		if (c->allocated_ == 0) \
		{ \
			new_allocation = INITIAL_ALLOCATION; \
		} \
		else \
		{ \
			new_allocation = NEW_ALLOCATION_INC; \
		} \
		tmp = realloc(c->ptr_, (c->allocated_ + new_allocation) * sizeof(TYPE_NAME)); \
		if (!tmp) \
		{ \
			return -1; \
		} \
		c->allocated_ += new_allocation; \
		c->ptr_ = (TYPE_NAME*)tmp; \
	} \
	c->ptr_[c->items_++] = t; \
	return c->items_ - 1; \
} \
void container_pop_back_##TYPE_NAME(container_##TYPE_NAME* c) \
{ \
	if (c->items_ == 0) \
	{ \
		return; \
	} \
	else \
	{ \
		--c->items_; \
	} \
} \
TYPE_NAME container_at_##TYPE_NAME(container_##TYPE_NAME* c, int pos) \
{ \
	return c->ptr_[pos]; \
} \
TYPE_NAME* container_at_ptr_##TYPE_NAME(container_##TYPE_NAME* c, int pos) \
{ \
	if (pos < 0 || pos > c->items_ - 1) \
	{ \
		return 0; \
	} \
	return &(c->ptr_[pos]); \
}

#define ARRAY_INSTANCE(TYPE_NAME, INSTANCE_NAME) \
container_##TYPE_NAME INSTANCE_NAME

#define ARRAY_INIT(TYPE_NAME, INSTANCE_NAME) \
	container_init_##TYPE_NAME(&INSTANCE_NAME)

#define ARRAY_ERASE(TYPE_NAME, INSTANCE_NAME) \
	container_erase_##TYPE_NAME(&INSTANCE_NAME)

#define ARRAY_REMOVE(TYPE_NAME, INSTANCE_NAME, POS) \
	container_remove_##TYPE_NAME(&INSTANCE_NAME, POS)

#define ARRAY_SIZE(TYPE_NAME, INSTANCE_NAME) \
	container_size_##TYPE_NAME(&INSTANCE_NAME)

#define ARRAY_PUSH_BACK(TYPE_NAME, INSTANCE_NAME, VALUE) \
	container_push_back_##TYPE_NAME(&INSTANCE_NAME, VALUE)

#define ARRAY_POP_BACK(TYPE_NAME, INSTANCE_NAME) \
	container_pop_back_##TYPE_NAME(&INSTANCE_NAME)

#define ARRAY_AT(TYPE_NAME, INSTANCE_NAME, POS) \
	container_at_##TYPE_NAME(&INSTANCE_NAME, POS)

#define ARRAY_AT_PTR(TYPE_NAME, INSTANCE_NAME, POS) \
	container_at_ptr_##TYPE_NAME(&INSTANCE_NAME, POS)

#endif // ARRAY_CONTAINER_H
