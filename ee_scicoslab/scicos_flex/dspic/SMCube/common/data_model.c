#include "data_model.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int dm_types_size[] = {sizeof(char), sizeof(short), sizeof(int), sizeof(float),
				   sizeof(double)};

void dm_init(struct dm_elem* data)
{
	data->data_ = 0;
	data->size_ = 0;
	data->description_.types_ = 0;
	data->description_.size_ = 0;
}

int dm_get_value(const struct dm_elem* data, int index, unsigned char* type,
			  void** value)
{
	int offset = 0, i, value_size;
	if (index < 0 || index >= data->description_.size_)
	{
		return 0;
	}
	for(i = 0; i < index; ++i)
	{
		assert(data->description_.types_[i] <
			   (sizeof(dm_types_size)/sizeof(int)));
		if (data->description_.types_[i] <
				(sizeof(dm_types_size)/sizeof(int)))
		{
			offset = offset + dm_types_size[data->description_.types_[i]];
		}
	}
	*type = data->description_.types_[index];
	assert(*type < (sizeof(dm_types_size)/sizeof(int)));
	if (*type < (sizeof(dm_types_size)/sizeof(int)))
	{
		value_size = dm_types_size[*type];
	}
	else
	{
		return 0;
	}
	*value = malloc(value_size);
	assert(offset+value_size <= data->size_);
	if (offset+value_size <= data->size_)
	{
		memcpy(*value, (const char*)data->data_+offset, value_size);
	}
	return value_size;
}

void dm_set_value(struct dm_elem *data, int index, const void *value, int size)
{
	int offset = 0, i, value_size;
	unsigned char type;
	if (index < 0 || index >= data->description_.size_)
	{
		return;
	}
	for(i = 0; i < index; ++i)
	{
		assert(data->description_.types_[i] <
			   (sizeof(dm_types_size)/sizeof(int)));
		if (data->description_.types_[i] <
				(sizeof(dm_types_size)/sizeof(int)))
		{
			offset = offset + dm_types_size[data->description_.types_[i]];
		}
	}
	type = data->description_.types_[index];
	assert(type < (sizeof(dm_types_size)/sizeof(int)));
	if (type < (sizeof(dm_types_size)/sizeof(int)))
	{
		value_size = dm_types_size[type];
	}
	assert(value_size == size && offset < data->size_);
	if (value_size == size && offset+size <= data->size_)
	{
		memcpy((char*)data->data_+offset, value, size);
	}
}

void dm_create_elem_descr(struct dm_elem_descr* descr, unsigned char* types,
						   int size)
{
	descr->types_ = malloc(size);
	descr->size_ = size;
	memcpy(descr->types_, types, size);
}

void dm_erase_elem_descr(struct dm_elem_descr* descr)
{
	free(descr->types_);
	descr->types_ = 0;
	descr->size_ = 0;
}

void dm_create_elem(struct dm_elem *data, unsigned char* types,
					int size)
{
	int i, data_size = 0;
	unsigned char type;
	dm_create_elem_descr(&data->description_, types, size);
	for(i = 0; i < size; ++i)
	{
		type = data->description_.types_[i];
		assert(type < (sizeof(dm_types_size)/sizeof(int)));
		if (type < (sizeof(dm_types_size)/sizeof(int)))
		{
			data_size += dm_types_size[type];
		}
	}
	if (data_size > 0)
	{
		data->data_ = malloc(data_size);
		data->size_ = data_size;
	}
	else
	{
		data->data_ = 0;
		data->size_ = 0;
	}
}

void dm_erase_elem(struct dm_elem *data)
{
	dm_erase_elem_descr(&data->description_);
	free(data->data_);
	data->data_ = 0;
	data->size_ = 0;
}

void dm_create_types(unsigned char **types_out, const char *types, int size)
{
	int i;
	int j=0;
	for(i=0; i<size; ++i)
	{
		if (types[i] == 'c')
		{
			*types_out = realloc(*types_out, j+1);
			(*types_out)[j] = CHAR_TYPE;
			++j;
		}
		else if (types[i] == 's')
		{
			*types_out = realloc(*types_out, j+1);
			(*types_out)[j] = SHORT_TYPE;
			++j;
		}
		else if (types[i] == 'i')
		{
			*types_out = realloc(*types_out, j+1);
			(*types_out)[j] = INT_TYPE;
			++j;
		}
		else if (types[i] == 'f')
		{
			*types_out = realloc(*types_out, j+1);
			(*types_out)[j] = FLOAT_TYPE;
			++j;
		}
		else if (types[i] == 'd')
		{
			*types_out = realloc(*types_out, j+1);
			(*types_out)[j] = DOUBLE_TYPE;
			++j;
		}
	}
}

void dm_erase_types(unsigned char **types_out)
{
	free(*types_out);
	*types_out = 0;
}
