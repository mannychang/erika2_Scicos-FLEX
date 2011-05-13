#include <math.h>
#include "machine.h"
#include "scicos_block4.h"

//** INRIA-Scilab 2008
//** Native Integer support for Scicos NAT_GAINBLK
//** 15 Jan 2008 : First prototype 

//** In this function the overflow mechanism is NOT handld or detected

//** Gain Type : int32 [-2.147.483.648,	2.147.483.647]
//** No action on overflow 
//** Input Type		: long (int32)
//** Output Type	: long (int32) 
void nat_gainblk_i32n(scicos_block *block,int flag)
{
 if ((flag==OutputUpdate)|(flag==ReInitialization))
   {
	 int   i, j, l, ji, jl, il ; //** used for index 
     
	 long *u,*y; //** input, output pointers 
     int   mu,ny,my,mo,no; //** size of internal object and signals 
     
	 long *opar; //** object (block parameter) "gain" 
     
     long C, D ; //** temporary variables

     //** size of the input scalar/vector
     mu = GetInPortRows(block,1);
     
	 //* size of the output scalar/vector/matrix 
	 my = GetOutPortRows(block,1);
     ny = GetOutPortCols(block,1);
     
	 //** input port pointer 
	 u  = Getint32InPortPtrs(block,1);
	 //** output port pointer 
     y  = Getint32OutPortPtrs(block,1);
     
	 //** size of the "gain" scalar/vector/matrix
     mo = GetOparSize(block,1,1);
     no = GetOparSize(block,1,2);

     opar = Getint32OparPtrs(block,1);
     
	 if (mo*no==1) //** single input / single output        
	   {
         for (i=0; i<ny*mu; ++i) //** scan the outputvector / inputvector
		    {
			//** y = gain * u ; 
                 D = (long)(opar[0])*(long)(u[i]); // scan the element of the in vector
               y[i]= (long)D; //** put the result in the elements of the output vector
            }
       }
	 else //** the input / output are matrix 
	   {
         for (l=0;l<ny;l++) //** scan the output port columns 
	        {
			  for (j=0; j<my; j++); //**scan the output port rows
	             {
					D = 0 ;  
	                for (i=0; i<mu; i++)
		               {
						 ji = j+i*my;
		                 jl = j+l*my;
		                 il = i+l*mu;
		                 C = (long)(opar[ji])*(long)(u[il]);
		                 D = D + C;
			            } //**scan the output port rows

			       y[jl] = (long)D;

	              }//** scan the output port columns 
        
		     } //** end ... the input / output are matrix 
	   
	     } //** and of ... single input / single output 

   } //** end ... if ((flag==OutputUpdate)|(flag==ReInitialization)

}//** end ... function : "nat_gainblk_i32n"

