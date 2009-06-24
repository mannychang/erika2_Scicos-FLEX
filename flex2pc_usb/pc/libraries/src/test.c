#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "flex_usb.h"

#define MAX_MESSAGE_SIZE 640

static char to_stop = 0;
unsigned int j = 0;

int main(int argc, char **argv)
{
	int retv = 0, i;
	unsigned int s;
	unsigned char received[MAX_MESSAGE_SIZE];
	unsigned char sent[MAX_MESSAGE_SIZE];

	printf("\n");
	printf("*************************************************\n");
	printf("            Test flex_pc_usb library             \n");
	printf("*************************************************\n");
	
	printf("\n - Compiler types : \n");
	s = sizeof(uint8_t);
	printf("   sizeof(uint8_t) = %u : ", s);
	if (s == 1) 
		printf("OK.\n");
	else
		printf("ERROR.\n");
	s = sizeof(uint16_t);
	printf("   sizeof(uint16_t) = %u : ", s);
	if (s == 2) 
		printf("OK.\n");
	else
		printf("ERROR.\n");
	s = sizeof(uint32_t);
	printf("   sizeof(uint32_t) = %u : ", s);
	if (s == 4) 
		printf("OK.\n");
	else
		printf("ERROR.\n");

	printf("\n - flex_usb_init() : ");
	retv = flex_usb_init(640, 640, 0); /* 0 = default options */
	if (retv < 0) {
		printf("ERROR, returned value is %d.\n", retv);
		return 0;
	} else {
		printf("OK.\n");
	}

	printf("\nStart Echo Test...\n");
	for (;;) {
		memset(sent, '\0', MAX_MESSAGE_SIZE);
		memset(received, '\0', MAX_MESSAGE_SIZE);
		printf("\nWrite message to send (type 'exit' to finish): ");
		retv = scanf("%s", sent);
		if (retv <= 0) {
			printf("Sending Nothing!");
		} else {
			if (strcmp(sent, "exit") == 0) {
				printf("\n\nEcho Test completed!\n");
				break;
			}
			printf("Sending: %s", sent);
			retv = flex_usb_write(sent, strlen(sent));
			if (retv == -116) {
				printf(" ERROR, TIMEOUT!");
				fflush(stdout);
				continue;
			} else if (retv < 0) {
				printf(" ERROR, ret value is %d.", retv);
				fflush(stdout);
				break;
			} else {
				printf(" OK!");
			}
		}
		fflush(stdout);
		printf("\nReceiving:");
		retv = flex_usb_read(received, strlen(sent), 
				     FLEX_USB_READ_BLOCK);
		if (retv < 0) {
			printf(" ERROR, read retv %d.", retv);
			fflush(stdout);
			break;
		} else {
			printf(" %s", received);
		}
		printf("\n");
		fflush(stdout);
	}
	printf("\n");
	flex_usb_close();
	
	return 0;
}
