
#if defined WIN32
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR
#endif

/*
 * Initializes the library for using usb.
 * RETURN VALUES:
 * - 0 on correct initialization
 * - non 0 on error */
DECLDIR int flex_pc_usb_init();

/*
 * Read at most len bytes from usb 
 * RETURN VALUES:
 * - the number of bytes read
 * - -1 on error 
 */
DECLDIR int flex_pc_usb_read(char *buf, int len, unsigned char channel, char readlast);

/*
 * Write len bytes to usb
 */
DECLDIR int flex_pc_usb_write(char *buf, int len, unsigned char channel);

/*
 * Close the usb connection
 */
DECLDIR int flex_pc_usb_close(void);
