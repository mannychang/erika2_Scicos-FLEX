/*
 * ERIKA Enterprise Basic - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2007  Evidence Srl
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

#include "stdafx.h"
#include <stdlib.h> /* for rand()*/
#include "machine.h"
#include "flexsim.h"
#include <math.h>

// Types: Use "float" and not "double". 
// This improves the accuracy of the simulation.

static void init(scicos_block *block)
{
  output(0,0) = 0.0;
}

static void inout(scicos_block *block)
{
  float w, pi = 3.1415927;
  float t = get_scicos_time();
  float amplitude = rpar(0);
  float frequency = rpar(1);
  float phase =     rpar(2);
  float bias =      rpar(3);
  float delay =     rpar(4);

   if (t < delay) 
		output(0,0) = 0.0;
   else {
		w = fmod(2*pi*frequency*(t - delay) - phase,(2*pi));
		output(0,0) = amplitude * sin(w) + bias;
   }
}

static void end(scicos_block *block)
{
  output(0,0) = 0.0;
}



FLEXSIM_LIB_API void rt_sinus(scicos_block *block,int flag)
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


