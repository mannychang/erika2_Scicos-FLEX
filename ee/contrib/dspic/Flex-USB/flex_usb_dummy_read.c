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

typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed long int int32_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned long int uint32_t;

int32_t flex_usbscicos_init(void); 
int32_t flex_usbscicos_close(void);
float flex_usbscicos_read(uint16_t ch);
void flex_usbscicos_write(uint16_t ch, float val);

void flex_usb_dummy_read(scicos_block *block, int flag) 
{
  int err = 0;   
  float output;
  unsigned char channel;   
  double *y; //** Roberto    
      
  switch(flag) {        
  case Init: //** Card and port init
         err = flex_usbscicos_init(1); 	
         if (err < 0)
           sciprint("Init problems \n");
		 else
		   sciprint("Init OK :) \n");
     break; 
  // **-----------------------------------------------------------------------------------
  case OutputUpdate:
		 channel = (unsigned char) block->ipar[0];
		 //sciprint(" Do READ! \n");
         output = flex_usbscicos_read(channel); 
         y = block->outptr[0] ; //** point to the data scructure 	     
		 *y = output;        //** pass the value to Scicos 	  			 
	break;
  // **-----------------------------------------------------------------------------------
  case Ending:
		 err = flex_usbscicos_close();
         if (err < 0)
           sciprint("Close problems \n");
		 else
		   sciprint("Close OK :) \n");
      break;	          
  } // close the switch   
} // close the function 

