#include "process_utils.h"
#include <string.h>

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

