#include <math.h>
#include "scicos_block4.h"
#include <stdio.h>

//** INRIA-Scilab 2008
//** Native Integer support for Scicos NAT_GAINBLK
//** 15 Jan 2008 : First prototype 

//** In this function the overflow mechanism is NOT handled or detected

//** Gain Type : int32 [-2.147.483.648,	2.147.483.647]
//** No action on overflow 
//** Input Type		: long (int32)
//** Output Type	: long (int32) 

void nat_summation_i32n(scicos_block *block,int flag)
{
  if ((flag==OutputUpdate)|(flag==ReInitialization))
    {
		int j,k ;       //** index 
        int nu,mu,nin;

        double *rpar; //** NEVER used in this code (in/out datatype)

		int *ipar; //** input vector sign 
        long *u; //** input 
        long *y; //** output (generic int32 pointer)

        y = Getint32OutPortPtrs(block,1); //** recover the output pointer 
       
		nu = GetInPortRows(block,1);
        mu = GetInPortCols(block,1);
        ipar = GetIparPtrs(block);
        rpar = GetRparPtrs(block);
        nin  = GetNin(block);

        if (nin==1) //** only ONE input !
		  {
            y[0] = 0;
            u    = Getint32InPortPtrs(block,1);
            for (j=0; j<nu*mu; j++)
			   {
	             y[0] = y[0]+u[j];
               }
          }
        else //** more than ONE input 
		  {
            for (j=0; j<nu*mu; j++)
			   {
                 y[j]=0;
	             for (k=0; k<nin; k++)
				    {
	                  u = Getint32InPortPtrs(block,k+1);
                      if (ipar[k]>0) //** this handle the (+/-)(sign)
	                      y[j] = y[j] + u[j]; //** (+)
	                  else
	                      y[j] = y[j] - u[j]; //** (-) 
					    
                     } //** inner loop 
                } //** outer loop 
           } //** more than one input
   } //** flag 

}
