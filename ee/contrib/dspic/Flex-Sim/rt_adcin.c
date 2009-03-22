#include "machine.h"
#include "scicos_block4.h"

#define VREF	3.3

static void init(scicos_block *block)
{
	//** nothing to do 
}

static void inout(scicos_block *block)
{
	unsigned int adcdata;
	float * y = block->outptr[0];
	
	int pin = block->ipar[0];

    adcdata = 0 ; // force the output to zero 

	y[0] = (adcdata * VREF) / 4096;
}

static void end(scicos_block *block)
{
	//** nothing to do 
}

void rt_adcin(scicos_block *block,int flag)
{
	switch (flag) {
		case 1:	/* set output */
			inout(block);
			break;

		case 2:	/* get input */
			inout(block);
			break;
		
		case 4:	/* initialisation */
			init(block);
			break;
		
		case 5:	/* ending */
	        end(block);
			break;
	}
}
