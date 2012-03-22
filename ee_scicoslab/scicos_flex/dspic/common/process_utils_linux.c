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
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>

#define INVALID_HANDLE_VALUE -1

static struct sigaction old_sa;
static int handler_already_registered = 0;

static void build_error(struct process_data* proc)
{
	proc->last_error_code_ = errno;
	proc->last_error_str_ = strerror(errno);
}

static void clean_error(struct process_data* proc)
{
	proc->last_error_code_ = -1;
	proc->last_error_str_  = 0;
}

static void handler(int sig, siginfo_t* info, void* unused)
{
	(void)sig;
	(void)info;
	(void)unused;
	fprintf(stderr, "Child process terminated\n");
}

static int register_handler()
{
	struct sigaction sa;
	if (handler_already_registered)
	{
		return 0;
	}
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = handler;
	if (sigaction(SIGCHLD, &sa, &old_sa) == -1)
	{
		return -1;
	}
	handler_already_registered = 1;
	return 0;
}

static int unregister_handler()
{
	if (!handler_already_registered)
	{
		return 0;
	}
	if (sigaction(SIGCHLD, &old_sa, NULL) == -1)
	{
		return -1;
	}
	handler_already_registered = 0;
	return 0;
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


void build_process(struct process_data *proc, const char *name,
				  const char **parameters, int npars)
{
	int i;
	proc->name_ = malloc(strlen(name) + 1);
	strcpy(proc->name_, name);
	/*The first parameter is the process name*/
	/*The last parameters is NULL according to the execv() arguments*/
	proc->nparameters_ = npars + 2;
	proc->parameters_ = malloc(proc->nparameters_ * sizeof(char*));
	proc->parameters_[0] = malloc(strlen(proc->name_) + 1);
	strcpy(proc->parameters_[0], proc->name_);
	for(i = 0; i < npars; ++i)
	{
		proc->parameters_[i+1] = malloc(strlen(parameters[i]) + 1);
		strcpy(proc->parameters_[i+1], parameters[i]);
	}
	proc->parameters_[proc->nparameters_-1] = 0;
}

void clean_process(struct process_data *proc)
{
	HANDLE child_pid;
	int status;
	clean_error(proc);
	if (proc->handle_ != INVALID_HANDLE_VALUE)
	{
		if (kill(proc->handle_, SIGTERM) < 0)
		{
			build_error(proc);
			return;
		}
		do
		{
			child_pid = wait(&status);
			if (child_pid < 0)
			{
				unregister_handler();
				build_error(proc);
				return;
			}
		}while(child_pid != proc->handle_);
	}
	unregister_handler();
}

int launch_process(struct process_data *proc)
{
	HANDLE pid;
	if (access(proc->name_, X_OK) == -1)
	{
		build_error(proc);
		return -1;
	}
	register_handler();
	pid = fork();
	if (pid < 0)
	{
		unregister_handler();
		build_error(proc);
		return -1;
	}
	if (pid == 0) /*child process, launch engine*/
	{
		execv(proc->name_, proc->parameters_);
		/* NEVER RETURNS OTHERWISE AN ERROR OCCURRED */
		fprintf(stderr,"Error while launching child process %d %s\n", errno,
				strerror(errno));
		exit(0);
	}
	/*parent process, save child pid*/
	proc->handle_ = pid;
	return 0;
}
