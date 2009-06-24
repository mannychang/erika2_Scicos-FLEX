#ifndef _flex_usb_frame_h_
#define _flex_usb_frame_h_

#define FLEX_USB_HEADER_START 0x15	/**< The start header: 10101 */
#define FLEX_USB_PACKET_PAYLOAD_SIZE 60	/**< The packet payload size */


typedef unsigned char uint8_t;
typedef unsigned int uint16_t;

struct flex_usb_packet_t {
	/*CHRIS: TODO: METTERE LA CRC IN FONDO!!!!! */
	unsigned crc : 8;		/**< The checksum, including header */
	unsigned start : 5;	/**< Commone header 10101 */
	unsigned type : 3;	/**< Packet type */
	unsigned more : 1; 	/**< Is last fragment */
	unsigned seq : 4; 	/**< Fragment sequence number */
	unsigned reserved1 : 3;	/**< Reserverd (seq, length) */
	unsigned reserved2 : 2;	/**< Reserverd (seq, length) */
	unsigned length : 6;	/**< Payload lenght */
	uint8_t payload[FLEX_USB_PACKET_PAYLOAD_SIZE]; /**< The packet payload*/
	//uint8_t crc;		/**< The checksum, including header */
};

enum flex_usb_header_type_t {
	FLEX_USB_HEADER_NULL = 0x00,	
	FLEX_USB_HEADER_COMMAND = 0x01,	
	FLEX_USB_HEADER_DATA = 0x02,
	FLEX_USB_HEADER_ACK = 0x03,
};

enum flex_usb_command_type_t {
	FLEX_USB_COMMAND_CONNECT = 0x0001,
	FLEX_USB_COMMAND_DISCONNECT = 0x0002,
};

#endif
