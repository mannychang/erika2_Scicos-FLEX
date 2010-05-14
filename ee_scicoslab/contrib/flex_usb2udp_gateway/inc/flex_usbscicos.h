#if defined WIN32
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR
#endif

#include "compiler.h"

DECLDIR int32_t flex_usbscicos_init(unsigned char block_type); 
DECLDIR int32_t flex_usbscicos_close(void);
DECLDIR float flex_usbscicos_read(uint16_t ch);
DECLDIR void flex_usbscicos_write(uint16_t ch, float val);
