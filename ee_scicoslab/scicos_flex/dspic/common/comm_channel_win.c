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
#include "comm_channel.h"


static void build_error(struct comm_channel* channel)
{
	channel->last_error_code_ = GetLastError();
}

void init_channel(struct comm_channel* channel)
{
	channel->handle_ = INVALID_HANDLE_VALUE;
	channel->name_ = 0;
	channel->in_buff_size_ = 0;
	channel->out_buff_size_ = 0;
	channel->disconnection_needed_ = 0;
	channel->last_error_code_ = -1;
	channel->last_error_str_ = 0;
}

void build_channel(struct comm_channel* channel, 
					   int in_size, int out_size, const char* name)
{
	const char* base_pipe_name = "\\\\.\\pipe\\";
	channel->in_buff_size_ = in_size;
	channel->out_buff_size_ = out_size;
	channel->name_ = (char*)malloc(strlen(base_pipe_name) + strlen(name) + 1);
	strcpy(channel->name_, base_pipe_name);
	strcat(channel->name_, name);
}

void clean_channel(struct comm_channel* channel)
{
	close_channel(channel);
	channel->handle_ = INVALID_HANDLE_VALUE;
	free(channel->name_);
	free(channel->last_error_str_);
	channel->name_ = 0;
	channel->last_error_str_ = 0;
}

int open_channel(struct comm_channel* channel)
{
	if (close_channel(channel) == -1)
	{
		return -1;
	}
	channel->handle_ = CreateNamedPipe((LPTSTR)channel->name_,
 									   PIPE_ACCESS_DUPLEX,
									   PIPE_TYPE_MESSAGE |
									   PIPE_READMODE_MESSAGE |
									   PIPE_WAIT,
									   1,
									   channel->out_buff_size_,
									   channel->in_buff_size_,
									   0,
									   NULL);
	if (channel->handle_ == INVALID_HANDLE_VALUE)
	{
		build_error(channel);
		return -1;
	}
	return 0;
}

int wait_for_connect(struct comm_channel* channel)
{
	BOOL res = FALSE;
	res = ConnectNamedPipe(channel->handle_, NULL);
	if (!res)
	{
		build_error(channel);
		if (channel->last_error_code_ != ERROR_PIPE_CONNECTED)
		{
			return -1;
		}
	}
	channel->disconnection_needed_ = 1;
	return 0;
}


int wait_for_connect_timeout(struct comm_channel* channel, int secs)
{
	BOOL res = FALSE;
	DWORD mode = PIPE_READMODE_MESSAGE | PIPE_NOWAIT;
	int attempts = secs > 0 ? secs : 1;
	res = SetNamedPipeHandleState(channel->handle_, &mode, NULL, NULL);
	if (!res)
	{
		build_error(channel);
		return -1;
	}
	do 
	{
		res = ConnectNamedPipe(channel->handle_, NULL);
		if (!res)
		{
			build_error(channel);
			if (channel->last_error_code_ == ERROR_PIPE_LISTENING)
			{
				if (--attempts == 0)
				{
					return -1;
				}
				Sleep(1000);
			}
			else if (channel->last_error_code_ == ERROR_PIPE_CONNECTED)
			{
				break;
			}
			else
			{
				return -1;
			}
		}
	}while(!res);
	mode = PIPE_READMODE_MESSAGE | PIPE_WAIT;
	res = SetNamedPipeHandleState(channel->handle_, &mode, NULL, NULL);
	if (!res)
	{
		build_error(channel);
		return -1;
	}
	channel->disconnection_needed_ = 1;
	return 0;
}

int connect_channel(struct comm_channel* channel)
{
	BOOL   res = FALSE; 
	DWORD  mode; 
	if (close_channel(channel) == -1)
	{
		return -1;
	}
	channel->handle_ = CreateFile((LPTSTR)channel->name_,
								 GENERIC_READ |
								 GENERIC_WRITE,
								 0,
								 NULL,
								 OPEN_EXISTING,
								 0,
								 NULL);
	if (channel->handle_ == INVALID_HANDLE_VALUE)
	{
		build_error(channel);
		return -1;
	}
	mode = PIPE_READMODE_MESSAGE; 
	res = SetNamedPipeHandleState(channel->handle_,
								  &mode,
								  NULL,
								  NULL);
	if (!res) 
	{
		build_error(channel);
		return -1;
	}
	return 0;
}

int wait_for_open(struct comm_channel* channel, int timeout)
{
	DWORD tout = NMPWAIT_WAIT_FOREVER;
	if (timeout > 0)
	{
		tout = timeout;
	}
	if (!WaitNamedPipe((LPTSTR)channel->name_, tout))
	{
		build_error(channel);
		return -1;
	}
	return 0;
}

int close_channel(struct comm_channel* channel)
{
	if (channel->handle_ != INVALID_HANDLE_VALUE)
	{
		if (channel->disconnection_needed_)
		{
			DisconnectNamedPipe(channel->handle_);
		}
		if (CloseHandle(channel->handle_) == TRUE)
		{
			return 0;
		}
		else
		{
			build_error(channel);
			return -1;
		}
	}
	return 0;
}

int write_to_channel(struct comm_channel* channel, const char* data, int size)
{
	BOOL res = FALSE;
	DWORD written;
	res = WriteFile(channel->handle_,
					(LPCVOID)data,
					(DWORD)size,
					&written,
					NULL);
	if (!res || written != size)
	{
		build_error(channel);
		return -1;
	}
	return 0;
}

int read_from_channel(struct comm_channel* channel, char* data, int max_size)
{
	BOOL res = FALSE;
	DWORD read = 0;
	DWORD to_read = (DWORD)max_size;
	do
	{
		res = ReadFile(channel->handle_,
					   (LPVOID)(data+max_size-to_read),
					   to_read,
					   &read,
					   NULL);
		to_read -= read;
	}while(res && to_read > 0);
	if (!res)
	{
		build_error(channel);
		if (channel->last_error_code_ != ERROR_MORE_DATA)
		{
			return -1;
		}
	}
	return 0;
}

int read_from_channel_size(struct comm_channel* channel, int size, char* data, int max_size)
{
	BOOL res = FALSE;
	DWORD read = 0;
	DWORD real_size = (size > max_size ? max_size:size);
	DWORD to_read = (DWORD)real_size;
	do
	{
		res = ReadFile(channel->handle_,
					   (LPVOID)(data + real_size - to_read),
					   to_read,
					   &read,
					   NULL);
		to_read -= read;
	}while(res && to_read > 0);
	if (!res)
	{
		build_error(channel);
		if (channel->last_error_code_ != ERROR_MORE_DATA)
		{
			return -1;
		}
	}
	return 0;
}
