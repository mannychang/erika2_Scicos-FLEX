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

struct comm_channel 
{
	HANDLE handle_;
	char* name_;
	int in_buff_size_;
	int out_buff_size_;
	int disconnection_needed_;
	int last_error_code_;
	char* last_error_str_;
};

void init_channel(struct comm_channel* channel);

void build_channel(struct comm_channel* channel, int in_size, int out_size, const char* name);

void clean_channel(struct comm_channel* channel);

int open_channel(struct comm_channel* channel);

int wait_for_connect(struct comm_channel* channel);

int connect_channel(struct comm_channel* channel);

int wait_for_open(struct comm_channel* channel, int timeout);

int close_channel(struct comm_channel* channel);

int write_to_channel(struct comm_channel* channel, const char* data, int size);

int read_from_channel(struct comm_channel* channel, char* data, int max_size);

#endif

