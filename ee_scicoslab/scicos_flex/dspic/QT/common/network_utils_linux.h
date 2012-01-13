#ifndef NETWORK_UTILS_LINUX_H
#define NETWORK_UTILS_LINUX_H

#include <sys/socket.h>
#include <stdint.h>

#ifdef TRUE
	#undef TRUE
#endif

#define TRUE 1

#ifdef FALSE
	#undef FALSE
#endif

#define FALSE 0

#ifdef BAD
	#undef BAD
#endif

#define BAD -1

#ifdef OK
	#undef OK
#endif

#define OK 0

typedef struct _udp_struct {
	/* Socket Descriptor*/
	int sock;
	/* BIND address */
	struct sockaddr_in* addr;
	/* PEER address [Optional] */
	struct sockaddr_in* peer_addr;
	int8_t valid;
	int8_t initialized;
	int8_t peer_set;
	int8_t non_blocking;
}udp_struct;

typedef struct _udp_data {
	void* buffer;
	int size;
}udp_data;

/*
	Allocate sockect data
	Return a pointer to the struct allocated
*/
udp_struct* create_udp();

/*
	Construct a socket
	giving a pointer to udp_struct allocated with create_udp(),
	the pair IP,Port and the direction of the socket
	 
*/
int init_udp(udp_struct* udp, const char* ip, uint16_t port);

/*
	Set IP,Port and the peer IP/Port where the datagram are sent with
	send_udp_data
	 
*/
int set_udp_peer(udp_struct* udp, const char* ip, uint16_t port);

/*
	Set the non blocking IO for the udp socket.
	Default is Blocking.
*/
int set_non_blocking(udp_struct* udp);

/*
	Free memory and close the socket
	It also set the upd pointer to the udp_struct to zero
	in order to recognize already free structure
*/
int close_udp(udp_struct** udp);

/*
	Send udp data to the peer specified by peer_addr
*/
int send_to_udp_data(udp_struct *udp, udp_data* data, struct sockaddr* peer_addr); 

/*
	Send udp data to the peer set with set_udp_peer
*/
int send_udp_data(udp_struct *udp, udp_data* data);

/*
	Receive udp data
*/
int receive_udp_data(udp_struct *udp, udp_data *data);

/*
	Wait for ready signal from engine
*/
int wait_for_engine_ready(udp_struct* udp);

/*
	Parse IPv4 Address
*/
int is_valid_ip_v4(const char* ip);

/*
	Swap size bytes of src and store in dst
*/
void swap_bytes(void* dst, void* src, int size);

#endif
