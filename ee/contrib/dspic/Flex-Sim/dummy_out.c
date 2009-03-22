//** 3 Feb 2008 : standard prototype of the empty output function
//**              used for off line simulation with Scicos 
#include "machine.h"
#include "scicos_block4.h"

static void init(scicos_block *block)
{
  //** nothing to do 
}

static void inout(scicos_block *block)
{
	//** nothing to do 
}

static void end(scicos_block *block)
{
    //** nothing to do 
}

void dummy_out(scicos_block *block,int flag)
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
