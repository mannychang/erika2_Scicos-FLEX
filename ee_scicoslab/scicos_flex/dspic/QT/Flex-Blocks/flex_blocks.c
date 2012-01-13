#include "../common/process_utils.h"
#include "../common/comm_channel.h"
#include "block_data.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
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

#define FLEX_ENV_PATH "FLEXPATH"
#define BLOCK_TYPE_BUTTONS 0
#define BLOCK_TYPE_LEDSLCD 1

#define PAR_BLOCK_TYPE 0
#define PAR_BLOCK_INDEX 1

#define ipar(i) (block->ipar[i])

#define BUTTON_1 0
#define BUTTON_2 1
#define BUTTON_3 2
#define BUTTON_4 3
#define BUTTONS_SIZE 4
#define LED_0 0
#define LED_1 1
#define LED_2 2
#define LED_3 3
#define LED_4 4
#define LED_5 5
#define LED_6 6
#define LED_7 7
#define LEDS_SIZE 8
#define LCD_MAX_SIZE 16 /*in case of array of uint8_t*/
#define LCD_INPORT_1 9
#define LCD_INPORT_2 10

#define MSG_REAL_TYPE 0
#define MSG_UINT8_TYPE 1

#define BUTTONS_CMD 0
#define LEDSLCD_CMD 1

#pragma pack(push,1)
typedef struct flex_buttons_data_ {
	unsigned char buttons[BUTTONS_SIZE];
}flex_buttons_data;

typedef struct header_msg_ 
{
	unsigned char cmd;
}header_msg;

typedef struct buttons_req_msg_
{
	header_msg header;
}buttons_req_msg;

typedef struct lcd_data_
{
	unsigned char type;
	unsigned char size;
	unsigned char data[LCD_MAX_SIZE];
	double value;
}lcd_data;

typedef struct ledslcd_msg_
{
	header_msg header;
	unsigned char leds[LEDS_SIZE];
	lcd_data lcd1;
	lcd_data lcd2;
}ledslcd_msg;
#pragma pack(pop)

/*BLOCK DATA*/
struct process_data* process = 0;
struct comm_channel* channel = 0;
int* engine_exists = 0;

BLOCK_INSTANCE(blocks_data);

int assign_current_block(int block_index);

void initialize_block_data(block_data* bd);

void remove_block_data(int block_index);

char* int_to_string(int block_index);

char* build_channel_name(const char* base_channel_name, const char* block_index);

/**/
char** build_engine_parameters(const char *channel_name, const char* block_index,
							   int* size);

void clean_engine_parameters(char** parameters, int size);

int file_exists(const char* file_name, const char* perm);

void do_buttons_update(scicos_block* block);

void do_ledslcd_update(scicos_block* block);

void EXPORT_SHARED_LIB flex_blocks(scicos_block *block,int flag)
{
	char* engine_path = 0;
#ifdef WIN32
	const char* base_channel_name = "flex_blocks_channel";
#else
	const char* base_channel_name = "/tmp/flex_blocks_channel";
#endif
	char* channel_name = 0;
	char* sblock_index = 0;
	char** parameters;
	int nparameters;
	switch (flag)
	{
 	/* Init */
	case 4:{
		int block_index = ipar(PAR_BLOCK_INDEX);
		int res;
		block_data new_block_data;
		initialize_block_data(&new_block_data);
		/*INITIALIZE BLOCK DATA*/
		if (BLOCK_AT_PTR(blocks_data, block_index))
		{
			break;
		}
		new_block_data.id_ = block_index;
		res = BLOCK_PUSH(blocks_data, new_block_data);
		if (res < 0)
		{
			Coserror("Block initialization failed, internal error"
				"(BLOCK_PUSH).");
			goto init_error;
		}
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
		/*ENGINE FILE*/
		engine_path = getenv(FLEX_ENV_PATH);
		if (!engine_path)
		{
			*engine_exists = 0;
			Coserror("\"FLEXPATH\" environment variable not set.");
			goto init_error;
		}
		/*CHECK FOR ENGINE PARAMETERS*/
		*engine_exists = 1;
		if (file_exists(engine_path, "r") == 0)
		{
			*engine_exists = 0;
			Coserror("FlexDemoBoard application binary file %s "
			    "error: %s.", engine_path, strerror(errno));
		}
		if (*engine_exists == 0)
		{
			goto init_error;
		}
		/* Create and initialize channel */
		build_channel(channel, sizeof(flex_buttons_data), sizeof(ledslcd_msg), channel_name);
		if (open_channel(channel) == -1)
		{
			*engine_exists = 0;
			Coserror("Channel open failed: %d.", channel->last_error_code_);
			goto init_error;
		}

		/* Create and launch process*/
		parameters = build_engine_parameters(channel_name,sblock_index,
											 &nparameters);
		build_process(process, engine_path, (const char**)parameters, nparameters);
		clean_engine_parameters(parameters, nparameters);
		if (launch_process(process) == -1)
		{
			*engine_exists = 0;
			Coserror("FlexDemoBoard launch failed: %d.", process->last_error_code_);
			goto init_error;
		}
		if (wait_for_connect_timeout(channel, 5) == -1)
		{
			*engine_exists = 0;
			Coserror("Channel wait for connection failed: %d." ,
				channel->last_error_code_);
			goto init_error;
		}
		free(channel_name);
		free(sblock_index);
		break;
init_error:
		free(channel_name);
		free(sblock_index);
		clean_process(process);
		clean_channel(channel);
		}break;
	/* output update */
	case 1:{
		int block_type = ipar(PAR_BLOCK_TYPE);
		if (assign_current_block(ipar(PAR_BLOCK_INDEX)) == 0)
		{
			Coserror("Block initialization failed, internal error"
				"(bad block_index from scicos_block, case 1).");
			break;
		}
		if (*engine_exists == 0)
		{
			break;
		}
		if (block_type == BLOCK_TYPE_BUTTONS)
		{
			do_buttons_update(block);
		}
		else if (block_type == BLOCK_TYPE_LEDSLCD)
		{
			do_ledslcd_update(block);
		}
		}break;
	/* ending */
	case 5:
		if (assign_current_block(ipar(PAR_BLOCK_INDEX)) == 0)
		{
			break;
		}
		clean_channel(channel);
		clean_process(process);
		remove_block_data(ipar(PAR_BLOCK_INDEX));
		break;
	}
}

int assign_current_block(int block_index)
{
	block_data* bd = 0;
	int i,s;
	for(i = 0, s = BLOCK_SIZE(blocks_data); i < s; ++i)
	{
		bd = BLOCK_AT_PTR(blocks_data, i);
		if (bd && bd->id_ == block_index)
		{
			break;
		}
		bd = 0;
	}
	if (bd)
	{
		process = &bd->process_;
		channel = &bd->channel_;
		engine_exists = &bd->exists_;
		return 1;
	}
	return 0;
}

void initialize_block_data(block_data* bd)
{
	bd->id_ = -1;
	bd->exists_ = 0;
	init_channel(&bd->channel_);
	init_process(&bd->process_);
}

void remove_block_data(int block_index)
{
	block_data* bd = 0;
	int i,s;
	for(i = 0, s = BLOCK_SIZE(blocks_data); i < s; ++i)
	{
		bd = BLOCK_AT_PTR(blocks_data, i);
		if (bd && bd->id_ == block_index)
		{
			BLOCK_REMOVE(blocks_data, i);
			break;
		}
	}
}

char* int_to_string(int block_index)
{
	char* sdigits = 0;
	int digits = 0;
	int val = block_index;
	do {
		++digits;
		val /= 10;
	} while(val != 0);
	sdigits = (char*)malloc(digits + 1);
	SNPRINTF(sdigits, digits + 1, "%d", block_index);
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

char** build_engine_parameters(const char *channel_name, const char* block_index,
							   int* size)
{
	char** parameters = 0;
	*size = 4;
	parameters = malloc(*size * sizeof(char*));
	parameters[0] = malloc(strlen("-channel") + 1);
	strcpy(parameters[0], "-channel");
	parameters[1] = malloc(strlen(channel_name) + 1);
	strcpy(parameters[1], channel_name);
	parameters[2] = malloc(strlen("-id") + 1);
	strcpy(parameters[2], "-id");
	parameters[3] = malloc(strlen(block_index) + 1);
	strcpy(parameters[3], block_index);
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

void do_buttons_update(scicos_block *block)
{
	buttons_req_msg msg;
	flex_buttons_data data;
	int i;
	msg.header.cmd = BUTTONS_CMD;
	if (write_to_channel(channel, (const char*)&msg, sizeof(msg)) < 0)
	{
		Coserror("Write to channel failed for buttons: %d.", 
			channel->last_error_code_);
		*engine_exists = 0;
		return;
	}
	if (read_from_channel_size(channel, sizeof(data), (char*)&data, sizeof(data)) < 0)
	{
		Coserror("Read from channel failed for buttons: %d.", 
			channel->last_error_code_);
		*engine_exists = 0;
		return;
	}
	for(i = 0; i < BUTTONS_SIZE; ++i)
	{
		if (Getint8OutPortPtrs(block,i+1))
		{
			*Getint8OutPortPtrs(block,i+1) = data.buttons[i];
		}
	}
}

void do_ledslcd_update(scicos_block *block)
{
	ledslcd_msg msg;
	int i;
	msg.header.cmd = LEDSLCD_CMD;
	for(i = 0; i < LEDS_SIZE; ++i)
	{
		if (Getint8InPortPtrs(block,i+1))
		{
			msg.leds[i] = *Getint8InPortPtrs(block,i+1);
		}
	}
	if (GetInType(block, LCD_INPORT_1) == SCSREAL_N)
	{
		msg.lcd1.type = MSG_REAL_TYPE;
		msg.lcd1.size = sizeof(SCSREAL_COP);
		msg.lcd1.value = *GetRealInPortPtrs(block,LCD_INPORT_1);
	}
	else if (GetInType(block, LCD_INPORT_1) == SCSUINT8_N)
	{
		msg.lcd1.type = MSG_UINT8_TYPE;
		msg.lcd1.size = (GetInPortSize(block, LCD_INPORT_1, 1) * 
			GetInPortSize(block, LCD_INPORT_1, 2)) * sizeof(SCSUINT8_COP);
		if (msg.lcd1.size > LCD_MAX_SIZE)
		{
			Coserror("Bad port size for LCD1: %d (greater then %d).", 
				msg.lcd1.size, LCD_MAX_SIZE);
			return;	
		}
		memcpy(msg.lcd1.data, GetInPortPtrs(block, LCD_INPORT_1), msg.lcd1.size);
	}
	else
	{
		Coserror("Bad port type for LCD1: %d.", GetInType(block, LCD_INPORT_1));
		return;
	}
	if (GetInType(block, LCD_INPORT_2) == SCSREAL_N)
	{
		msg.lcd2.type = MSG_REAL_TYPE;
		msg.lcd2.size = sizeof(SCSREAL_COP);
		msg.lcd2.value = *GetRealInPortPtrs(block,LCD_INPORT_2);
	}
	else if (GetInType(block, LCD_INPORT_2) == SCSUINT8_N)
	{
		msg.lcd2.type = MSG_UINT8_TYPE;
		msg.lcd2.size = (GetInPortSize(block, LCD_INPORT_2, 1) * 
			GetInPortSize(block, LCD_INPORT_2, 2)) * sizeof(SCSUINT8_COP);
		if (msg.lcd2.size > LCD_MAX_SIZE)
		{
			Coserror("Bad port size for LCD2: %d (greater then %d).", 
				msg.lcd2.size, LCD_MAX_SIZE);
			return;	
		}
		memcpy(msg.lcd2.data, GetInPortPtrs(block, LCD_INPORT_2), msg.lcd2.size);
	}
	else
	{
		Coserror("Bad port type for LCD2: %d.", GetInType(block, LCD_INPORT_2));
		return;
	}

	if (write_to_channel(channel, (const char*)&msg, sizeof(msg)) < 0)
	{
		Coserror("Write to channel failed for ledslcd: %d.", 
			channel->last_error_code_);
		*engine_exists = 0;
		return;
	}
}

