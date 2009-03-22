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

#define BUF_SIZE 64 /* default buffer size*/ 


void flex_usb_dummy_write(scicos_block *block, int flag) 
{
  int err ; 
  char buf[BUF_SIZE];
  int bytes_read;
  float output;

  unsigned char channel ; 
  char readlast = 0     ;
  int len = 4 ; //** 4 byte = 1 float :) 

  //** scicos_blocs data structure specific
  float  volts     ; //* the output variable that you have ge 
  double * u       ; //** Roberto Bucher

  switch(flag) {  
      
      case Init: //** Card and port init   
         //
		 // ------- Put the initialization code here ----------------------------------
		 // 
      
      break; 
      
      case StateUpdate:
		  u = block->inptr[0] ; //** get the input from Scicos (pointer)
	      volts = u[0] ; //** put in a variable
		  //** now your input signal is stored in "volts": you can use it to generate the
		  //** physical output 
		 
		 //
		 // ------- Put the running code here ----------------------------------
		 // 



      break;
      
      case Ending:
		 //
		 // ------- Put the ending code here ----------------------------------
		 // 
      break;	  
        
  } // close the switch
   
} // close the function 

