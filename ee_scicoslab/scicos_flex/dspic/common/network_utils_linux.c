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
 
#include "network_utils_linux.h"
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_IP_LENGTH 16 /* xxx.xxx.xxx.xxx\0  */

#define VALID(udp) \
        if (udp == 0) \
        { \
                fprintf(stderr,"ERROR: (%s)  udp_struct not valid\n",__func__); \
                return BAD; \
        } \
        if (udp->valid == FALSE) \
        { \
                fprintf(stderr,"ERROR: (%s) udp_struct not valid\n",__func__); \
                return BAD; \
        }


#define INITIALIZED(udp) \
        if (udp->initialized == FALSE) \
        { \
                fprintf(stderr,"ERROR: (%s) udp_struct not initialized\n",__func__); \
                return BAD; \
        }


#define DBG(str) \
	fprintf(stdout,"[DBG] (%s) %s",__func__,str);

#define WARN(str) \
	fprintf(stdout,"[WARN] (%s) %s",__func__,str);

#define ERR(str) \
	fprintf(stderr,"[ERR] (%s) %s",__func__,str);

udp_struct* create_udp()
{
	udp_struct* udp = (udp_struct *) malloc(sizeof(udp_struct));

	udp->addr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));

	udp->peer_addr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));

	udp->initialized = FALSE;

	udp->peer_set = FALSE;

	udp->non_blocking = FALSE;
	
	udp->valid = TRUE;

	return udp;
}

int init_udp(udp_struct* udp, const char* ip, uint16_t port)
{
	VALID(udp)

        if (is_valid_ip_v4(ip) == FALSE)
	{
                fprintf(stderr,"ERROR: (init_udp) bad IPv4 Address %s\n",ip);
                return BAD;
        }

	udp->addr->sin_family = AF_INET;
	udp->addr->sin_addr.s_addr = inet_addr(ip);
	udp->addr->sin_port = htons(port);

	if ((udp->sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		fprintf(stderr,"ERROR: (init_udp) error %d (%s)\n",errno,strerror(errno));
		return BAD;
	}

	if (bind(udp->sock, (struct sockaddr *)udp->addr,
		sizeof(struct sockaddr_in))<0)
	{
		fprintf(stderr,"ERROR: (init_udp) error %d (%s)\n",errno,strerror(errno));
		return BAD;
	}
	udp->initialized = TRUE;
	return OK;
}

int set_udp_peer(udp_struct* udp, const char* ip, uint16_t port)
{
	VALID(udp)

	if (is_valid_ip_v4(ip) == FALSE)
	{
		fprintf(stderr,"ERROR: (set_udp_peer) bad IPv4 Address %s\n",ip);
		return BAD;
	}
        udp->peer_addr->sin_family = AF_INET;
        udp->peer_addr->sin_addr.s_addr = inet_addr(ip);
        udp->peer_addr->sin_port = htons(port);
	udp->peer_set = TRUE;
	return OK;
}

int set_non_blocking(udp_struct* udp)
{
	VALID(udp)

	INITIALIZED(udp)
	
	if (fcntl(udp->sock, F_SETFL, O_NONBLOCK) < 0)
	{
		fprintf(stderr,"ERROR: (set_non_blocking) error %d (%s)\n",errno,strerror(errno));	
		return BAD;
	}
	udp->non_blocking = TRUE;
	return OK;
}

int close_udp(udp_struct** udp)
{	
	int res=OK;
	VALID((*udp))
	if ((*udp)->initialized == TRUE)
	{
		if (close((*udp)->sock) < 0)
		{
			fprintf(stderr,"ERROR: (close_udp) error %d (%s)\n",errno,strerror(errno));
			res = BAD;
		}
	}
	free((*udp)->addr);
	free((*udp)->peer_addr);
	free((*udp));
	*udp = 0;
	return res;
}

int send_to_udp_data(udp_struct *udp, udp_data* data, struct sockaddr* peer_addr)
{
	int sent;

	VALID(udp)

	INITIALIZED(udp)

	if ((sent = sendto(udp->sock,(char *)data->buffer, data->size, 0,
                (const struct sockaddr *)peer_addr,
                sizeof(struct sockaddr_in))) < 0)
        {
                fprintf(stderr,"ERROR: (send_udp_data) error %d (%s)\n",errno,strerror(errno));
                return BAD;
        }
        else
        {
                return sent;
        }
}

int send_udp_data(udp_struct *udp, udp_data* data)
{
	int sent;

	VALID(udp)

	INITIALIZED(udp)

	if (udp->peer_set == FALSE)
	{
		fprintf(stderr,"ERROR: (send_udp_data) peer not set. \n");
		return BAD;

	}
	if ((sent = sendto(udp->sock,(char *)data->buffer, data->size, 0,       
                (const struct sockaddr *)udp->peer_addr,    
                sizeof(struct sockaddr_in))) < 0)
        {
                fprintf(stderr,"ERROR: (send_udp_data) error %d (%s)\n",errno,strerror(errno));
                return BAD;
        } 
        else
        {
                return sent;
        }
}

int receive_udp_data(udp_struct *udp, udp_data *data)
{
	int recvd;

	VALID(udp)

	INITIALIZED(udp)

	if ((recvd = recvfrom(udp->sock,(char *)data->buffer,data->size,0,0,0)) < 0 )
	{	
		if (udp->non_blocking == FALSE || (udp->non_blocking == TRUE && errno != EWOULDBLOCK))
	                fprintf(stderr,"ERROR: (receive_udp_data) error %d (%s)\n",errno,strerror(errno));
		return BAD;
	}
	else
	{
		return recvd;
	}
}

int wait_for_engine_ready(udp_struct *udp)
{
	int recvd;

	char data;

	VALID(udp)

	INITIALIZED(udp)

	if ((recvd = recvfrom(udp->sock, &data, 1,0,0,0)) < 0 )
	{
		if (udp->non_blocking == FALSE || (udp->non_blocking == TRUE && errno != EWOULDBLOCK))
					fprintf(stderr,"ERROR: (receive_udp_data) error %d (%s)\n",errno,strerror(errno));
		return BAD;
	}
	else
	{
		return recvd;
	}
}

int is_valid_ip_v4(const char* ip)
{
	unsigned int n1,n2,n3,n4;

	if(sscanf(ip,"%u.%u.%u.%u", &n1, &n2, &n3, &n4) != 4) 
		return FALSE;

	if((n1 <= 255) && (n1 <= 255) && (n2 <= 255) && (n3 <= 255) && (n4 <= 255)) {
		char buf[64];
		sprintf(buf,"%u.%u.%u.%u",n1,n2,n3,n4);
		if(strcmp(buf,ip) != 0) 
			return FALSE;
		return TRUE;
	}
	return FALSE;
}

void swap_bytes(void* dst, void* src, int size)
{
	int i;
	char* c_src = (char*)src;
	char* c_dst = (char*)dst;
	if (size <= 0)
	{
		ERR("Invalid size")
		return;
	}
	for(i=0; i < size; i++)
		memcpy(c_dst+i,c_src+size-1-i,1);
}
