#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "flex_usb.h"
#include "flex_bus_frame.h"

#define MAX_MESSAGE_SIZE 640

static char to_stop = 0;
unsigned int j = 0;

int main(int argc, char **argv)
{
	int retv = 0, i;
	unsigned int node;
	unsigned int cnt[4] = {0, 0, 0, 0};
	struct flex_bus_packet_t rx_packet;

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
	printf("\nStart Test2 ...\n");
	for (;;) {
		memset((uint8_t*) &rx_packet, 0, 
		       sizeof(struct flex_bus_packet_t));
		printf("\nReceiving:");
		retv = flex_usb_read((uint8_t*) &rx_packet, 
				     sizeof(struct flex_bus_packet_t),
				     FLEX_USB_READ_BLOCK);
		if (retv < 0) {
			printf(" ERROR, read retv %d.", retv);
			fflush(stdout);
			break;
		} else {
			node = rx_packet.channel & 0x00FF;
			printf(" NODE=%d  CHANNEL=%d   SIZE=%d   DATA-> int=%d", 
			       node, (rx_packet.channel >> 8) & 0x00FF,
			       rx_packet.payload.length,
			       *((uint32_t *)rx_packet.payload.data));
			if (*((uint32_t *)rx_packet.payload.data) % 100 == 0) {
				cnt[node]++;  
				memset((uint8_t*) &rx_packet, 0, 
				       sizeof(struct flex_bus_packet_t));
				rx_packet.channel = node;
			        *((uint32_t *)rx_packet.payload.data) = 
								cnt[node];
				printf("  Send(%d)", cnt[node]);
				retv = flex_usb_write((uint8_t*) &rx_packet, 
				      sizeof(struct flex_bus_packet_t));
				if (retv == -116) {
					printf(" ERROR, TIMEOUT!");
					fflush(stdout);
					continue;
				} else if (retv < 0) {
					printf(" ERROR, retv is %d.", retv);
					fflush(stdout);
					break;
				} else {
					printf(" OK!");
				}	
			}
		}
		fflush(stdout);
	}
	printf("\n");
	flex_usb_close();
	
	return 0;
}
