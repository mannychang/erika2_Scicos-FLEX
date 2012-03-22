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
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

static int d_backlog;

static void build_error(struct comm_channel* channel)
{
	channel->last_error_code_ = errno;
	channel->last_error_str_  = strerror(errno);
}

static void clean_error(struct comm_channel* channel)
{
	channel->last_error_code_ = -1;
	channel->last_error_str_  = 0;
}

static int set_blocking(struct comm_channel* channel, HANDLE handle, int blocking)
{
	int arg, res;
	if (blocking)
	{
		arg = fcntl(handle, F_GETFL, NULL);
		if (arg < 0)
		{
			build_error(channel);
			return -1;
		}
		arg |= O_NONBLOCK;
		res = fcntl(handle, F_SETFL, arg);
		if (res < 0)
		{
			build_error(channel);
			return -1;
		}

	}
	else
	{
		arg = fcntl(handle, F_GETFL, NULL);
		if (arg < 0)
		{
			build_error(channel);
			return -1;
		}
		arg &= ~O_NONBLOCK;
		res = fcntl(handle, F_SETFD, arg);
		if (res < 0)
		{
			build_error(channel);
			return -1;
		}
	}
	return 0;
}

void init_channel(struct comm_channel* channel)
{
	channel->handle_ = -1;
	channel->name_ = 0;
	channel->in_buff_size_ = 0;
	channel->out_buff_size_ = 0;
	channel->disconnection_needed_ = -1;
	channel->last_error_code_ = -1;
	channel->last_error_str_ = 0;
	channel->listenHandle_ = -1;
	d_backlog = 1;
}

void build_channel(struct comm_channel *channel, int in_size, int out_size,
				  const char *name)
{
	channel->in_buff_size_ = in_size;
	channel->out_buff_size_ = out_size;
	channel->name_ = (char*) malloc(strlen(name) + 1);
	strcpy(channel->name_, name);

}

void clean_channel(struct comm_channel *channel)
{
	if (channel->handle_ != -1)
	{
		if (close(channel->handle_) == -1)
		{
			build_error(channel);
		}
	}
	if (channel->listenHandle_ != -1)
	{
		if (close(channel->listenHandle_) == -1)
		{
			build_error(channel);
		}
	}
}

int open_channel(struct comm_channel *channel)
{
	struct sockaddr_un s_un;
	if (channel->listenHandle_ != -1)
	{
		close(channel->listenHandle_);
	}
	channel->listenHandle_ = socket(AF_UNIX, SOCK_STREAM, 0);

	if (channel->listenHandle_ == -1)
	{
		build_error(channel);
		return -1;
	}
	memset(&s_un, 0, sizeof(s_un));
	s_un.sun_family = AF_UNIX;
	strncpy(s_un.sun_path, channel->name_, sizeof(s_un.sun_path)-1);
	unlink(channel->name_);
	if (bind(channel->listenHandle_, (const struct sockaddr*)&s_un, sizeof(s_un))
		== -1)
	{
		build_error(channel);
		return -1;
	}

	if (listen(channel->listenHandle_, d_backlog) == -1)
	{
		build_error(channel);
		return -1;
	}
	/*set non blocking*/
	if (set_blocking(channel, channel->listenHandle_, 0) == -1)
	{
		return -1;
	}
	return 0;
}

int wait_for_connect(struct comm_channel *channel)
{
	channel->handle_ = accept(channel->listenHandle_, 0, 0);

	if (channel->handle_ == -1)
	{
		build_error(channel);
		return -1;
	}
	return 0;
}

int wait_for_connect_timeout(struct comm_channel *channel, int secs)
{
	int res;
	struct pollfd pfd;
	if (secs <= 0) /*no timeout*/
	{
		return wait_for_connect(channel);
	}
	pfd.fd = channel->listenHandle_;
	pfd.events = POLLIN;
	res = poll(&pfd, 1, secs * 1000);
	if (res < 0)
	{
		build_error(channel);
        	return -1;
	} 
	else if (res == 0) /*timeout*/
	{
		clean_error(channel);/*TODO:return a different code for timed out accept*/
		return -1;			
	}	
	channel->handle_ = accept(channel->listenHandle_, 0, 0);
	if (channel->handle_ == -1)
	{
		build_error(channel);
		return -1;
	}
	return 0;
}

int connect_channel(struct comm_channel *channel)
{
	struct sockaddr_un s_un;
	int ret;
	if (channel->handle_ != -1)
	{
		close(channel->handle_);
	}
	channel->handle_ = socket(AF_UNIX, SOCK_STREAM, 0);
	if (channel->handle_ == -1)
	{
		build_error(channel);
		return -1;
	}
	memset(&s_un, 0, sizeof(s_un));
	s_un.sun_family = AF_UNIX;
	strncpy(s_un.sun_path, channel->name_, sizeof(s_un.sun_path)-1);
	ret = connect(channel->handle_, (const struct sockaddr*)&s_un, sizeof(s_un));
	if (ret == -1)
	{
		build_error(channel);
		return -1;
	}
	return 0;
}

int wait_for_open(struct comm_channel *channel, int timeout)
{
	struct pollfd pfd;
	int res;
	struct sockaddr_un s_un;
	if (timeout <= 0) /*no timeout*/
	{
		return connect_channel(channel);	
	}
	if (channel->handle_ != -1)
	{
		close(channel->handle_);
	}
	s_un.sun_family = AF_UNIX;
	strncpy(s_un.sun_path, channel->name_, sizeof(s_un.sun_path)-1);
	/*set non blocking*/
	if (set_blocking(channel, channel->handle_, 0) == -1)
	{
		return -1;
	}
	res = connect(channel->handle_, (const struct sockaddr*)&s_un,
				  sizeof(s_un));
	if (res < 0)
	{
		build_error(channel);
		if (channel->last_error_code_ == EINPROGRESS)
		{
			pfd.fd = channel->handle_;
			pfd.events = POLLOUT;
			res = poll(&pfd, 1, timeout * 1000);
			if (res < 0)
			{
				build_error(channel);
        			return -1;
			} 
			else if (res == 0) /*timeout*/
			{
				clean_error(channel);/*TODO:return a different code for timed out connect*/
				return -1;			
			}
		} 
		else 
		{
			return -1;
		}
	} 

	/*set blocking*/
	if (set_blocking(channel, channel->handle_, 1) == -1)
	{
		return -1;
	}
	return 0;
}

int close_channel(struct comm_channel *channel)
{
	int res = 0;
	if (channel->listenHandle_ != -1)
	{
		if (close(channel->listenHandle_) == -1)
		{
			res = res + 1;
		}
	}
	if (channel->handle_ != -1)
	{
		if (close(channel->handle_) == -1)
		{
			res = res + 1;
		}
	}
	if (res)
	{
		build_error(channel);
		return -1;
	}
	return 0;
}

int write_to_channel(struct comm_channel *channel, const char *data, int size)
{
	int sent = 0;
	int to_send = size;
	do 
	{
		int bytes;
		bytes = send(channel->handle_, (const char*)data + sent, size - sent, MSG_NOSIGNAL);
		if (bytes == -1)
		{
			build_error(channel);
			if (channel->last_error_code_ == EINTR)
			{
				continue;
			}
			return -1;
		}
		sent += bytes;
	} while(sent < to_send);
	return 0;
}

int read_from_channel(struct comm_channel *channel, char *data, int max_size)
{
	int read;
	int to_read = channel->in_buff_size_;
	if (max_size < channel->in_buff_size_)
	{
		clean_error(channel);
		return -1;
	}
	do
	{
		read = recv(channel->handle_,
		       (char*)data+channel->in_buff_size_ - to_read, to_read,
		       0);
		if (read == 0)
		{
			return -1; /*the peer has performed an orderly shutdown, quit*/
		}
		else if (read == -1)
		{
			build_error(channel);
			return -1;
		}
		to_read -= read;
	}while(to_read > 0);
	return 0;
}

int read_from_channel_size(struct comm_channel *channel, int size, char *data, int max_size)
{
	int read;
	int to_read = (size>max_size?max_size:size);
	if (size > channel->in_buff_size_ || size > max_size)
	{
		clean_error(channel);
		return -1;
	}
	do
	{
		read = recv(channel->handle_,
		       (char*)data + size - to_read, to_read, 0);
		if (read == 0)
		{
			return -1; /*the peer has performed an orderly shutdown, quit*/
		}
		else if (read == -1)
		{
			build_error(channel);
			return -1;
		}
		to_read -= read;
	}while(to_read > 0);
	return 0;
}


