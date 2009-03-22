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

#include "machine.h"
#include "scicos_block4.h"

#include <math.h>

static void init(scicos_block *block)
{
  double * y = block->outptr[0];
  y[0]=0.0;
}

static void inout(scicos_block *block)
{
  double w,pi=3.1415927;
  double * y = block->outptr[0];
  double t = get_scicos_time();

   if (t<block->rpar[4]) y[0]=0.0;
   else {
     w=fmod(2*pi*block->rpar[1]*(t-block->rpar[4])-block->rpar[2],(2*pi));
     y[0]=block->rpar[0]*sin(w)+block->rpar[3];
   }
}

static void end(scicos_block *block)
{
  double * y = block->outptr[0];
  y[0]=0.0;
}



void rt_sinus(scicos_block *block,int flag)
{
  if (flag==1){          /* set output */
    inout(block);
  }
  else if (flag==5){     /* termination */ 
    end(block);
  }
  else if (flag ==4){    /* initialisation */
    init(block);
  }
}


