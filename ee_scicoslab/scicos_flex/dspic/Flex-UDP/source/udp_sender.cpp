#include "stdafx.h"
#include "dll_udp.h"
#include "udp_sender.h"


/*** udp_sender ***/
UDP_LIB_API void udp_sender(scicos_block *block,int flag){
    int i;
    int err;
    float tmp[MAX_CHANNELS];
	static int udp_sender_is_initialized = 0;
	static int udp_sender_lines = 0;
	
    switch (flag){
         case SCICOS_BLOCK_INIT:
			if(!udp_sender_is_initialized) {
				udp_sender_is_initialized = 1;
				udp_sender_lines = TX_LINES;
			}
            break;


        case SCICOS_BLOCK_OUTPUT_UPDATE:
			for (i=0; i<MAX_CHANNELS; i++)
				tmp[i] = 0.0; /* This initialization avoids random values in the vector */
			for (i=0; i<udp_sender_lines; i++)
				tmp[i] = (float)(u(i,0));
			for (i=0; i<udp_sender_lines; i++)
				fprintf(fuscrs,"tmp[%d]:%f\n", i, tmp[i]);
            err = sendto(sock_str_ptr_rs->sd,(char *)&tmp[0],sizeof(tmp),0,(sockaddr *)&(dest_addr), sizeof(sockaddr_in));
			if (err < 0) {
				fprintf(fuscrs,"Socket error while trying to transmit.\n");
				fprintf(fuscrs,"#error: %d\n",WSAGetLastError());
			}
			break;

        case SCICOS_BLOCK_END:
			if(udp_sender_is_initialized) {
				udp_sender_is_initialized = 0;
				udp_sender_lines = 0;
			}
			
            break;
    }

}

