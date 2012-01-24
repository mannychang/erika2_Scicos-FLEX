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
 
#include "process_utils.h"

#define STR_TERM_SIZE 1
#define STR_SPACE_SIZE 1

static void build_error(struct process_data* proc)
{
	proc->last_error_code_ = GetLastError();
}

static char* build_parameters(struct process_data* proc)
{
	char* parameters;
	int i;
	parameters = malloc(1);
	strcpy(parameters, "");
	for(i = 0; i < proc->nparameters_; ++i)
	{
		parameters = realloc(parameters, strlen(parameters) + 
										 strlen(proc->parameters_[i]) + 
										 STR_TERM_SIZE + STR_SPACE_SIZE);
		strcat(parameters, proc->parameters_[i]);
		strcat(parameters, " ");
	}
	return parameters;
}

void init_process(struct process_data* proc)
{
	proc->handle_ = INVALID_HANDLE_VALUE;
	proc->last_error_code_ = -1;
	proc->last_error_str_ = 0;
	proc->name_ = 0;
	proc->nparameters_ = 0;
	proc->parameters_ = 0;
}

void build_process(struct process_data* proc, const char* name, 
				  const char** parameters, int npars)
{
	int i, name_size, param_size;
	char* param = 0;
	name_size = strlen(name) + STR_TERM_SIZE + 2; /*"+2" for the quoted strings*/
	proc->name_ = malloc(name_size); 
	strncpy(proc->name_, "\"", 1);
	strncpy(proc->name_+1, name, strlen(name));
	proc->name_[name_size-2] = '\"';
	proc->name_[name_size-1] = '\0';
	/*The first parameter is the process name*/
	proc->nparameters_ = npars + 1;
	proc->parameters_ = malloc(proc->nparameters_ * sizeof(char*));
	proc->parameters_[0] = malloc(strlen(proc->name_) + STR_TERM_SIZE);
	strcpy(proc->parameters_[0], proc->name_);
	for(i = 0; i < npars; ++i)
	{
		param_size = strlen(parameters[i]) + STR_TERM_SIZE + 2;/*"+2" for the quoted strings*/
		param = malloc(param_size);
		strncpy(param, "\"", 1);
		strcpy(param + 1, parameters[i]);
		param[param_size-2] = '\"';
		param[param_size-1] = '\0';
		proc->parameters_[i+1] = param;
	}
}

void clean_process(struct process_data* proc)
{
	int i;
	if (proc->handle_ != INVALID_HANDLE_VALUE)
	{
		CloseHandle(proc->handle_);
		proc->handle_ = INVALID_HANDLE_VALUE;
	}
	free(proc->name_);
	for(i = 0; i < proc->nparameters_; ++i)
	{
		free(proc->parameters_[i]);
	}
	free(proc->parameters_);
}

int launch_process(struct process_data* proc)
{
	STARTUPINFO startup_info;
	PROCESS_INFORMATION process_info;
	BOOL res = FALSE;
	char* parameters;
	memset(&startup_info, 0, sizeof(startup_info));
	memset(&process_info, 0, sizeof(process_info));
	startup_info.cb = sizeof(startup_info);
	parameters = build_parameters(proc);
	res = CreateProcess(0,//(LPTSTR)proc->name_, 
						(LPTSTR)parameters,
						0,
						0,
						FALSE,
						CREATE_DEFAULT_ERROR_MODE,
						0,
						0,
						&startup_info,
						&process_info);
	free(parameters);
	if (!res || process_info.hProcess == INVALID_HANDLE_VALUE)
	{
		build_error(proc);
		return -1;
	}
	proc->handle_ = process_info.hProcess;
	return 0;
}

