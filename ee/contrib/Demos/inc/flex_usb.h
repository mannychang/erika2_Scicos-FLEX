#ifndef _flex_usb_h_
#define _flex_usb_h_
/* chris: capire questa cosa delle dll */
#if defined WIN32
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR
#endif

#include "compiler.h"

#define FLEX_USB_VID 0x04d8	/**< USB Vendor Identifier */
#define FLEX_USB_PID 0x0033	/**< USB Product Identifier */
#define FLEX_USB_EP_IN 0x81	/**< USB Input end-point */
#define FLEX_USB_EP_OUT 0x01	/**< USB Output end-point */

/** \brief Shift queue front, override oldest data, on RX queue full. */
#define FLEX_USB_RX_BUFFER_FULL_SHIFT 0x00
/** \brief Block USB reader thread on RX queue full. */
#define FLEX_USB_RX_BUFFER_FULL_BLOCK 0x40
/** \brief Reception with acknowledgment */
#define FLEX_USB_RX_ACKED 0x10
/** \brief Reception with fragmentation */
#define FLEX_USB_RX_FRAGMENT 0x08
/** \brief Transmission with acknowledgment */
#define FLEX_USB_TX_ACKED 0x02
/** \brief Transmission with fragmentation */
#define FLEX_USB_TX_FRAGMENT 0x01

/** \brief Blocking reading is performed */
#define FLEX_USB_READ_BLOCK 0x00
/** \brief Non-blocking reading is performed */
#define FLEX_USB_READ_NOBLOCK 0x01

DECLDIR int32_t flex_usb_init(uint32_t rx_buf_size, uint32_t tx_buf_size, uint8_t opt);

DECLDIR int32_t flex_usb_read(uint8_t *buf, uint32_t len, uint8_t opt);

DECLDIR int32_t flex_usb_write(uint8_t *buf, uint32_t len);

DECLDIR int32_t flex_usb_close(void);

#endif
