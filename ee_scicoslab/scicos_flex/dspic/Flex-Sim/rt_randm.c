/*
 * ERIKA Enterprise Basic - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2010-2012  Evidence Srl
 *
 * This file is part of ERIKA Enterprise Basic.
 *
 * ERIKA Enterprise Basic is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * ERIKA Enterprise Basic is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with ERIKA Enterprise Basic; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 */

//#include "stdafx.h"
#include <stdlib.h> /* for rand()*/

#include "flexsim.h"

// Types: Use "float" and not "double". 
// This improves the accuracy of the simulation.

static void init(scicos_block *block)
{
  float seed = (float)rpar(1)  ;

  srand ( (int)seed ); // randomize the generator
  output(0,0) = 0.0;
}

static void inout(scicos_block *block)
{
  float amplitude = (float)rpar(0); 

  //** one random integer [0 RAND_MAX]
  output(0,0) = amplitude * ( (float)rand() / (float)RAND_MAX  ) ; 
}

static void end(scicos_block *block)
{
  output(0,0) = 0.0;
}


//** This block produce a pseudo random output 

FLEXSIM_LIB_API void rt_randm(scicos_block *block,int flag)
{
  if (flag==OutputUpdate){
    /* set output */
    inout(block);
  }
  else if (flag==Ending){
    /* termination */ 
    end(block);
  }
  else if (flag ==Initialization){
    /* initialisation */
    init(block);
  }
}


