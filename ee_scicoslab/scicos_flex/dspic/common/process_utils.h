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


typedef struct process_data
{
	HANDLE handle_;
	char* name_;
	char** parameters_;
	int nparameters_;
	int last_error_code_;
	char* last_error_str_;
}process_data;

void init_process(struct process_data* proc);

void build_process(struct process_data* proc, const char* name, 
				  const char** parameters, int npars);

void clean_process(struct process_data* proc);

int launch_process(struct process_data* proc);

#endif

