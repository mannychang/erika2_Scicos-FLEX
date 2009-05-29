//** 24 June 2008 : 

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

int32_t flex_bus_init(void);
int32_t flex_bus_read(uint8_t *buf, uint16_t len, uint16_t ch);
int32_t flex_bus_write(uint8_t *buf, uint16_t len, uint16_t ch);

void flex_usb_dummy_write(scicos_block *block, int flag) 
{
  int err = 0;      
  unsigned char channel;       
  double *u; //** Roberto Bucher
  float output;

  switch(flag) {        
      case Init: //** Card and port init            
		err = flex_bus_init() ;
		if (err<0)
			sciprint("Init problems \n");
		else
			sciprint("Init OK :) \n");      
		break;       
      case StateUpdate:
		channel = (unsigned char) block->ipar[0] ;
		u = block->inptr[0] ; //** get the input from Scicos (pointer)	     
		output = *u;
		flex_bus_write((unsigned char*) &output, sizeof(float), channel);
		break;      
      case Ending:	
		flex_usb_close();
        sciprint("Usb Closed\n");  //** :)         
		break;	  
        
  } // close the switch   
} // close the function 

