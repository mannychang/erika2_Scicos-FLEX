#ifndef DATA_MODEL_H
#define DATA_MODEL_H

#define CHAR_TYPE 0
#define SHORT_TYPE 1
#define INT_TYPE 2
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

