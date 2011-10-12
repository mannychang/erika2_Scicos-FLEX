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

#include "flexsim.h"

static void init(scicos_block *block)
{
	// to do: initialization of the figure
}

static void inout(scicos_block *block)
{
	// to do: display the value in the figure
}

static void end(scicos_block *block)
{
	// to do: figure termination procedure
}

FLEXSIM_LIB_API void flex_gpout(scicos_block *block,int flag)
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


