#include "stdafx.h"
#include "dll_rs232.h"
#include "rs232_receiver.h"


/*** rs232_receiver ***/
RS232_LIB_API void rs232_receiver(scicos_block *block,int flag){
	static int rs232_receiver_is_initialized = 0;
	int rxflag = 1;		
#define RX_BUF_DIM 16 
	unsigned char rbyte[RX_BUF_DIM];

    switch (flag){
         case SCICOS_BLOCK_INIT:	
			if(!rs232_receiver_is_initialized) {
				rs232_receiver_is_initialized = 1;
				/* initialize the receiver console */
				// ... to do ...
				y(0,0) = '\0';
			}
            break;

		case SCICOS_BLOCK_OUTPUT_UPDATE:
			rxflag = sci.read(rbyte, RX_BUF_DIM);
			myfprintf(fuscrs,"received bytes num: %d\n", rxflag);
			if(rxflag < 0){
				myfprintf(fuscrs,"RS232 error while trying to receive.\n");
				myfprintf(fuscrs,"#error: %d\n", GetLastError());
			}
			else
				if(rxflag == 0)
					y(0,0) = '\0';
				else
					y(0,0) = rbyte[rxflag-1];
			// ... to do ... (in case of console)
			break;

        case SCICOS_BLOCK_END:		
			if(rs232_receiver_is_initialized) {
				rs232_receiver_is_initialized = 0;
				/* close the receiver console */
				// ... to do ...
			}
            break;
    }

}
