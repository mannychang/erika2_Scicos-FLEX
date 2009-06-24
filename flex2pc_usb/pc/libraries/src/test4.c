#include <stdlib.h>
#include <stdio.h>
#include "flex_usb.h"

#define MAX_MESSAGE_SIZE 640

static char to_stop = 0;
unsigned int j = 0;

int main(int argc, char **argv)
{
	int retv = 0, i;
	short int data[2];

	printf("\n");
	printf("*************************************************\n");
	printf("            Test flex_pc_usb library             \n");
	printf("*************************************************\n");
	
	printf("\n - flex_usb_init() : ");
	retv = flex_usb_init(640, 640, 0); /* 0 = default options */
	if (retv < 0) {
		printf("ERROR, returned value is %d.\n", retv);
		return 0;
	} else {
		printf("OK.\n");
	}
	printf("\nStart Test4 ...\n");
	for (;;) {
		data[0] = 0;
		data[1] = 0;
		printf("\nReceiving:");
		retv = flex_usb_read((uint8_t*) data, 4,
				     FLEX_USB_READ_BLOCK);
		if (retv < 0) {
			printf(" ERROR, read retv %d.", retv);
			fflush(stdout);
			break;
		} else {
			printf(" DATA-> X=%d Y=%d", data[0], data[1]);
		}
		fflush(stdout);
	}
	printf("\n");
	flex_usb_close();
	
	return 0;
}
