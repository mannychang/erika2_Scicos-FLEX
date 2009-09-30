#if defined WIN32
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR
#endif

#define FLEX_BUS_DAFAULT_CHANNEL_NUMBER 65535
#define FLEX_BUS_DAFAULT_QUEUES_SIZE 10

#include "compiler.h"
#include "flex_bus_frame.h"

DECLDIR int32_t flex_bus_init(void);
DECLDIR int32_t flex_bus_close(void);
DECLDIR int32_t flex_bus_read(uint8_t *buf, uint16_t len, uint16_t ch);
DECLDIR int32_t flex_bus_write(uint8_t *buf, uint16_t len, uint16_t ch);
