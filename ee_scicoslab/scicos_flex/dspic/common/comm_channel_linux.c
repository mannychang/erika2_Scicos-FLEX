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

#include "comm_channel.h"
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>
#include <assert.h>

static HANDLE d_listen_socket;
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

void init_channel(struct comm_channel* channel)
{
	channel->handle_ = -1;
	channel->name_ = 0;
	channel->in_buff_size_ = 0;
	channel->out_buff_size_ = 0;
	channel->disconnection_needed_ = -1;
	channel->last_error_code_ = -1;
	channel->last_error_str_ = 0;
	d_listen_socket = -1;
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
	if (d_listen_socket != -1)
	{
		if (close(d_listen_socket) == -1)
		{
			build_error(channel);
		}
	}
}

int open_channel(struct comm_channel *channel)
{
	struct sockaddr_un s_un;
	remove(channel->name_);
	s_un.sun_family = AF_UNIX;
	strcpy(s_un.sun_path, channel->name_);
	if (d_listen_socket != -1)
	{
		close(d_listen_socket);
	}
	d_listen_socket = socket(AF_UNIX, SOCK_STREAM, 0);

	if (d_listen_socket == -1)
	{
		build_error(channel);
		return -1;
	}

	if (bind(d_listen_socket, (const struct sockaddr*)&s_un, sizeof(s_un))
		== -1)
	{
		build_error(channel);
		return -1;
	}

	if (listen(d_listen_socket, d_backlog) == -1)
	{
		build_error(channel);
		return -1;
	}
	return 0;
}

int wait_for_connect(struct comm_channel *channel)
{
	channel->handle_ = accept(d_listen_socket, 0, 0);

	if (channel->handle_ == -1)
	{
		build_error(channel);
		return -1;
	}
	return 0;
}

int wait_for_connect_timeout(struct comm_channel *channel, int secs)
{
	/*TODO*/
	return -1;
}

int connect_channel(struct comm_channel *channel)
{
	struct sockaddr_un s_un;
	s_un.sun_family = AF_UNIX;
	strcpy(s_un.sun_path, channel->name_);
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

	if (connect(channel->handle_, (const struct sockaddr*)&s_un,
				  sizeof(s_un)))
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
	pfd.fd = channel->handle_;
	pfd.events = POLLOUT;
	if (timeout > 0)
	{
		res = poll(&pfd, 1, timeout);
	}
	else
	{
		res = poll(&pfd, 1, 0);
	}
	if (res == -1) //ERROR
	{
		build_error(channel);
		return -1;
	}
	else if (res == 0)// TIMEOUT
	{
		clean_error(channel);
		return -1;
	}
	s_un.sun_family = AF_UNIX;
	strcpy(s_un.sun_path, channel->name_);
	if (connect(channel->handle_, (const struct sockaddr*)&s_un,
				  sizeof(s_un)))
	{
		build_error(channel);
		return -1;
	}
	return 0;
}

int close_channel(struct comm_channel *channel)
{
	int res = 0;
	if (d_listen_socket != -1)
	{
		if (close(d_listen_socket) == -1)
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
	int written;
	written = send(channel->handle_, data, size, MSG_NOSIGNAL);
	if (written == -1 || written != size)
	{
		build_error(channel);
		return -1;
	}
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
					  (char*)data+channel->in_buff_size_ - to_read, to_read, 0);
		if (read == -1 || read == 0)
		{
			break;
		}
		to_read -= read;
	}while(to_read > 0);
	if (read == -1)
	{
		build_error(channel);
		return -1;
	}
	return 0;
}

int read_from_channel_size(struct comm_channel *channel, int size, char *data, int max_size)
{
	int read;
	int to_read = size;
	if (size > channel->in_buff_size_ || size > max_size)
	{
		clean_error(channel);
		return -1;
	}
	do
	{
		read = recv(channel->handle_,
					  (char*)data + size - to_read, to_read, 0);
		if (read == -1 || read == 0)
		{
			break;
		}
		to_read -= read;
	}while(to_read > 0);
	if (read == -1)
	{
		build_error(channel);
		return -1;
	}
	return 0;
}


