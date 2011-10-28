#include "stdafx.h"
#include "dll_udp.h"
#include "udp_receiver.h"


/*** udp_receiver ***/
UDP_LIB_API void udp_receiver(scicos_block *block,int flag){
    int i;
    int err;
    float tmp[MAX_CHANNELS];
	static int udp_receiver_is_initialized = 0;
	static int udp_receiver_lines = 0;

    switch (flag){
         case SCICOS_BLOCK_INIT:	
			if(!udp_receiver_is_initialized) {
				udp_receiver_is_initialized = 1;
				udp_receiver_lines = RX_LINES;
				for (i=0;i<udp_receiver_lines;i++)
					y(i,0)=0.0;
			}
            break;

		case SCICOS_BLOCK_OUTPUT_UPDATE:
			for (i=0; i<MAX_CHANNELS; i++)
				tmp[i] = 0.0;
			err = recvfrom(sock_str_ptr_rs->sd,(char *)&tmp[0],sizeof(tmp),0, 0, 0);
			if(err == SOCKET_ERROR) {
					//fprintf(fuscrs,"Socket error while trying to receive.\n");
					//fprintf(fuscrs,"#error: %d\n",WSAGetLastError());
				}
			if (err < 0)
   				break;
			else {
				fprintf(fuscrs,"recvfrom: %d\n",err);
				for (i=0; i < udp_receiver_lines; i++) {
					/* y(i,0): this access should be used to read the first element in the i-th line */
                    /* y(0,i): this access should be used to read the i-th element in the line */
					y(i,0) = (double)tmp[i];
					fprintf(fuscrs,"y(%d)= %f\n",i,y(i,0));
				}
			}
			break;

        case SCICOS_BLOCK_END:		
			if(udp_receiver_is_initialized) {
				udp_receiver_is_initialized = 0;
				udp_receiver_lines = 0;
			}
			
            break;
    }

}
