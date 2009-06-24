#ifndef _flex_bus_frame_h_
#define _flex_bus_frame_h_

#include "compiler.h"

#define FLEX_BUS_MESSAGE_SIZE 32

struct flex_bus_message_t {
	uint16_t length;
	uint8_t data[FLEX_BUS_MESSAGE_SIZE];
};

struct flex_bus_packet_t {
	uint16_t channel;
	struct flex_bus_message_t payload;
};

#endif
