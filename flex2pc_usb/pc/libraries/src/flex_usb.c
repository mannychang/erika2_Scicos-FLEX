/* chris: TODO generali 
	  1- Mettere un comment header sensato
	  2- Spostare le funzioni statiche tutte all'inizio in un'unica section
	  3- Devo mantenere la roba della DLL (vedi DECLDIR nel punto .h)
	  4- Usare convenzione di nomi sensata
	  5- Segare il codice a 80 colonne
	  6- mettere doxygen
*/

#include <usb.h>	
#include <stdio.h>	
#include <pthread.h>	
#include <sched.h>
#include <string.h>
#include <stdlib.h>
#include "compiler.h"
#include "flex_usb.h" 
#include "flex_usb_frame.h" 

/* Debugging*/
//#define FLEX_USB_DEBUG
#define FLEX_USB_DEBUG_TAG "flex_usb DEBUG: "

#define FLEX_USB_TX_BUFFER_SIZE 256
#define FLEX_USB_RX_BUFFER_SIZE 256

union flex_usb_opt_t {
	struct {
	unsigned rx_buffer_policy : 2; 
	unsigned rx_acked : 1; 		/* chris: TODO: not yet used */
	unsigned rx_fragment : 1; 	/* chris: TODO: not yet used */
	unsigned tx_buffer_policy : 2; 	/* chris: TODO: not yet used */
	unsigned tx_acked : 1; 		/* chris: TODO: not yet used */
	unsigned tx_fragment : 1;	/* chris: TODO: not yet used */
	} option;
	uint8_t options;
};

union flex_usb_stat_t {
	struct {
		unsigned init : 1;
		unsigned stop_reader : 1;
	} stat;
	uint8_t stats;
};

static union flex_usb_opt_t opt_info;
static union flex_usb_stat_t stat_info = {0, 0};

static usb_dev_handle *dev = NULL; /* the device handle */
uint8_t usb_interface_id = 0;
static uint8_t *tx_buffer = NULL;
static uint8_t *rx_buffer = NULL;
static uint32_t rx_buffer_size = 0;
static uint32_t tx_buffer_size = 0;
static volatile uint32_t rx_first = 0;
static volatile uint32_t rx_last = 0;
static volatile uint32_t rx_held = 0;

static pthread_t reader_tid; //tid of stop_reader	
static pthread_mutex_t usb_link_mutex;
static pthread_mutex_t tx_mutex;
static pthread_mutex_t rx_mutex;
static pthread_cond_t rx_condition;


/* 
 * compute a checksum
 */
static uint8_t checksum(uint8_t *buffer, uint8_t len)
{
	uint8_t sum = 0;

	while (len-- > 0) 
		sum += *(buffer++);
	return sum;
}

static void write_rx_buffer_shift(uint8_t *buf, uint32_t size) 
{
	uint32_t tmp_idx;

	pthread_mutex_lock(&rx_mutex);
	if (size >= rx_buffer_size) {
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr, "WRITE RX_QUEUE: override whole buffer\n");
		fflush(stderr);
		#endif
		/* If the read message is larger than the whole buffer,
		   write the last rx_buffer_size bytes in the buffer.
		   TODO: is this a possible scenario?? */
		memcpy(rx_buffer, buf + size - rx_buffer_size, 
		       rx_buffer_size * sizeof(uint8_t));
		rx_last = 0;
		rx_first = 0;
		rx_held = rx_buffer_size;
	} else {
		if (rx_buffer_size - rx_held < size) {
			/* Overriding condition: implement shift policy on the
			   rx_first indec,  tmp_idx=amount of overriding bytes*/
			tmp_idx = size - (rx_buffer_size - rx_held);
			rx_first = (rx_first + tmp_idx) % rx_buffer_size;
		}
		/* NOTE: tmp_idx = new_last position for rx */
		tmp_idx = (rx_last + size) % rx_buffer_size;
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr, "WRITE RX_QUEUE: rx_last=%d", rx_last);
		fprintf(stderr, " new_rx_last=%d", tmp_idx);
		fprintf(stderr, " size=%d", size);
		fprintf(stderr, " rx_held=%d", rx_held);
		fprintf(stderr," rx_buffer_size=%d\n",rx_buffer_size);
		fflush(stderr);
		#endif
		if (tmp_idx > rx_last) {
			/* The message fits from rx_last to the 
			   end of the buffer. */
			memcpy(rx_buffer + rx_last, buf, size*sizeof(uint8_t));
		} else {
			/* The first part of message must be stored 
			   from rx_last to the end of the buffer, the 
			   remaining part goes from begin buffer to
			   tmp_idx index.  */
			memcpy(rx_buffer + rx_last, buf, 
			       (rx_buffer_size - rx_last) * sizeof(uint8_t));
			memcpy(rx_buffer, buf + rx_buffer_size - rx_last, 
			       (tmp_idx + 1) * sizeof(uint8_t));
		}
		rx_last = tmp_idx;
		rx_held += size;
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr, 
			"WRITE_EXIT: rx_held , size (%d , %d)\n",
			rx_held, size);
		#endif
	}
	pthread_cond_signal(&rx_condition);
	pthread_mutex_unlock(&rx_mutex);
}

/*
 * Thread for reading from USB
 */
/* chris: TODO: riscrivere quasi da zero, vedere cosa serve mantenere! */
static void *reader_thread(void *ignored_param)
{
	struct flex_usb_packet_t pkt;
	int read_bytes;

	/* USB Thread master loop: process data from the USB link */
	while (!stat_info.stat.stop_reader) {	
		pthread_mutex_lock(&usb_link_mutex);
		read_bytes = usb_claim_interface(dev, usb_interface_id);
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr,"check: usb_claim returns %d\n", read_bytes);
		#endif
		read_bytes = usb_bulk_read(dev, FLEX_USB_EP_IN, (uint8_t*) &pkt,
					   sizeof(struct flex_usb_packet_t), 
					   2000);
		if (read_bytes < 1 || 
		    read_bytes != sizeof(struct flex_usb_packet_t)) {
			#ifdef FLEX_USB_DEBUG
			fprintf(stderr, FLEX_USB_DEBUG_TAG);
			fprintf(stderr, "error:  bulk read failed, %d bytes\n", 
				read_bytes);
			fflush(stderr);
			#endif
			pthread_mutex_unlock(&usb_link_mutex);
			continue;
		}
		pthread_mutex_unlock(&usb_link_mutex);
		/* chris: FIXME: REMOVE this test!!!!!!*/
		#ifdef FLEX_USB_DEBUG
		//buf[60] = '\0'; // chris: giusto per limitare il printf
		pkt.payload[59] = '\0';
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr, "Read %d bytes; string(%d) = %s\n", read_bytes,
			pkt.length, pkt.payload);
		fflush(stderr);
		#endif
		/* Apply the specified RX buffer policy */
		switch (opt_info.options & 0xC0) {	
		case FLEX_USB_RX_BUFFER_FULL_SHIFT:
			write_rx_buffer_shift(pkt.payload, pkt.length);
			break;
		#ifdef FLEX_USB_DEBUG
		default:
			fprintf(stderr, FLEX_USB_DEBUG_TAG);
			fprintf(stderr, "Error, invalid rx buffer policy!");
			fflush(stderr);
		#endif
		}
// chris TODO: il for serve a testare un po' il semaforo di condition
//for(i = 0; i < 1000000; i++);
	} /* End Of master loop */
	return NULL;
}


static usb_dev_handle *open_device(void)
{
	struct usb_bus *bus;
	struct usb_device *dev;
	
	#ifdef FLEX_USB_DEBUG
	fprintf(stderr, FLEX_USB_DEBUG_TAG);
	fprintf(stderr, "Scan devices... \n");
	#endif

	/* chris: perchè faccio un get_busses in piu' ????? */
	bus = usb_get_busses();
	for (bus = usb_get_busses(); bus != NULL; bus = bus->next) {
		for (dev = bus->devices; dev != NULL; dev = dev->next) {
			#ifdef FLEX_USB_DEBUG
			fprintf(stderr, FLEX_USB_DEBUG_TAG);
			fprintf(stderr, "Device: vendor=%x product=%x\n", 
				dev->descriptor.idVendor, 
				dev->descriptor.idProduct);
			#endif
			if (dev->descriptor.idVendor == FLEX_USB_VID && 
			    dev->descriptor.idProduct == FLEX_USB_PID) {
				usb_interface_id = dev->config->interface->
						   altsetting->bInterfaceNumber;
				return usb_open(dev);
			}
		}
	}
	return NULL;
}


DECLDIR 
int32_t flex_usb_init(uint32_t rx_size, uint32_t tx_size, uint8_t opt)
{
	int32_t ret;
	int policy;
	struct sched_param param;
	pthread_attr_t t_attr;
	struct flex_usb_packet_t cmd;

	/* chris: init non deve ritornare 0 (non errore) a meno che non 
		  setti prima stop_reader=0 altrimenti non funziona più il 
	          main thread!
	*/
	//stat_info.stat.stop_reader = 1;
	stat_info.stat.stop_reader = 0;
	if (stat_info.stat.init)
		return 0;
	// TODO: try something like this in future
	/* Wait the end of the thread reader before creating it again */
	//if (stat_info.stat.stop_reader == 1) { 
	//	pthread_join(reader_tid, NULL);
	//	stat_info.stat.stop_reader = 0;
	//}
	opt_info.options = opt;
	/* chris: TODO: scegliere una politica appropriata invece di < 64!!*/
	rx_buffer_size = rx_size < sizeof(struct flex_usb_packet_t) ? 
			 FLEX_USB_RX_BUFFER_SIZE : rx_size;
	tx_buffer_size = tx_size < sizeof(struct flex_usb_packet_t) ? 
			 FLEX_USB_TX_BUFFER_SIZE : tx_size;
	if (rx_buffer != NULL)
		free(rx_buffer);
	if (tx_buffer != NULL)
		free(tx_buffer);
	rx_buffer = (uint8_t *) malloc(rx_buffer_size * sizeof(uint8_t));
	tx_buffer = (uint8_t *) malloc(tx_buffer_size * sizeof(uint8_t));
	if (rx_buffer == NULL) {
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr,"fatal error: unable to allocate rx buffer\n");
		#endif
		return -5;
	}
	if (tx_buffer == NULL) {
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr,"fatal error: unable to allocate tx buffer\n");
		#endif
		return -5;
	}
	/* Initialize the USB */
	usb_init(); 		
	usb_find_busses(); 	
	usb_find_devices(); 	
	dev = open_device();
	if (dev == NULL) {
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr,"error: device not found!\n");
		#endif
		return -1;
	}
	#ifdef LINUX
	/* chris: e' corretta questa qui???? */
	usb_detach_kernel_driver_np(dev, 0);
	#endif
	if (usb_set_configuration(dev, 1) < 0) {
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr,"error: setting config 1 failed\n");
		#endif
		if (usb_close(dev) < 0) {
			#ifdef FLEX_USB_DEBUG
			fprintf(stderr, FLEX_USB_DEBUG_TAG);
			fprintf(stderr,"error: Unable to close device\n");
			#endif
			return -11;
		}
		return -1;
	}
	if (usb_claim_interface(dev, 0) < 0) {
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr,"error: claiming interface 0 failed\n");
		#endif
		if (usb_close(dev) < 0) {
			#ifdef FLEX_USB_DEBUG
			fprintf(stderr, FLEX_USB_DEBUG_TAG);
			fprintf(stderr,"error: Unable to close device\n");
			#endif
			return -11;
		}
		return -2;
	}
	/* Init local semaphores */
	if (pthread_mutex_init(&usb_link_mutex, 0) < 0) {
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr,"error: pthread init error\n");
		perror(FLEX_USB_DEBUG_TAG);
		#endif
		return -3;
	}
	if (pthread_mutex_init(&tx_mutex, 0) < 0) {
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr,"error: pthread init error\n");
		perror(FLEX_USB_DEBUG_TAG);
		#endif
		return -3;
	}
	if (pthread_mutex_init(&rx_mutex, 0) < 0) {
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr,"error: pthread init error\n");
		perror(FLEX_USB_DEBUG_TAG);
		#endif
		return -3;
	}
	if (pthread_cond_init(&rx_condition, 0) < 0) {
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr,"error: pthread init error\n");
		perror(FLEX_USB_DEBUG_TAG);
		#endif
		return -3;
	}
	/* init with default attributes */
	if (pthread_attr_init(&t_attr) < 0) {
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr,"error: pthread init error\n");
		perror(FLEX_USB_DEBUG_TAG);
		#endif
		return -3;
	}
	/* safe to get existing scheduling param */
	if (pthread_attr_getschedparam(&t_attr, &param) < 0) {
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr,"error: pthread sched_policy error\n");
		perror(FLEX_USB_DEBUG_TAG);
		#endif
		return -3;
	}
	/* set the priority; others are unchanged */
	if (pthread_attr_getschedpolicy(&t_attr, &policy) < 0) {
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr,"error: pthread sched_policy error\n");
		perror(FLEX_USB_DEBUG_TAG);
		#endif
		return -3;
	}
	param.sched_priority = sched_get_priority_max(policy);
	/* setting the new scheduling param */
	if (pthread_attr_setschedparam(&t_attr, &param) < 0) {
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr,"error: pthread sched_policy error\n");
		perror(FLEX_USB_DEBUG_TAG);
		#endif
		return -3;
	}
	if (pthread_create(&reader_tid, &t_attr, reader_thread, 0) < 0) {
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr,"error: pthread create error\n");
		perror(FLEX_USB_DEBUG_TAG);
		#endif
		return -3;
	}
	/* Create start connection command */
	memset(&cmd, 0x00, sizeof(struct flex_usb_packet_t)); 
	cmd.start = FLEX_USB_HEADER_START;
	cmd.type = FLEX_USB_HEADER_COMMAND;
	cmd.payload[0] = FLEX_USB_COMMAND_CONNECT >> 8;
	cmd.payload[1] = FLEX_USB_COMMAND_CONNECT & 0x00FF;
	cmd.length = 2;
	cmd.crc = checksum((uint8_t*) &cmd + 1, 
			   sizeof(struct flex_usb_packet_t) - 1);
	/* Send start connection command */
	ret = usb_claim_interface(dev, usb_interface_id);
	#ifdef FLEX_USB_DEBUG
	fprintf(stderr, FLEX_USB_DEBUG_TAG);
	fprintf(stderr,"check: usb_claim returns %d\n", ret);
	#endif
	ret = usb_bulk_write(dev, FLEX_USB_EP_OUT, (uint8_t*) &cmd, 
			     sizeof(struct flex_usb_packet_t), 5000);
	if (ret != sizeof(struct flex_usb_packet_t)) {      
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr, "error: bulk write failed, ret=%d\n", ret);
		#endif
	}
	stat_info.stat.init = 1;
	return 0;
}




DECLDIR 
int32_t flex_usb_read(uint8_t *buf, uint32_t len, uint8_t opt)
{
	uint32_t tmp_idx;

	/* chris: TODO: scegliere politica in questo caso !!! */
	if (len == 0)
		return -1;
	switch (opt) {
	case FLEX_USB_READ_BLOCK :
		/* chris: TODO: scegliere politica in questo caso !!! */
		if (len > rx_buffer_size)
			return -1;
		pthread_mutex_lock(&rx_mutex);
		while (rx_held < len) {
			#ifdef FLEX_USB_DEBUG
			fprintf(stderr, FLEX_USB_DEBUG_TAG);
			fprintf(stderr, 
				"READ RX_QUEUE: lock: rx_held<len(%d<%d)\n",
				rx_held, len);
			#endif
			pthread_cond_wait(&rx_condition, &rx_mutex);
		}
		break;
	case FLEX_USB_READ_NOBLOCK:
		pthread_mutex_lock(&rx_mutex);
		if (rx_held == 0) { /* Nothing to read */
			pthread_mutex_unlock(&rx_mutex);
			return 0;
		}
		if (len > rx_held) /* Read either length or the held bytes */
			len = rx_held;
		break;
	default:
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr, "Error, invalid read policy!");
		fflush(stderr);
		#endif
		return -2;
	}
	tmp_idx = (rx_first + len) % rx_buffer_size;
	#ifdef FLEX_USB_DEBUG
	fprintf(stderr, FLEX_USB_DEBUG_TAG);
	fprintf(stderr, "READ RX_QUEUE: rx_first=%d", rx_first);
	fprintf(stderr, " new_rx_first=%d", tmp_idx);
	fprintf(stderr, " len=%d\n", len);
	fflush(stderr);
	#endif
	/* NOTE: tmp_idx = new_first position for rx */
	if (tmp_idx > rx_first) {
		/* Message is located from rx_first to the end of the buffer. */
		memcpy(buf, rx_buffer + rx_first, len * sizeof(uint8_t));
	} else {
		/* The first part of message is stored from rx_first to the end
		    of the buffer, the remaining part goes from the of the 
		    begin buffer to tmp_idx index.  */
		memcpy(buf, rx_buffer + rx_first, 
		       (rx_buffer_size - rx_first) * sizeof(uint8_t));
		memcpy(buf + (rx_buffer_size - rx_first), rx_buffer, 
		       (tmp_idx + 1) * sizeof(uint8_t));
	}
	rx_held -= len;
	rx_first = tmp_idx;
	pthread_mutex_unlock(&rx_mutex);
	/* chris: note: if len > 2^31 a wrong negative value is returned */
	return (int32_t) len; 
}

DECLDIR 
int32_t flex_usb_write(uint8_t *buf, uint32_t len)
{
	int32_t n;
	struct flex_usb_packet_t pkt; 

	/* chris: TODO: implementare altre politiche di invio:
		  - Usiamo un parametro "opt" per indicare la politica o 
		    lo specifichiamo nella funzione init come opzione
		  - Implementare politica di ack 
		  - Implementare politica di frammentazione
		  - Implementare thread scrittore con buffer di scrittura?
		  - Implementare politica non bloccante.
	*/
	/* chris: attualmente la funzione ritorna quando ha finito la 
	          trasmissione di un solo pacchetto senza ack.
		  NOTA: non so quello che fa la libusb, quindi non so se ho
			certezza sull'avvenuta consegna del pacchetto
	*/
	if (len > FLEX_USB_PACKET_PAYLOAD_SIZE)
		return -2;
	memset(&pkt, 0x00, sizeof(struct flex_usb_packet_t));
	pkt.start = FLEX_USB_HEADER_START;
	pkt.type = FLEX_USB_HEADER_DATA;
	pkt.more = 0;
	pkt.seq = 0;
	pkt.length = len;
	memcpy(pkt.payload, buf, len); 
	pkt.crc = checksum((uint8_t*) &pkt + 1, 
			   sizeof(struct flex_usb_packet_t) - 1);
	pthread_mutex_lock(&usb_link_mutex);
	n = usb_claim_interface(dev, usb_interface_id);
	#ifdef FLEX_USB_DEBUG
	fprintf(stderr, FLEX_USB_DEBUG_TAG);
	fprintf(stderr,"check: usb_claim returns %d\n", n);
	#endif
//	do {
	#ifdef FLEX_USB_DEBUG
	fprintf(stderr, FLEX_USB_DEBUG_TAG);
	fprintf(stderr,"ATTEMPT TO WRITE\n");
	fflush(stderr);
	#endif
	n = usb_bulk_write(dev, FLEX_USB_EP_OUT, (uint8_t*) &pkt, 
			   sizeof(struct flex_usb_packet_t), 1000);
//	} while (n == -116);
	if (n < 0 ) {
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr,"error: usb_write, bulk write failed (%d)\n", n);
		#endif
	}
	pthread_mutex_unlock(&usb_link_mutex);
	return n;
}

DECLDIR int32_t flex_usb_close(void)
{
	int32_t n;
	struct flex_usb_packet_t cmd;
	
	#ifdef FLEX_USB_DEBUG
	fprintf(stderr, FLEX_USB_DEBUG_TAG);
	fprintf(stderr,"Starting close...\n");
	fflush(stderr);
	#endif
	
	if (stat_info.stat.init == 0)
		return 1;
	/* Prepare close connection command */
	memset(&cmd, 0x00, sizeof(struct flex_usb_packet_t)); 
	cmd.start = FLEX_USB_HEADER_START;
	cmd.type = FLEX_USB_HEADER_COMMAND;
	cmd.payload[0] = FLEX_USB_COMMAND_DISCONNECT >> 8;
	cmd.payload[1] = FLEX_USB_COMMAND_DISCONNECT & 0x00FF;
	cmd.length = 2;
	cmd.crc = checksum((uint8_t*) &cmd + 1, 
			   sizeof(struct flex_usb_packet_t) - 1);
	/* Send close connection command */
	pthread_mutex_lock(&usb_link_mutex);
	stat_info.stat.stop_reader = 1;	/* Stop the stop_reader thread (terminates) */
	n = usb_claim_interface(dev, usb_interface_id);
	#ifdef FLEX_USB_DEBUG
	fprintf(stderr, FLEX_USB_DEBUG_TAG);
	fprintf(stderr,"check: usb_claim returns %d\n", n);
	#endif
	n = usb_bulk_write(dev, FLEX_USB_EP_OUT, (uint8_t*) &cmd, 
			   sizeof(struct flex_usb_packet_t), 5000);
	#ifdef FLEX_USB_DEBUG
	fprintf(stderr, FLEX_USB_DEBUG_TAG);
	fprintf(stderr,"Sent close bytes %d\n", n);
	#endif
	n = usb_claim_interface(dev, usb_interface_id);
	#ifdef FLEX_USB_DEBUG
	fprintf(stderr, FLEX_USB_DEBUG_TAG);
	fprintf(stderr,"check: usb_claim returns %d\n", n);
	#endif
	/* chris: perche' faccio questo? per svuotare il buffer??*/
	usb_bulk_read(dev, FLEX_USB_EP_IN, (uint8_t*) &cmd, 
		      sizeof(struct flex_usb_packet_t), 1000);
	pthread_mutex_unlock(&usb_link_mutex);
	/* Close USB interface */
	usb_release_interface(dev, 0);
	if (usb_close(dev) < 0) {
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr,"error: Unable to close device\n");
		#endif
		return -11;
	}
	/* Free the buffers */
	if (rx_buffer != NULL) {
		free(rx_buffer);
		rx_buffer = NULL;
	}
	if (tx_buffer != NULL) {
		free(tx_buffer); 
		tx_buffer = NULL;
	}
	stat_info.stat.init = 0;
	return n;
}
