#include "machine.h"
#include "scicos_block4.h"

static void init(scicos_block *block)
{
  //** nothing to do 
}

static void inout(scicos_block *block)
{
  float * y = block->outptr[0];

  unsigned int pin = block->ipar[0];

  //** return back a constant value
  y[0] = 0.0; //** "0.0" (float)

}

static void end(scicos_block *block)
{
	//** nothing to do 
}

void rt_gpin(scicos_block *block,int flag)
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
