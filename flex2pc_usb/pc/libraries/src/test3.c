#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include "flex_usb.h"
#include "flex_usbscicos.h"

#define MAX_MESSAGE_SIZE 640

static char to_stop = 0;
unsigned int j = 0;

void close_handler(int signo)
{
	printf("\n\nClosing USBSCICOS connection ... ");
	flex_usbscicos_close();
	printf("Done!\n\n");
	exit(0);
}

int main(int argc, char **argv)
{
	struct sigaction n,v;
	int retv = 0, i, loopy;
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
		/* 9 USB RX blocks */
		retv = flex_usbscicos_init(1); 
		retv = flex_usbscicos_init(1); 
		retv = flex_usbscicos_init(1); 
		retv = flex_usbscicos_init(1); 
		retv = flex_usbscicos_init(1); 
		retv = flex_usbscicos_init(1); 
		retv = flex_usbscicos_init(1); 
		retv = flex_usbscicos_init(1); 
		retv = flex_usbscicos_init(1); 
		/* 3 USB TX blocks */
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
			printf("\nReceiving:\n");
			data0 = flex_usbscicos_read(0);
			data1 = flex_usbscicos_read(1);
			data2 = flex_usbscicos_read(2);
			data3 = flex_usbscicos_read(3);
			data4 = flex_usbscicos_read(4);
			data5 = flex_usbscicos_read(5);
			data6 = flex_usbscicos_read(6);
			data7 = flex_usbscicos_read(7);
			data8 = flex_usbscicos_read(8);
			data9 = flex_usbscicos_read(9);
			if (retv < 0) {
				printf(" ERROR, read retv %d.", retv);
				fflush(stdout);
				break;
			} else {
				printf("DATA_ID=1 DATA=%f\n", data0);
				printf("DATA_ID=2 DATA=%f\n", data1);
				printf("DATA_ID=3 DATA=%f\n", data2);
				printf("DATA_ID=4 DATA=%f\n", data3);
				printf("DATA_ID=5 DATA=%f\n", data4);
				printf("DATA_ID=6 DATA=%f\n", data5);
				printf("DATA_ID=6 DATA=%f\n", data6);
				printf("DATA_ID=6 DATA=%f\n", data7);
				printf("DATA_ID=6 DATA=%f\n", data8);
				printf("DATA_ID=6 DATA=%f\n", data9);
			}
		///*
			flex_usbscicos_write(0, 1.1);
			flex_usbscicos_write(1, 1.8);
			flex_usbscicos_write(2, 1.2);
			printf("SEND\n");
		//*/
			fflush(stdout);
		}
		printf("\n - flex_usbscicos_close() : ");
		flex_usbscicos_close();
		sleep(1);
	}
	return 0;
}
