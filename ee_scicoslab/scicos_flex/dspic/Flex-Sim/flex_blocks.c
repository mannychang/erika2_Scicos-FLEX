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

//#include "stdafx.h"
#include "flexsim.h"

#define BLOCK_TYPE_BUTTONS 0
#define BLOCK_TYPE_LEDSLCD 1
#define BLOCK_TYPE_LEDSLCDBUZZER 2

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


//#define __NO_FLEXDMB_SIMULATOR__

#ifndef __NO_FLEXDMB_SIMULATOR__ 

#include "../common/process_utils.h"
#include "../common/comm_channel.h"
#include "block_data.h"

#define SNPRINTF snprintf
#if defined(_WIN32) && defined(_MSC_VER)
#undef SNPRINTF
#define SNPRINTF _snprintf
#endif

#define FLEX_ENV_PATH "FLEXPATH"

#define LCD_INPORT_1 9
#define LCD_INPORT_2 10
#define BUZZER_INPORT 11

#define MSG_REAL_TYPE 0
#define MSG_UINT8_TYPE 1

#define BUTTONS_CMD 0
#define LEDSLCD_CMD 1
#define LEDSLCDBUZZER_CMD 2

#define CONNECTION_TIMEOUT 15

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

typedef struct buzzer_data_
{
	double value;
}buzzer_data;

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

typedef struct ledslcdbuzzer_msg_
{
	header_msg header;
	unsigned char leds[LEDS_SIZE];
	lcd_data lcd1;
	lcd_data lcd2;
	buzzer_data buzzer;
} ledslcdbuzzer_msg;
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

void do_leds_update(scicos_block * block, unsigned char* leds);

int do_lcds_update(scicos_block* block, lcd_data* lcd1, lcd_data* lcd2);

void do_ledslcd_update(scicos_block* block);

void do_ledslcdbuzzer_update(scicos_block* block);

void FLEXSIM_LIB_API flex_blocks(scicos_block *block,int flag)
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
		if (wait_for_connect_timeout(channel, CONNECTION_TIMEOUT) == -1)
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
		else if (block_type == BLOCK_TYPE_LEDSLCDBUZZER)
		{
			do_ledslcdbuzzer_update(block);
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
	parameters = (char **)malloc(*size * sizeof(char*));
	parameters[0] = (char *)malloc(strlen("-channel") + 1);
	strcpy(parameters[0], "-channel");
	parameters[1] = (char *)malloc(strlen(channel_name) + 1);
	strcpy(parameters[1], channel_name);
	parameters[2] = (char *)malloc(strlen("-id") + 1);
	strcpy(parameters[2], "-id");
	parameters[3] = (char *)malloc(strlen(block_index) + 1);
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

void do_leds_update(scicos_block * block, unsigned char* leds)
{
	if (leds != NULL && sizeof(leds) == LEDS_SIZE)
	{
		int i;
		for(i = 0; i < LEDS_SIZE; ++i)
		{
			if (Getint8InPortPtrs(block,i+1))
			{
				leds[i] = *Getint8InPortPtrs(block,i+1);
			}
		}
	}
}

int do_lcds_update(scicos_block* block, lcd_data* lcd1, lcd_data* lcd2)
{
	if (lcd1 == NULL || lcd2 == NULL)
		return 1;
	if (GetInType(block, LCD_INPORT_1) == SCSREAL_N)
	{
		lcd1->type = MSG_REAL_TYPE;
		lcd1->size = sizeof(SCSREAL_COP);
		lcd1->value = *GetRealInPortPtrs(block,LCD_INPORT_1);
	}
	else if (GetInType(block, LCD_INPORT_1) == SCSUINT8_N)
	{
		lcd1->type = MSG_UINT8_TYPE;
		lcd1->size = (GetInPortSize(block, LCD_INPORT_1, 1) * 
			GetInPortSize(block, LCD_INPORT_1, 2)) * sizeof(SCSUINT8_COP);
		if (lcd1->size > LCD_MAX_SIZE)
		{
			Coserror("Bad port size for LCD1: %d (greater then %d).", 
				lcd1->size, LCD_MAX_SIZE);
			return 1;	
		}
		memcpy(lcd1->data, GetInPortPtrs(block, LCD_INPORT_1), lcd1->size);
	}
	else
	{
		Coserror("Bad port type for LCD1: %d.", GetInType(block, LCD_INPORT_1));
		return 1;
	}
	if (GetInType(block, LCD_INPORT_2) == SCSREAL_N)
	{
		lcd2->type = MSG_REAL_TYPE;
		lcd2->size = sizeof(SCSREAL_COP);
		lcd2->value = *GetRealInPortPtrs(block,LCD_INPORT_2);
	}
	else if (GetInType(block, LCD_INPORT_2) == SCSUINT8_N)
	{
		lcd2->type = MSG_UINT8_TYPE;
		lcd2->size = (GetInPortSize(block, LCD_INPORT_2, 1) * 
			GetInPortSize(block, LCD_INPORT_2, 2)) * sizeof(SCSUINT8_COP);
		if (lcd2->size > LCD_MAX_SIZE)
		{
			Coserror("Bad port size for LCD2: %d (greater then %d).", 
				lcd2->size, LCD_MAX_SIZE);
			return 1;	
		}
		memcpy(lcd2->data, GetInPortPtrs(block, LCD_INPORT_2), lcd2->size);
	}
	else
	{
		Coserror("Bad port type for LCD2: %d.", GetInType(block, LCD_INPORT_2));
		return 1;
	}
	return 0;
}

void do_ledslcd_update(scicos_block *block)
{
	ledslcd_msg msg;
	
	msg.header.cmd = LEDSLCD_CMD;

	do_leds_update(block, msg.leds);

	if (do_lcds_update(block, &msg.lcd1, &msg.lcd2) != 0)
		return;
	if (write_to_channel(channel, (const char*)&msg, sizeof(msg)) < 0)
	{
		Coserror("Write to channel failed for ledslcd: %d.", 
			channel->last_error_code_);
		*engine_exists = 0;
		return;
	}
}

void do_ledslcdbuzzer_update(scicos_block *block)
{
	ledslcdbuzzer_msg msg;

	msg.header.cmd = LEDSLCDBUZZER_CMD;

	do_leds_update(block, msg.leds);

	if (do_lcds_update(block, &msg.lcd1, &msg.lcd2) != 0)
		return;

	msg.buzzer.value = *GetRealInPortPtrs(block, BUZZER_INPORT);

	if (write_to_channel(channel, (const char*)&msg, sizeof(msg)) < 0)
	{
		Coserror("Write to channel failed for ledslcdbuzzer: %d.", 
			channel->last_error_code_);
		*engine_exists = 0;
		return;
	}
}

#else

void FLEXSIM_LIB_API flex_blocks(scicos_block *block,int flag)
{
	switch (flag)
	{

		/* Init */
		case 4:{
					int block_type = ipar(PAR_BLOCK_TYPE);
					if (block_type == BLOCK_TYPE_BUTTONS) {
							int i;
							for(i = 0; i < BUTTONS_SIZE; ++i)
								if (Getint8OutPortPtrs(block,i+1))
									*Getint8OutPortPtrs(block,i+1) = 0;
					}
					else if (block_type == BLOCK_TYPE_LEDSLCD) {
					}
				} break;

		/* output update */
		case 1:{
					int block_type = ipar(PAR_BLOCK_TYPE);
					if (block_type == BLOCK_TYPE_BUTTONS) {
							int i;
							for(i = 0; i < BUTTONS_SIZE; ++i)
								if (Getint8OutPortPtrs(block,i+1))
									*Getint8OutPortPtrs(block,i+1) = 0;
					}
					else if (block_type == BLOCK_TYPE_LEDSLCD) {
					}
				} break;

		/* ending */
		case 5:{
					int block_type = ipar(PAR_BLOCK_TYPE);
					if (block_type == BLOCK_TYPE_BUTTONS) {
							int i;
							for(i = 0; i < BUTTONS_SIZE; ++i)
								if (Getint8OutPortPtrs(block,i+1))
									*Getint8OutPortPtrs(block,i+1) = 0;
					}
					else if (block_type == BLOCK_TYPE_LEDSLCD) {
					}
				} break;
	}
}

#endif


