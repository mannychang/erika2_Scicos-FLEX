/* _SERIAL_GATEWAY_H_ */

#ifndef _SERIAL_GATEWAY_H_
#define _SERIAL_GATEWAY_H_

#ifdef SERIAL_GATEWAY_LIB_EXPORTS
#define SERIAL_GATEWAY_LIB_API	__declspec(dllexport) 
#else
#define SERIAL_GATEWAY_LIB_API	__declspec(dllimport) 
#endif

/* To make this header compatible with c sources */
#ifdef __cplusplus
extern "C"{
#endif
#include "scicos_block4.h"
SERIAL_GATEWAY_LIB_API void serial_gateway_block(scicos_block *block, int flag);
SERIAL_GATEWAY_LIB_API HWND GetGatewayHwnd(void);
#ifdef __cplusplus
}
#endif


/*** Definitions ***/
/* This macro should be used to read block inputs 
 * i: is the index to select the line
 * j: is the index to select the element of the i-th line 
 */
#define input(block,i,j)  ((double *)block->inptr[i])[j]   

/* This macro should be used to write block outputs 
 * i: is the index to select the line
 * j: is the index to select the element of the i-th line 
 */
#define output(block,i,j)  ((double *)block->outptr[i])[j]

/* This macro should be used to read integer-type parameters */
#define ipar(block,i) (block->ipar[i])

/* This macro should be used to read integer-type parameters */
#define rpar(block,i) (block->rpar[i])


/*** Global variables ***/
#ifdef _DEBUG
extern FILE* fserial_gateway_debug;      /* File to be used for debug purpose */
#define start_debug() ((fserial_gateway_debug = _fsopen("serial_gateway_debug.log", "w", _SH_DENYNO)) != NULL)
#define print_debug(...) fprintf(fserial_gateway_debug, __VA_ARGS__)
#define flush_debug() fflush(fserial_gateway_debug)
#define stop_debug() fclose(fserial_gateway_debug)
#else
#define start_debug() (0)
#define print_debug(...) (0)
#define flush_debug() (0)
#define stop_debug() (0)
#endif

#endif /* _SERIAL_GATEWAY_H_ */
