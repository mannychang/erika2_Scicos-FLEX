#include "common/process_utils.h"
#include "common/comm_channel.h"
#include "common/data_model.h"
#include "common/block_data.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "scicos/scicos_block4.h"

#define SNPRINTF snprintf

#ifdef _WIN32
#define EXPORT_SHARED_LIB __declspec(dllexport)
#else
#define EXPORT_SHARED_LIB
#endif

#if defined(_WIN32) && defined(_MSC_VER)
#undef SNPRINTF
#define SNPRINTF _snprintf
#endif

#define SMCUBE_ENV_PATH "SMCUBEPATH"

#define SIMULATION_MODE_INTERACTIVE 1
#define SIMULATION_MODE_BACKGROUND 2

#define PAR_INTERNAL_BLOCK_INDEX 0

#define PAR_SIMULATION_MODE 1

#define PAR_INPUT_DESCR_LENGTH 2
#define PAR_INPUT_DESCR_BASE (PAR_INPUT_DESCR_LENGTH+1)

/*The others parametes position is known at run time*/


#define ipar(i) (block->ipar[i])
#define iwork(i) (((int*)(*block->work))[0])


/*BLOCK DATA*/
struct process_data* process = 0;
struct comm_channel* channel = 0;
int* engine_exists = 0;
struct dm_elem* input_data = 0;
struct dm_elem* output_data = 0;
static int erase_blocks = 0;

BLOCK_INSTANCE(blocks_data);

static int assign_current_block(int block_index);

static void initialize_block_data(block_data* bd);

static char* int_to_string(int val);

static char* build_channel_name(const char* base_channel_name, const char* block_index);

/**/
static char** build_engine_parameters(const char *engine_file, int background,
							   const char *channel_name, const char* block_index,
							   int* size);

static void clean_engine_parameters(char** parameters, int size);
/* Parse input*/
static void assign_input_data(struct dm_elem* input, scicos_block* block);

/* Parse output*/
static void assign_output_data(struct dm_elem* output, scicos_block* block);

/* Parse string block parameters */
static char* get_string(scicos_block* block, int base, int length);

static char* trim_string(const char* str);

static int file_exists(const char* file_name, const char* perm);

void EXPORT_SHARED_LIB smcube_block(scicos_block *block,int flag)
{
	int block_index;
	int background;
	char* input_descr = 0;
	char* output_descr = 0;
	char* engine_path = 0;
	char* engine_file = 0;
	int par_output_descr_base;
	int par_output_descr_length;
	int par_engine_file_base;
	int par_engine_file_length;
	unsigned char* inout_types=0;
#ifdef WIN32
	const char* base_channel_name = "smcube_channel";
#else
	const char* base_channel_name = "/tmp/smcube_channel";
#endif
	char* channel_name = 0;
	char* sblock_index = 0;
	char** parameters;
	int nparameters;
	switch (flag)
	{
 	/* Init */
	case 4:{
		block_data new_block_data;
		char* str_tmp;
		initialize_block_data(&new_block_data);
		/*INITIALIZE BLOCK DATA*/
		if (erase_blocks)
		{
			BLOCK_ERASE(blocks_data);
			erase_blocks = 0;
		}
		block_index = BLOCK_PUSH(blocks_data, new_block_data);
		if (block_index < 0)
		{
			Coserror("Block initialization failed, internal error"
				"(BLOCK_PUSH).");
			goto init_error;
		}
		*block->work = scicos_malloc(sizeof(int));
		iwork(PAR_INTERNAL_BLOCK_INDEX) = block_index;
		if (assign_current_block(block_index) == 0)
		{
			Coserror("Block initialization failed, internal error"
				"(bad block_index).");
			goto init_error;
		}
		sblock_index = int_to_string(block_index);
		if (!sblock_index)
		{
			Coserror("Block initialization failed, internal error"
				"(build_block_index_str).");
			goto init_error;
		}
		channel_name = build_channel_name(base_channel_name, sblock_index);
		if (!channel_name)
		{
			Coserror("Block initialization failed, internal error"
				"(build_channel_name).");
			goto init_error;
		}
		/*SIMULATION MODE*/
		background = 0;
		if ((int)ipar(PAR_SIMULATION_MODE) == SIMULATION_MODE_BACKGROUND)
		{
			background = 1;
		}
		/*INPUT DESCRIPTION*/
		input_descr = get_string(block, PAR_INPUT_DESCR_BASE,
								 ipar(PAR_INPUT_DESCR_LENGTH));
		/*OUTPUT DESCRIPTION*/
		par_output_descr_length = PAR_INPUT_DESCR_BASE +
				ipar(PAR_INPUT_DESCR_LENGTH);
		par_output_descr_base = par_output_descr_length + 1;
		output_descr = get_string(block, par_output_descr_base,
								  ipar(par_output_descr_length));
		/*ENGINE PATH*/
		par_engine_file_length = par_output_descr_base +
				ipar(par_output_descr_length);
		par_engine_file_base = par_engine_file_length + 1;
		engine_file = get_string(block, par_engine_file_base,
								  ipar(par_engine_file_length));
		/*ENGINE FILE*/
		engine_path = getenv(SMCUBE_ENV_PATH);
		if (!engine_path)
		{
			*engine_exists = 0;
			Coserror("\"SMCUBEPATH\" environment variable not set.");
			goto init_error;
		}

		/*INITIALIZE INPUT DATA STRUCTURE*/
		str_tmp = trim_string(input_descr);
		free(input_descr);
		input_descr = str_tmp;
		dm_create_types(&inout_types, input_descr, strlen(input_descr));
		dm_create_elem(input_data, inout_types, strlen(input_descr));
		dm_erase_types(&inout_types);
		/*INITIALIZE OUTPUT DATA STRUCTURE*/
		str_tmp = trim_string(output_descr);
		free(output_descr);
		output_descr = str_tmp;
		dm_create_types(&inout_types, output_descr, strlen(output_descr));
		dm_create_elem(output_data, inout_types, strlen(output_descr));
		dm_erase_types(&inout_types);
		/*CHECK FOR ENGINE PARAMETERS*/
		*engine_exists = 1;
		if (file_exists(engine_path, "r") == 0)
		{
			*engine_exists = 0;
			Coserror("SMCube application binary file %s "
			    "error: %s.", engine_path, strerror(errno));
		}
		if (file_exists(engine_file, "r+") == 0)
		{
			*engine_exists = 0;
			Coserror("SMCube xml file %s error: %s.", 
				engine_file, strerror(errno));
		}
		if (*engine_exists == 0)
		{
			goto init_error;
		}
		/* Create and initialize channel */
		/* The write operation is from the block input while
		   the read operation is from the block output       */
		build_channel(channel, output_data->size_, input_data->size_,
						 channel_name);
		if (open_channel(channel) == -1)
		{
			*engine_exists = 0;
			Coserror("Channel open failed: %d.", channel->last_error_code_);
			goto init_error;
		}

		/* Create and launch process*/
		parameters = build_engine_parameters(engine_file, background,
											 channel_name,sblock_index,
											 &nparameters);
		build_process(process, engine_path, (const char**)parameters, nparameters);
		clean_engine_parameters(parameters, nparameters);
		if (launch_process(process) == -1)
		{
			*engine_exists = 0;
			Coserror("SMCube launch failed: %d.", process->last_error_code_);
			goto init_error;
		}
#if defined(_WIN32)
		if (wait_for_connect_timeout(channel, 5))
#else
		if (wait_for_connect(channel) == -1)
#endif
		{
			*engine_exists = 0;
			Coserror("Channel wait for connection failed: %d." ,
				channel->last_error_code_);
			goto init_error;
		}
		free(engine_file);
		free(input_descr);
		free(output_descr);
		free(channel_name);
		free(sblock_index);
		break;
init_error:
		free(engine_file);
		free(input_descr);
		free(output_descr);
		free(channel_name);
		free(sblock_index);
		dm_erase_elem(input_data);
		dm_erase_elem(output_data);
		}break;
	/* output update */
	case 1:{
		int inevents = GetNevIn(block);
		if (assign_current_block(iwork(PAR_INTERNAL_BLOCK_INDEX)) == 0)
		{
			Coserror("Block initialization failed, internal error"
				"(bad block_index from scicos_block, case 1).");
			break;
		}
		/* CHECK IF ENGINE EXISTS */
		if (*engine_exists == 0)
			break;
		/* INPUT DATA INITIALIZATION */
		assign_input_data(input_data, block);
		if (write_to_channel(channel, (char*)&inevents, sizeof(inevents)) == -1)
		{
			Coserror("Write to channel failed: %d.", channel->last_error_code_);
			*engine_exists = 0;
			break;
		}
		if (input_data->size_ && 
			write_to_channel(channel, input_data->data_, input_data->size_) == -1)
		{
			Coserror("Write to channel failed: %d.", channel->last_error_code_);
			*engine_exists = 0;
			break;
		}
		if (output_data->size_)
		{
			if (read_from_channel(channel, output_data->data_, output_data->size_) == -1)
			{
				Coserror("Read from channel failed: %d.", channel->last_error_code_);
				*engine_exists = 0;
				break;
			}
			/*BUILD OUTPUT */
			assign_output_data(output_data, block);
		}
		}break;
	/* ending */
	case 5:
		/*CLEAN SIMULATION*/
		if (assign_current_block(iwork(PAR_INTERNAL_BLOCK_INDEX)) == 0)
		{
			Coserror("Block initialization failed, internal error"
				"(bad block_index from scicos_block, case 5).");
			break;
		}
		dm_erase_elem(input_data);
		dm_erase_elem(output_data);
		clean_channel(channel);
		clean_process(process);
		if (!erase_blocks)
		{
			erase_blocks = 1;
		}
		scicos_free(*block->work);
		break;
	}
}

int assign_current_block(int block_index)
{
	block_data* bd = BLOCK_AT_PTR(blocks_data, block_index);
	if (bd == 0)
	{
		return 0;
	}
	process = &bd->process_;
	channel = &bd->channel_;
	input_data = &bd->input_;
	output_data = &bd->output_;
	engine_exists = &bd->exists_;
	return 1;
}

void initialize_block_data(block_data* bd)
{
	bd->exists_ = 0;
	init_channel(&bd->channel_);
	init_process(&bd->process_);
	dm_init(&bd->input_);
	dm_init(&bd->output_);
}

char* int_to_string(int val)
{
	char* sdigits = 0;
	int digits = 0;
	int tmp = val;
	do {
		++digits;
		tmp /= 10;
	} while(tmp != 0);
	sdigits = (char*)malloc(digits + 1);
	SNPRINTF(sdigits, digits + 1, "%d", val);
	return sdigits;
}

char* build_channel_name(const char* base_channel_name, const char* block_index)
{
	char* name = 0;
	name = (char*)malloc(strlen(base_channel_name) + strlen(block_index) + 1);
	strcpy(name, base_channel_name);
	strcat(name, block_index);
	return name;
}

char** build_engine_parameters(const char *engine_file, int background,
							   const char *channel_name, const char* block_index,
							   int* size)
{
	int bg = 0;
	char** parameters = 0;
	*size = 6;
	if (background)
	{
		*size = *size + 1;
		bg = 1;
	}
	parameters = malloc(*size * sizeof(char*));
	parameters[0] = malloc(strlen("-simulate") + 1);
	strcpy(parameters[0], "-simulate");
	if (background)
	{
		parameters[bg] = malloc(strlen("-bgsim") + 1);
		strcpy(parameters[bg], "-bgsim");
	}
	parameters[bg+1] = malloc(strlen("-channel") + 1);
	strcpy(parameters[bg+1], "-channel");
	parameters[bg+2] = malloc(strlen(channel_name) + 1);
	strcpy(parameters[bg+2], channel_name);
	parameters[bg+3] = malloc(strlen("-id") + 1);
	strcpy(parameters[bg+3], "-id");
	parameters[bg+4] = malloc(strlen(block_index) + 1);
	strcpy(parameters[bg+4], block_index);
	parameters[bg+5] = malloc(strlen(engine_file) + 1);
	strcpy(parameters[bg+5], engine_file);
	return parameters;
}

void clean_engine_parameters(char **parameters, int size)
{
	int i;
	for(i = 0; i < size; ++i)
	{
		free(parameters[i]);
	}
	free(parameters);
}

void assign_input_data(struct dm_elem *input, scicos_block* block)
{
	int i, value_size, offset = 0;
	unsigned char value_type;
	for(i=0; i < input->description_.size_; ++i)
	{
		value_type = input->description_.types_[i];
		if (value_type <= sizeof(dm_types_size)/sizeof(int))
		{
			value_size = dm_types_size[value_type];
			memcpy((char*)input->data_+offset,
				   (const char*)block->inptr[i], value_size);
			offset += value_size;
		}
	}
}

void assign_output_data(struct dm_elem *output, scicos_block* block)
{
	int i, value_size, offset = 0;
	void* value;
	unsigned char value_type;
	for(i=0; i < output->description_.size_; ++i)
	{
		value_size = dm_get_value(output, i, &value_type, &value);
		if (value_size != 0 && value_size == dm_types_size[value_type])
		{
			memcpy((char*)block->outptr[i], value, value_size);
			offset += value_size;
			free(value);
			value = 0;
		}
	}
}

char* get_string(scicos_block* block, int base, int length)
{
	int i;
	char c;
        char* string;
        string = (char*)malloc(length+1);
        strcpy(string,"");
	for (i=0; i<length; ++i)
	{
		c = ipar(base+i);
                strncat(string,&c,1);
	}
        return string;
}

char* trim_string(const char* str)
{
	int size;
	int l, r;
	char* res = 0;
	if (str == 0)
	{
		return 0;
	}
	size = strlen(str);
	l = 0;
	r = size-1;
	while(l < size && isspace(str[l]))
	{
		++l;
	}
	while(r >= 0 && isspace(str[r]))
	{
		--r;
	}
	if (l <= r+1)
	{
		res = (char*)malloc(r-l+2);
		res[r-l+1] = '\0';
		strncpy(res, str+l, r-l+1);
	}
	else
	{
		res = (char*)malloc(1);
		res[0] = '\0';
	}
	return res;
}

int file_exists(const char* file_name, const char* perm)
{
	FILE* f = fopen(file_name, perm);
	if (f)
	{
		fclose(f);
		return 1;
	}
	return 0;
}
