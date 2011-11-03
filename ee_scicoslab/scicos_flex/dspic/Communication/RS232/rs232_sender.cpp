#include "stdafx.h"
#include "dll_rs232.h"
#include "rs232_sender.h"


/*** rs232_sender ***/
RS232_LIB_API void rs232_sender(scicos_block *block,int flag){
	static int rs232_sender_is_initialized = 0;
	int txflag = 1;		
	unsigned char sbyte;
	
    switch (flag){
         case SCICOS_BLOCK_INIT:
			if(!rs232_sender_is_initialized) {
				rs232_sender_is_initialized = 1;
				/* initialize the sender console */
				// ... to do ...
			}
            break;


        case SCICOS_BLOCK_OUTPUT_UPDATE:
			sbyte = u(0,0);
			txflag = sci.write(&sbyte, 1);
			if (txflag < 0) {
				myfprintf(fuscrs,"RS232 error while trying to transmit.\n");
				myfprintf(fuscrs,"#error: %d\n", GetLastError());
			}
			myfprintf(fuscrs,"sent byte: %d\n", sbyte);
			// ... to do ... (in case of console)
			break;

        case SCICOS_BLOCK_END:
			if(rs232_sender_is_initialized) {
				rs232_sender_is_initialized = 0;
				/* close the sender console */
				// ... to do ...
			}
			
            break;
    }

}

