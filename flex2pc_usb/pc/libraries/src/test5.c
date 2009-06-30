#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include "flex_usb.h"
#include "udp_comms.h"
#include "flex_usbscicos.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>



extern int errno;



#define MAX_MESSAGE_SIZE 640
#define UDP_PORT 50000
#define MAX_CHANNELS 15

UDPStruct *udp_sender; 
UDPStruct *udp_receiver;
UDPData *data_sender;
UDPData *data_receiver;

static char to_stop = 0;
unsigned int j = 0;

void close_udp_data(UDPData *data)
{
	if (data != 0){
		free(data->buffer);
		free(data);
	}
}


void close_handler(int signo)
{
	printf("\n\nClosing USBSCICOS connection ... ");
	flex_usbscicos_close();
	close_udp(udp_sender);	
	close_udp(udp_receiver);	
	close_udp_data(data_sender);	
	close_udp_data(data_receiver);	
	printf("Done!\n\n");
	exit(0);
}



int main(int argc, char **argv)
{
	struct sigaction n,v;
	int retv = 0, i,j, loopy,sent,recvd;
	unsigned int node;
	unsigned int cnt[4] = {0, 0, 0, 0};
	float data0;
	float data1;
	float data2;
	float data3;
	float data4;
	float data5;
	float data6;
	float data7;
	float data8;
	float data9;
	float data_out = 33;

	udp_sender = create_udp_struct();
	udp_receiver = create_udp_struct();
	init_udp_struct(udp_sender, "127.0.0.1", UDP_PORT, UDP_OUT);
	init_udp_struct(udp_receiver, "127.0.0.1", UDP_PORT+1, UDP_IN);
	data_sender = (UDPData *)malloc (sizeof(UDPData));
	data_receiver = (UDPData *)malloc (sizeof(UDPData));
	data_sender->buffer = (float *) malloc(MAX_CHANNELS*sizeof(float));
	data_sender->size = MAX_CHANNELS * sizeof(float);
	data_receiver->buffer = (float *) malloc(MAX_CHANNELS*sizeof(float));
	data_receiver->size = MAX_CHANNELS * sizeof(float);
	for (i=0;i<MAX_CHANNELS; i++){
		((float *)data_sender->buffer)[i] = 0;
		((float *)data_receiver->buffer)[i] = 0;
	}	

	n.sa_handler = close_handler;
	n.sa_flags=0;
	sigemptyset(&(n.sa_mask));
	if(sigaction(SIGINT, &n,&v)==-1){
		printf("ERROR, Install SIGINT handler \n");
		exit(0);
	}
	printf("\n");
	printf("*************************************************\n");
	printf("          Test flex_usbscicos library            \n");
	printf("*************************************************\n");
	for (;;) {
		printf("\n - flex_usbscicos_init() : ");
		/* 15 USB RX blocks */
		retv = flex_usbscicos_init(1); 
		retv = flex_usbscicos_init(1); 
		retv = flex_usbscicos_init(1); 
		retv = flex_usbscicos_init(1); 
		retv = flex_usbscicos_init(1); 
		retv = flex_usbscicos_init(1); 
		retv = flex_usbscicos_init(1); 
		retv = flex_usbscicos_init(1); 
		retv = flex_usbscicos_init(1); 
		retv = flex_usbscicos_init(1); 
		retv = flex_usbscicos_init(1); 
		retv = flex_usbscicos_init(1); 
		retv = flex_usbscicos_init(1); 
		retv = flex_usbscicos_init(1); 
		retv = flex_usbscicos_init(1); 
		/* 15 USB TX blocks */
		retv = flex_usbscicos_init(0); 
		retv = flex_usbscicos_init(0); 
		retv = flex_usbscicos_init(0); 
		retv = flex_usbscicos_init(0); 
		retv = flex_usbscicos_init(0); 
		retv = flex_usbscicos_init(0); 
		retv = flex_usbscicos_init(0); 
		retv = flex_usbscicos_init(0); 
		retv = flex_usbscicos_init(0); 
		retv = flex_usbscicos_init(0); 
		retv = flex_usbscicos_init(0); 
		retv = flex_usbscicos_init(0); 
		retv = flex_usbscicos_init(0); 
		retv = flex_usbscicos_init(0); 
		retv = flex_usbscicos_init(0); 
		if (retv < 0) {
			printf("ERROR, returned value is %d.\n", retv);
			return 0;
		} else {
			printf("OK.\n");
		}
		printf("\nStart Test3 ...\n");
		//for (loopy = 0; loopy < 1000; loopy++) {
		for (;;) {
			//printf("\nReceiving:\n");
			//data0 = flex_usbscicos_read(0);
			//data1 = flex_usbscicos_read(1);
			//data2 = flex_usbscicos_read(2);
			//data3 = flex_usbscicos_read(3);
			//data4 = flex_usbscicos_read(4);
			//data5 = flex_usbscicos_read(5);
			//data6 = flex_usbscicos_read(6);
			//data7 = flex_usbscicos_read(7);
			//data8 = flex_usbscicos_read(8);
			//data9 = flex_usbscicos_read(9);

			for (i=0;i<MAX_CHANNELS; i++){
				((float *)data_sender->buffer)[i] = 
					flex_usbscicos_read(i);			
			}	

			if (retv < 0) {
				printf(" ERROR, read retv %d.", retv);
				fflush(stdout);
				break;
			} else {
				sent = send_udp_data(udp_sender,data_sender);
				if (sent > 0)
					printf(".");
				//printf("DATA_ID=1 DATA=%f\n", data0);
				//printf("DATA_ID=2 DATA=%f\n", data1);
				//printf("DATA_ID=3 DATA=%f\n", data2);
				//printf("DATA_ID=4 DATA=%f\n", data3);
				//printf("DATA_ID=5 DATA=%f\n", data4);
				//printf("DATA_ID=6 DATA=%f\n", data5);
				//printf("DATA_ID=6 DATA=%f\n", data6);
				//printf("DATA_ID=6 DATA=%f\n", data7);
				//printf("DATA_ID=6 DATA=%f\n", data8);
				//printf("DATA_ID=6 DATA=%f\n", data9);
			}
		///*

			recvd = receive_udp_data(udp_receiver, data_receiver);
			if (recvd > 0)
				printf("*");
			for (i=0;i<MAX_CHANNELS; i++){
				flex_usbscicos_write(
					i,
					((float *)data_receiver->buffer)[i]);			
			}	

			//flex_usbscicos_write(0, 1.1);
			//flex_usbscicos_write(1, 1.8);
			//flex_usbscicos_write(2, 1.2);
			//flex_usbscicos_write(3, 1.1);
			//flex_usbscicos_write(4, 1.1);
			//flex_usbscicos_write(5, 1.1);
			//flex_usbscicos_write(6, 1.1);
			//flex_usbscicos_write(7, 1.1);
			//flex_usbscicos_write(8, 1.8);
			//flex_usbscicos_write(9, 1.2);
			//flex_usbscicos_write(10, 1.8);
			//flex_usbscicos_write(11, 1.2);
			//flex_usbscicos_write(12, 1.8);
			//flex_usbscicos_write(13, 1.2);
			//flex_usbscicos_write(14, 1.8);
			//printf("SEND\n");
		//*/
			fflush(stdout);
		}
		printf("\n - flex_usbscicos_close() : ");
		flex_usbscicos_close();
		sleep(1);
	}
	return 0;
}

