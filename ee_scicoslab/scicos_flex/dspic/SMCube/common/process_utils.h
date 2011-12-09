#ifndef PROCESS_UTILS_H
#define PROCESS_UTILS_H

#if defined(WIN32)
	#include <windows.h>
#else
	#include <unistd.h>
	#if defined(HANDLE)
		#undef HANDLE
	#endif
	#define HANDLE pid_t
#endif


struct process_data 
{
	HANDLE handle_;
	char* name_;
	char** parameters_;
	int nparameters_;
	int last_error_code_;
	char* last_error_str_;
};

void init_process(struct process_data* proc);

void build_process(struct process_data* proc, const char* name, 
				  const char** parameters, int npars);

void clean_process(struct process_data* proc);

int launch_process(struct process_data* proc);

#endif

