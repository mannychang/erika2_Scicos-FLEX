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

void rt_lcd(scicos_block *block,int flag)
{
	switch (flag) {
		case 1:	/* set output */
		case 2:	/* get input */
			inout(block);
			break;
		case 5:	/* ending */
			end(block);
			break;
		case 4:	/* initialisation */
			init(block);
			break;
	}
}
