#include <math.h>
#include "scicos_block4.h"
#include <stdio.h>

//** INRIA-Scilab 2008
//** Native Integer support for Scicos NAT_GAINBLK
//** 15 Jan 2008 : First prototype 

//** In this function the overflow mechanism is NOT handled or detected

//** Multiplication by n : int32 [-2.147.483.648,	2.147.483.647]
//** No action on overflow 
//** Input Type		: long (int32)
//** Output Type	: long (int32) 


///================ BEWARE =========== ; ======= WORK IN PROGRESS ================== ; 

void nat_product_i32n(scicos_block *block,int flag)
{
  int j, k; //** index 

  long *u; //** input 
  long *y; //** output (generic int32 pointer)

  ipar = GetIparPtrs(block); //** get the (* or / )

  nu = GetInPortRows(block,1); //** Row of first single input
  mu = GetInPortCols(block,1); //** Col of first single input  

  y = Getint32OutPortPtrs(block,1); //** recover the output pointer 

  nin = GetNin(block); //** number of INPUT ports

  //** This block is executed ONLY in this phase
  if (flag==OutputUpdate)
    {
      if (nin==1) //** the block has only one input port
	    {
	      //** the single input is handled as a vector/matrix: the results is the 
	      //** component's producttorial. For a vector : y = u1 * u2 * .... u(n)
          //** For a matrix : y = u11 * u12 * ... u(nu,mu) ;
          y[0] = 1 ; //** put the default to one (invariant data for multiplication)
		  u = Getint32InPortPtrs(block,1); //** get the pointer of the FIRST SINGLE input port 
          for (j=0; j<nu*mu; j++)
		     {
	           y[0] = y[0]*block->inptr[0][j];
             }
         }
      else //** more than one input 
	     {
          for (j=0; j<nu*mu ; j++) //** scan ALL the input e
		     {
                //**----------------------------------------------------------------------
	            y[0] = 1 ; //** put the default to one (invariant data for multiplication)
				for (k=0; k < nin; k++)
				   {    //** NB the FIRST port is "1" -> "k+1"
					 u = Getint32InPortPtrs(block,k+1); //** recover the input port pointer
	                 //** NB this kind of looping works because the previous instruction
					 //**    "shift" the input pointer at each scan ! 
					 if (ipar[k]>0)
					   { //** positive case : MULTIPLICATION 
	                     y[j] = y[j] * u[j]; // (*)
	                   }
					 else //** negative case : DIVISION 
					   {
	                     if (u[k][j]==0)
						   { //** check if the input variable is zero 
	                         set_block_error(-2);
	                         return ;
						   }
						 else //** the division is allowed 
						   {
	                         y[j] = y[j] / u[j];
	                       }
	                    }
	                
				   }//** .. for() loop 
                //**----------------------------------------------------------------------
			 }//** for loop  
      }
   
	}//** end 

}//** end function
