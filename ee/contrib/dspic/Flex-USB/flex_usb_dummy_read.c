// 4 Dec 2006 : Coding by Simone Mannori
//              ScicosHIL porting under Windows
//              First working code :) 
//
// 7 Dec 2006 : Final 1.0 version 
// 7 Dec 2007 : Update to Scilab 4.1.2

/* Include files */

#include <windows.h>                    /* Compiler's include files's */
#include <string.h>

#include <stdlib.h>
#include <stdio.h>

#include "scicos_block4.h"  /* Specific for scicos block developement */
#include "machine.h"

 
#define Init           4   /* Initialization */ 
#define OutputUpdate   1
#define StateUpdate    2
#define Integration    0
#define Mode           9    /* Zero Crossing */
#define EventScheduler 3
#define Ending         5   

#define BUF_SIZE 64 /* default buffer size*/ 

int flex_pc_usb_init();

/*
 * Read at most len bytes from usb 
 * RETURN VALUES:
 * - the number of bytes read
 * - -1 on error 
 */
int flex_pc_usb_read(char *buf, int len, unsigned char channel, char readlast);

int flex_pc_usb_close(void);

void flex_usb_dummy_read(scicos_block *block, int flag) 
{
  int err ; 

  char buf[BUF_SIZE];
  int bytes_read;
  float output;

  unsigned char channel ; 
  char readlast = 1     ;

  //** scicos_blocs data structure specific
  double * y           ; //** Roberto
      
  switch(flag) {  
      
     case Init: //** Card and port init
         err = flex_pc_usb_init() ;
         if (err<0)
           sciprint("Init problems \n");
		 else
		   sciprint("Init OK :) \n");
     break; 
     //**------------------------------------------------------------------------------------

     case OutputUpdate:

		 channel = (unsigned char) block->ipar[0] ; 

         bytes_read = flex_pc_usb_read( buf, BUF_SIZE, channel, readlast);
         /* il float è memorizzato a partire dal byte 2 del buffer perchè i
            messaggi hanno il seguente formato:
             - al byte 0 c'è la lunghezza del messaggio
             - al byte 1 c'è il tipo di messaggio
             - al byte 2 ci sono i dati spediti
         */

         output = *(float *) &buf[2]; //** conversion to float (nice trick ;) 
         y = block->outptr[0] ; //** point to the data scructure 
	     y[0] = output ;        //** pass the value to Scicos 
	  	
      break;
      
      case Ending:
          sciprint("Bye Bye baby \n");  //** :)
          flex_pc_usb_close();
      break;	  
        
  } // close the switch
   
} // close the function 

