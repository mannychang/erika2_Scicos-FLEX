#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#ifdef WIN32
	#include "network_utils_win32.h"
#else
	#include "network_utils_linux.h"
#endif

#endif
