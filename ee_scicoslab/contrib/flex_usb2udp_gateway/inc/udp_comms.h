#ifndef _UDPHOST_H
#define _UDPHOST_H

#define UDP_IN 1
#define UDP_OUT 0
#define MAX_IP_LENGTH 16 /* xxx.xxx.xxx.xxx\0  */

extern int errno;

struct sockaddr_in;

/* data to be exchenged */
struct _UDPData {
	void* buffer;
	int size;
};
typedef struct _UDPData UDPData;

/* Sockect data */
struct _UDPStruct {
	int sock;
	char *ip_addr;
	unsigned int port;
	struct sockaddr_in* addr;
	int initialized;
};
typedef struct _UDPStruct UDPStruct;

/*
	Allocate sockect data
	Return a pointer to the struct allocated
*/
UDPStruct* create_udp_struct();

/*
	Construct a socket
	giving a pointer to UDPStruct allocated with create_udp_struct(),
	the pair IP,Port and the direction of the socket
	 
*/
int init_udp_struct(UDPStruct* udp_str, const char* ip,unsigned int port,unsigned char dir);

/*
	Free memory and close the socket
*/
void close_udp(UDPStruct* udp_str);

/*
	Send udp data
*/
//int send_udp_data(UDPStruct *udp_str,UDPData *udp_data); 
int send_udp_data(UDPStruct *udp_str, UDPData *udp_data, struct sockaddr_in* dest_addr);

/*
	Receive udp data
*/
int receive_udp_data(UDPStruct *udp_str,UDPData *udp_data); 

#endif

