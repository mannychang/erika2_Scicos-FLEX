#include "stdafx.h"
#include "dll_rs232.h"
#include "rs232_config.h"

volatile int rs232_port_is_initialized = 0;
volatile int rs232_config_file_is_opened = 0;

/*** rs232_config ***/
RS232_LIB_API void rs232_config(scicos_block *block,int flag){
	unsigned long int noblock = 1;
	
	char comname[8];

    switch (flag){
         case SCICOS_BLOCK_INIT:	
			if(!rs232_config_file_is_opened) {
				if(fuscrs != NULL)
					rs232_config_file_is_opened = 1;
			}
			if(!rs232_port_is_initialized) {	
				for(int i=0; i<GetNipar(block); i++)
					myfprintf(fuscrs,"ipar(%d)=%d\n",i,ipar(i));	
				char digit0 = '0' + RS232_COM_PORT % 10;
				char digit1 = '0' + RS232_COM_PORT / 10 % 10;
				if(digit1 != '0')
					sprintf(comname, "COM%c%c", digit1, digit0);
				else
					sprintf(comname, "COM%c", digit0);
				myfprintf(fuscrs,"comname=%s\n",comname);	
				long int baudrate = (long int)RS232_BAUDRATE;
				myfprintf(fuscrs,"baudrate=%d\n",baudrate);	
				if(sci.open(comname,ParityNone,FlowNone,baudrate,8,1,SERIAL_MAX_RX_BUF) == true) {
					sci.purge();
					sci.setReadTimeOut(100);
					rs232_port_is_initialized = 1;
				}
				else{
					myfprintf(fuscrs,"RS232 error while trying to open the port.\n");
					myfprintf(fuscrs,"#error: %d\n", GetLastError());
				}
			}
            break;

        case SCICOS_BLOCK_OUTPUT_UPDATE:		
			break;

        case SCICOS_BLOCK_END:		
			myfprintf(fuscrs,"rs232_config: closing...done!\n");
			if(rs232_port_is_initialized) {
				sci.close();
				rs232_port_is_initialized = 0;
			}
			if(rs232_config_file_is_opened){
				fclose(fuscrs);
				rs232_config_file_is_opened = 0;
			}
            break;
    }

}



