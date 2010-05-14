#include <math.h>
#include "machine.h"
#include "scicos_block4.h"

//** INRIA-Scilab 2008
//** Native Integer support for Scicos NAT_GAINBLK
//** 15 Jan 2008 : First prototype 

//** In this function the overflow mechanism is NOT handled or detected

//** Gain Type : int8 [-128	,127] 
//** No action on overflow 
//** Input Type		: char (int8)
//** Output Type	: char (int8)

void nat_gainblk_i8n(scicos_block *block,int flag)
{
  if ((flag==OutputUpdate)|(flag==ReInitialization))
    {
      int i,j,l,ji,jl,il;
      char *u,*y;
      int mu,ny,my,mo,no;
      char *opar;

      char C, D ; //** temporary variables

      mo = GetOparSize(block,1,1);
      no = GetOparSize(block,1,2);
      mu = GetInPortRows(block,1);
      my = GetOutPortRows(block,1);
      ny = GetOutPortCols(block,1);
      
	  u = Getint8InPortPtrs(block,1);
      y = Getint8OutPortPtrs(block,1);
      opar = Getint8OparPtrs(block,1);

      if (mo*no==1)
	    {
			for (i=0;i<ny*mu;++i)
			   {
				   D = (char)(opar[0])*(char)(u[i]);
                   y[i] = (char)D;
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
		                         C =(char)(opar[ji])*(char)(u[il]);
		                         D = D + C;
							 }
		                  y[jl] = (char)D;
		              }
	            }
         }
     }
}
