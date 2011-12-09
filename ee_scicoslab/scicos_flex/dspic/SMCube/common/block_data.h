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
