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

#include "../common/process_utils.h"
#include "../common/comm_channel.h"
#include "../common/data_model.h"
#include "../common/block_data.h"
#include "smcube_block_p.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "scicos/scicos_block4.h"

#ifdef _WIN32
#define EXPORT_SHARED_LIB __declspec(dllexport)
static const char* executable_name = "SMCube.exe";
static const char* folder_separator = "\\";
#else
#define EXPORT_SHARED_LIB
static const char* executable_name = "SMCube";
static const char* folder_separator = "/";
#endif

#define SMCUBE_ENV_PATH "SMCUBEPATH"

#define SIMULATION_MODE_INTERACTIVE 1
#define SIMULATION_MODE_BACKGROUND 2

#define PAR_INTERNAL_BLOCK_INDEX 0

#define PAR_SIMULATION_MODE 1

#define PAR_ENGINE_FILE_LENGTH 2
#define PAR_ENGINE_FILE_BASE (PAR_ENGINE_FILE_LENGTH+1)

/*The others parametes position is known at run time*/


#define ipar(i) (block->ipar[i])
#define iwork(i) (((int*)(*block->work))[0])

#define CHECK_OUTPUT_EVENT(events, evt) \
	( events & ((unsigned int)1 << evt) )

#define CONNECTION_TIMEOUT 15

/*BLOCK DATA*/
struct process_data* process = 0;
struct comm_channel* channel = 0;
int* engine_exists = 0;
struct dm_elem* input_data = 0;
struct dm_elem* output_data = 0;
static int erase_blocks = 0;

BLOCK_INSTANCE(blocks_data);

static int assign_current_block(int block_index);

static int get_output_events(int block_index, unsigned int* evtout);

static int set_output_events(int block_index, unsigned int evtout);

static void initialize_block_data(block_data* bd);

void EXPORT_SHARED_LIB smcube_block(scicos_block *block,int flag)
{
	int block_index;
	int background;
	char* engine_path = 0;
	char* engine_exe = 0;
	char* engine_file = 0;
	dm_item_type* inout_types;
#ifdef WIN32
	const char* base_channel_name = "smcube_channel";
#else
	const char* base_channel_name = "/tmp/smcube_channel";
#endif
	char* channel_name = 0;
	char* sblock_index = 0;
	char** parameters;
	int nparameters;
	switch (flag) {
 	/* Init */
	case 4:{
		block_data new_block_data;
		initialize_block_data(&new_block_data);
		/*INITIALIZE BLOCK DATA*/
		if (erase_blocks) {
			BLOCK_ERASE(blocks_data);
			erase_blocks = 0;
		}
		block_index = BLOCK_PUSH(blocks_data, new_block_data);
		if (block_index < 0) {
			Coserror("Block initialization failed, internal error"
				"(BLOCK_PUSH).");
			goto init_error;
		}
		*block->work = scicos_malloc(sizeof(int));
		iwork(PAR_INTERNAL_BLOCK_INDEX) = block_index;
		if (assign_current_block(block_index) == 0) {
			Coserror("Block initialization failed, internal error"
				"(bad block_index).");
			goto init_error;
		}
		sblock_index = int_to_string(block_index);
		if (!sblock_index) {
			Coserror("Block initialization failed, internal error"
				"(build_block_index_str).");
			goto init_error;
		}
		channel_name = build_channel_name(base_channel_name, sblock_index);
		if (!channel_name) {
			Coserror("Block initialization failed, internal error"
				"(build_channel_name).");
			goto init_error;
		}
		/*SIMULATION MODE*/
		background = 0;
		if ((int)ipar(PAR_SIMULATION_MODE) == SIMULATION_MODE_BACKGROUND) {
			background = 1;
		}
		/*ENGINE PATH*/
		engine_file = get_string(block, PAR_ENGINE_FILE_BASE, ipar(PAR_ENGINE_FILE_BASE));
		/*ENGINE FILE*/
		engine_path = getenv(SMCUBE_ENV_PATH);
		if (!engine_path) {
			*engine_exists = 0;
			Coserror("\"SMCUBEPATH\" environment variable not set.");
			goto init_error;
		}
		engine_exe = get_engine_exe(engine_path, executable_name, folder_separator);
		if (!engine_exe) {
			*engine_exists = 0;
			Coserror("Error while building executable name of SMCube.");
			goto init_error;		
		}
		/*INITIALIZE INPUT DATA STRUCTURE*/
		build_from_scicos_types(&(block->insz[2*block->nin]), block->nin, &inout_types); 
		dm_create_elem(input_data, inout_types, &(block->insz[0]), block->nin);
		free(inout_types);
		/*INITIALIZE OUTPUT DATA STRUCTURE*/
		build_from_scicos_types(&(block->outsz[2*block->nin]), block->nout, &inout_types); 
		dm_create_elem(output_data, inout_types, &(block->outsz[0]), block->nout);
		free(inout_types);
		/*CHECK FOR ENGINE PARAMETERS*/
		*engine_exists = 1;
		if (file_exists(engine_exe, "r") == 0) {
			*engine_exists = 0;
			Coserror("SMCube application binary file %s "
			    "error: %s.", engine_exe, strerror(errno));
		}
		if (file_exists(engine_file, "r+") == 0) {
			*engine_exists = 0;
			Coserror("SMCube xml file %s error: %s.", 
				engine_file, strerror(errno));
		}
		if (*engine_exists == 0) {
			goto init_error;
		}
		/* Create and initialize channel */
		/* The write operation is from the block input while
		   the read operation is from the block output       */
		build_channel(channel, output_data->data_size, input_data->data_size,
						 channel_name);
		if (open_channel(channel) == -1) {
			*engine_exists = 0;
			Coserror("Channel open failed: %d.", channel->last_error_code_);
			goto init_error;
		}

		/* Create and launch process*/
		parameters = build_engine_parameters(engine_file, background,
											 channel_name,sblock_index,
											 &nparameters);
		build_process(process, engine_exe, (const char**)parameters, nparameters);
		clean_engine_parameters(parameters, nparameters);
		if (launch_process(process) == -1) {
			*engine_exists = 0;
			Coserror("SMCube launch failed: %d.", process->last_error_code_);
			goto init_error;
		}
		if (wait_for_connect_timeout(channel, CONNECTION_TIMEOUT)) {
			*engine_exists = 0;
			Coserror("Channel wait for connection failed: %d." ,
				channel->last_error_code_);
			goto init_error;
		}
		free(engine_exe);
		free(engine_file);
		free(channel_name);
		free(sblock_index);
		break;
init_error:
		free(engine_exe);
		free(engine_file);
		free(channel_name);
		free(sblock_index);
		dm_erase_elem(input_data);
		dm_erase_elem(output_data);
	}break;
	/* output update */
	case 1:{
		int inevents = GetNevIn(block);
		unsigned int evtout = 0;
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
		if (write_to_channel(channel, (char*)&inevents, sizeof(inevents)) == -1) {
			Coserror("Write to channel failed: %d.", channel->last_error_code_);
			*engine_exists = 0;
			break;
		}
		if (input_data->data_size && 
			write_to_channel(channel, input_data->data_ptr, input_data->data_size) == -1) {
			Coserror("Write to channel failed: %d.", channel->last_error_code_);
			*engine_exists = 0;
			break;
		}
		if (output_data->data_size) {
			if (read_from_channel_size(channel, output_data->data_size, 
					output_data->data_ptr, output_data->data_size) == -1) {
				Coserror("Read from channel failed: %d.", channel->last_error_code_);
				*engine_exists = 0;
				break;
			}
			/*BUILD OUTPUT */
			assign_output_data(output_data, block);
		}
		if (read_from_channel_size(channel, sizeof(evtout), (char*)&evtout, sizeof(evtout)) == -1) {
			Coserror("Read from channel failed: %d.", channel->last_error_code_);
			*engine_exists = 0;
			break;
		}
		if (!set_output_events(iwork(PAR_INTERNAL_BLOCK_INDEX), evtout)) {
			Coserror("Internal error(on set_output_events): unknown block %d", 
				iwork(PAR_INTERNAL_BLOCK_INDEX));
			*engine_exists = 0;
			break;		
		}
	}break;
	/* output events update */
	case 3:{
		unsigned int evtsout = 0;
		int i;
		if (!get_output_events(iwork(PAR_INTERNAL_BLOCK_INDEX), &evtsout)) {
			Coserror("Internal error(on set_output_events): unknown block %d", 
				iwork(PAR_INTERNAL_BLOCK_INDEX));
			break;
		}
		for (i = 0; i < block->nevout; ++i) {
			if (CHECK_OUTPUT_EVENT(evtsout, i)) {
				block->evout[i] = 0;
			}
		}
	}break;
	/* ending */
	case 5:
		/*CLEAN SIMULATION*/
		if (assign_current_block(iwork(PAR_INTERNAL_BLOCK_INDEX)) == 0) {
			Coserror("Block initialization failed, internal error"
				"(bad block_index from scicos_block, case 5).");
			break;
		}
		dm_erase_elem(input_data);
		dm_erase_elem(output_data);
		clean_channel(channel);
		clean_process(process);
		if (!erase_blocks) {
			erase_blocks = 1;
		}
		scicos_free(*block->work);
		break;
	}
}

int assign_current_block(int block_index)
{
	block_data* bd = BLOCK_AT_PTR(blocks_data, block_index);
	if (bd == 0) {
		return 0;
	}
	process = &bd->process_;
	channel = &bd->channel_;
	input_data = &bd->input_;
	output_data = &bd->output_;
	engine_exists = &bd->exists_;
	return 1;
}

int get_output_events(int block_index, unsigned int* evtout)
{
	block_data* bd = BLOCK_AT_PTR(blocks_data, block_index);
	if (bd == 0) {
		return 0;
	}
	*evtout = bd->output_events_;
	return 1;
}

int set_output_events(int block_index, unsigned int evtout)
{
	block_data* bd = BLOCK_AT_PTR(blocks_data, block_index);
	if (bd == 0) {
		return 0;
	}
	bd->output_events_ = evtout;
	return 1;
}


void initialize_block_data(block_data* bd)
{
	bd->exists_ = 0;
	init_channel(&bd->channel_);
	init_process(&bd->process_);
	dm_init(&bd->input_);
	dm_init(&bd->output_);
	bd->output_events_ = 0;
}

