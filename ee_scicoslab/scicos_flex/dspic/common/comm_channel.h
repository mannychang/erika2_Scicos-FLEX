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
#ifndef COMM_CHANNEL_H
#define COMM_CHANNEL_H

#if defined(WIN32)
	#include <windows.h>
#else
#if defined(HANDLE)
	#undef HANDLE
#endif
	#define HANDLE int
#endif

typedef struct comm_channel
{
	HANDLE handle_;
	char* name_;
	int in_buff_size_;
	int out_buff_size_;
	int disconnection_needed_;
	int last_error_code_;
	char* last_error_str_;
}comm_channel;

void init_channel(struct comm_channel* channel);

void build_channel(struct comm_channel* channel, int in_size, int out_size, const char* name);

void clean_channel(struct comm_channel* channel);

int open_channel(struct comm_channel* channel);

int wait_for_connect(struct comm_channel* channel);

int wait_for_connect_timeout(struct comm_channel* channel, int secs);

int connect_channel(struct comm_channel* channel);

int wait_for_open(struct comm_channel* channel, int timeout);

int close_channel(struct comm_channel* channel);

int write_to_channel(struct comm_channel* channel, const char* data, int size);

int read_from_channel(struct comm_channel* channel, char* data, int max_size);

int read_from_channel_size(struct comm_channel* channel, int size, char* data, int max_size);

#endif

