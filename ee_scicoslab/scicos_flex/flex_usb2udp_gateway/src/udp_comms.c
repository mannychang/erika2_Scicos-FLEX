#include "udp_comms.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

extern int errno;

UDPStruct* create_udp_struct()
{
	UDPStruct* udp_str = 
		(UDPStruct *) malloc(sizeof(UDPStruct));
	udp_str->ip_addr = 
		(char *)malloc(MAX_IP_LENGTH * sizeof(char));
	udp_str->addr = 
		(struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
	udp_str->initialized = 1;
	return udp_str;
}

void close_udp(UDPStruct* udp_str)
{
	if (udp_str != 0)
		if (udp_str->initialized == 1){
			close(udp_str->sock);
			free(udp_str->addr);
			free(udp_str->ip_addr);
			free(udp_str);
			udp_str = 0;
		}
	

}

int init_udp_struct(
	UDPStruct* udp_str, 
	const char* ip,
	unsigned int port, 
	unsigned char dir)
{
	if (udp_str == 0){
		fprintf(stderr,"ERROR: UDPStruct not allocated\n");
		return -1;
	}
	if (udp_str->initialized == 0){
		fprintf(stderr,"ERROR: UDPStruct not initialized\n");
		return -1;
	}
	if (ip == 0){
		fprintf(stderr,"ERROR: bad IP\n");
		return -1;
	} 
	strcpy(udp_str->ip_addr,ip);
	udp_str->port = port;
	udp_str->addr->sin_family = AF_INET;
	udp_str->addr->sin_addr.s_addr = inet_addr(ip);
	udp_str->addr->sin_port = htons(port);
	if (dir == UDP_IN){
		udp_str->sock = socket(AF_INET, SOCK_DGRAM, 0);
		if (udp_str->sock < 0){
			fprintf(stderr,"ERROR: socket() error %d\n",errno);
			return -1;
		}
		if (bind(
			udp_str->sock,
			(struct sockaddr *)udp_str->addr,
			sizeof(struct sockaddr_in))<0){
			fprintf(stderr,"ERROR: UDP_IN bind() error %d\n",errno);
			return -1;
		}
		fcntl(udp_str->sock, F_SETFL, O_NONBLOCK); /*set non blocking*/
	}
	else if (dir == UDP_OUT){
		udp_str->sock = socket(AF_INET, SOCK_DGRAM, 0);
		if (udp_str->sock < 0){
			fprintf(stderr,"ERROR: socket() error %d\n",errno);
			return -1;
		}
		if (bind(
					udp_str->sock,
					(struct sockaddr *)udp_str->addr,
					sizeof(struct sockaddr_in))<0){
			fprintf(stderr,"ERROR: bind() error %d\n",errno);
			return -1;
		}
	}
	else{
		fprintf(stderr,"ERROR: bad direction\n");
		return -1;
	}	
	return 1;
}

/*
int send_udp_data(UDPStruct *udp_str,UDPData *udp_data)
{	
	if (udp_str->initialized != 1){
		fprintf(stderr,"ERROR: UDPStruct not initialized\n");
		return -1;
	}

	return sendto(
		udp_str->sock,
		(char *)udp_data->buffer, 
		udp_data->size, 
		0, 
		(const struct sockaddr *)udp_str->addr, 
		sizeof(struct sockaddr_in));
} 
*/
int send_udp_data(UDPStruct *udp_str, UDPData *udp_data, struct sockaddr_in* dest_addr)
{	
	if (udp_str->initialized != 1){
		fprintf(stderr,"ERROR: UDPStruct not initialized\n");
		return -1;
	}

	return sendto( udp_str->sock, (char *)udp_data->buffer, udp_data->size, 0, (const struct sockaddr *)dest_addr, sizeof(struct sockaddr_in));
} 

int receive_udp_data(
	UDPStruct *udp_str,
	UDPData *udp_data){
	
	if (udp_str->initialized != 1){
		fprintf(stderr,"ERROR: UDPStruct not initialized\n");
		return -1;
	}
	
	return recvfrom(
		udp_str->sock,
		(char *)udp_data->buffer,
		udp_data->size,
		0,0,0);
}

 
