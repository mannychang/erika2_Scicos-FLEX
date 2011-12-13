#include "process_utils.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>

static struct sigaction old_sa;

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
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = handler;
	if (sigaction(SIGCHLD, &sa, &old_sa) == -1)
	{
		return -1;
	}
	return 0;
}

static int unregister_handler()
{
	if (sigaction(SIGCHLD, &old_sa, NULL) == -1)
	{
		return -1;
	}
	return 0;
}

void init_process(struct process_data* proc)
{
	proc->handle_ = 0;
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
	if (proc->handle_)
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
