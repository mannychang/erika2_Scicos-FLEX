#include <math.h>
#include "machine.h"
#include "scicos_block4.h"

//** INRIA-Scilab 2008
//** Native Integer support for Scicos NAT_GAINBLK
//** 15 Jan 2008 : First prototype 

//** In this function the overflow mechanism is NOT handled or detected

//** Gain Type : int16 [-32.768	,32.767] 
//** No action on overflow 
//** Input Type		: short (int16)
//** Output Type	: short (int16)

void nat_gainblk_i16n(scicos_block *block,int flag)
{
  if ((flag==OutputUpdate)|(flag==ReInitialization))
    {
		int   i,j,l,ji,jl,il; //** used for index 
        
		short *u,*y; //** input, output pointers 
        int mu,ny,my,mo,no; //** size of internal object and signals
        
		short *opar; //** object (block parameter) "gain"
  
        short C, D ; //** temporary variables

        mo = GetOparSize(block,1,1);
        no = GetOparSize(block,1,2);
  
        mu = GetInPortRows(block,1);
        my = GetOutPortRows(block,1);
        ny = GetOutPortCols(block,1);
        u  = Getint16InPortPtrs(block,1);
        y  = Getint16OutPortPtrs(block,1);
        opar = Getint16OparPtrs(block,1);
  
        if (mo*no==1)
		  {
			  for (i=0;i<ny*mu;++i)
			     {
                   D    = (short)(opar[0])*(short)(u[i]);
                   y[i] = (short)D;
                 }
           }
		else
		  {
			  for (l=0;l<ny;l++)
			     {
					 for (j=0;j<my;j++)
	                    {
							D = 0;
	                        for (i=0;i<mu;i++)
		                       {
								   ji = j+i*my;
		                           jl = j+l*my;
		                           il = i+l*mu;
		                           C = (short)(opar[ji])*(short)(u[il]);
		                           D = D + C;
							   }
                             y[jl]=(short)D;
		                 }
	              }
           }//** end of maxtrix case 
     }//** ...end of  
}//**...end of function
