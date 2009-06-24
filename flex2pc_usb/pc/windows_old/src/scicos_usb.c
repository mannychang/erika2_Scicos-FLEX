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
/* chris: forse serve solo quando ho il concetto di canale usato da scicos
	  Conviene dropparlo nella libreria generale e metterlo solo in quella
	  di scicos.
*/
#include "channels.h"	
/* chris: TODO: cambiare nome! usare flex_usb o simile
	  Questa libreria deve diventare una libreria generica che non usa solo
	  Scicos, poi (se necessario) ne creiamo un'altra che 
	  viene utilizzata soltanto da scicos e implementa quello che gli serve
 */
#include "scicos_usb.h" 

/* Debugging*/
#define FLEX_USB_DEBUG
#define FLEX_USB_DEBUG_TAG "flex_usb DEBUG: "
/* The device's vendor and product id */
#define FLEX_USB_VID 0x04d8
#define FLEX_USB_PID 0x0033
/* The device's endpoints */
#define FLEX_USB_EP_IN 0x81
#define FLEX_USB_EP_OUT 0x01
#define BUF_SIZE 64
/* chris: FIXME: queste costanti vengono dal file channels.c e sono hardcoded
	  in entramby i file. SISTEMARE QUESTA COSA!!!!
 */
#define NUM_CHANNEL 256
#define QUEUE_SIZE 256

//#define min(a,b) (((a)<(b))?(a):(b))
#define _FLEX_USB_MIN(a,b) (((a)<(b))?(a):(b))

/* chris: deve essere globale sto tid? */
pthread_t usb_tid; //tid of usb_thread	

static int initialized=0;
static char stop_usb_thread = 0;
static pthread_mutex_t usb_bus_mutex;
static usb_dev_handle *dev = NULL; /* the device handle */

/* 
 * compute a checksum
 */
static int checksum(unsigned char *buf)
{
	unsigned char len, sum, *pt;
	
	len = buf[1] + 2;
	sum = 0;
	pt = buf + 1;
	while (len--)
		sum += *(pt++);
	if(sum != *pt)
		return 0;
	return 1;
}

/*
 * Thread for reading from USB
 */
/* chris: TODO: riscrivere quasi da zero, vedere cosa serve mantenere! */
static void *usb_thread(void *par)
{
	unsigned char part_msg[300];
	unsigned int part_msg_len=0;
	unsigned char buf[BUF_SIZE];
	unsigned char ch, *curr_pkt;
	int read_bytes, i, copied_bytes, consumed, len;
	unsigned short int pkt_bytes;

	/* USB Thread master loop: process data from the USB link */
	while (!stop_usb_thread) {	

		pthread_mutex_lock(&usb_bus_mutex);
		/* chris: TODO: controllare questa read! sizeof è sicuro? */
		read_bytes = usb_bulk_read(dev, FLEX_USB_EP_IN, (char *) buf, 
					   sizeof(buf), 2000);
		if (read_bytes < 1) {
			#ifdef FLEX_USB_DEBUG
			/* chris: rimuovo if sul modulo 64, serve a qualcosa?*/
			//if ((read_bytes % 64) != 0) {
				fprintf(stderr, FLEX_USB_DEBUG_TAG);
				fprintf(stderr,
					"error: bulk read failed, %d bytes\n", 
					read_bytes);
			//}
			#endif
			pthread_mutex_unlock(&usb_bus_mutex);
			continue;
		}
		/* chris: qui stabilisco che mando pacchetti da 64? */
		/* chris: essendo buffer da 64, che senso ha fare il modulo?*/
		if ((read_bytes % 64) != 0){
			#ifdef FLEX_USB_DEBUG
			fprintf(stderr, FLEX_USB_DEBUG_TAG);
			fprintf(stderr, "error: bulk read failed, %d bytes\n", 
				read_bytes);
			#endif
			pthread_mutex_unlock(&usb_bus_mutex);
			continue;
		}
		pthread_mutex_unlock(&usb_bus_mutex);

		/* chris: questo sembra essere il pezzo in cui parso i 
			  pacchetti provenienti dal pic18, posso cambiare 
			  completamente questa parte? 
		*/
		/* chris: TODO: mi pare che questo ciclo avvenga una sola volta,
			  essendo che buf ha proprio dimensione pari a 64!!!!*/
		/* Parse all packets read */
		curr_pkt = buf;
		for(i = 0; i < read_bytes; i += 64, curr_pkt = buf + i) {
			consumed = 0;
			/* chris: sintassi agghiacciante! */
			pkt_bytes = *(unsigned short int *)&curr_pkt[62];
			if (pkt_bytes > 62)
				pkt_bytes = 62;

			//in case, concatenate to a previous message
			if (part_msg_len > 0) {
				if (part_msg_len == 1) { //only header
					part_msg[1] = curr_pkt[consumed++];
					part_msg_len++;
				}
				copied_bytes = 
					_FLEX_USB_MIN(pkt_bytes - consumed, 
						      5 + part_msg[1] - 
						      part_msg_len);
				memcpy(part_msg + part_msg_len, 
				       curr_pkt + consumed,
				       copied_bytes);
				consumed += copied_bytes;
				if (5 + part_msg[1] == 
				    part_msg_len + copied_bytes) {
				//the message has been completely reconstructed
					if (checksum(part_msg) == 1) {
						//checksum OK
						len = part_msg[1];
						ch = part_msg[len + 2];
						insert_message(part_msg + 1,
							       len + 1, ch);
					} else {
						#ifdef FLEX_USB_DEBUG
						fprintf(stderr, 
							FLEX_USB_DEBUG_TAG);
						fprintf(stderr,
							"message corrupted\n");
						#endif
					}
					part_msg_len=0;
				} else {
					part_msg_len += copied_bytes;
				}
				//if read all bytes go to next packet
				if(consumed == pkt_bytes) 
					continue;
			}
	
			/* Parse the single packet */
			while (1) {
				//search for header character 0xAA:
				//it should be in pole position
				while (consumed < pkt_bytes && 
				       curr_pkt[consumed] != 0xAA) {
					#ifdef FLEX_USB_DEBUG
					fprintf(stderr, FLEX_USB_DEBUG_TAG);
					fprintf(stderr,"not in sync, pos=%d\n",
						consumed);
					#endif
					consumed++;
				}
				//if no more bytes to read then get next packet
				if(consumed >= pkt_bytes){
					part_msg_len=0;
					break;
				}
				// if the message is incomplete
				len = pkt_bytes - consumed;
				if (len <= 2 || 
				    curr_pkt[consumed + 1] + 5 > len) {
					memcpy(part_msg, curr_pkt + consumed, 
					       len);
					part_msg_len = len;
					break;
				}
				if (checksum(curr_pkt + consumed) == 1) {
					len = curr_pkt[consumed + 1];
					ch = curr_pkt[consumed + len + 2];
					insert_message(curr_pkt + consumed + 1,
						       len+1, ch);
					consumed += curr_pkt[consumed + 1] + 5;
				} else {
					#ifdef FLEX_USB_DEBUG
					fprintf(stderr, FLEX_USB_DEBUG_TAG);
					fprintf(stderr, "message corrupted\n");
					#endif
					consumed += 1;
					/* chris: se siamo qui significa che
					          nel pacchetto corrente ho gia'
						  trovato un header AA ma che la
						  checksum ha fallito, ha senso
						  continuare a cercare un'altro
						  header in questo pacchetto?
					*/
				}
			} /* End Of single packet parsing*/
		} /* End Of all packets parsing */
	} /* End Of master loop */
}


static usb_dev_handle *open_dev(void)
{
	struct usb_bus *bus;
	struct usb_device *dev;
	
	#ifdef FLEX_USB_DEBUG
	fprintf(stderr, FLEX_USB_DEBUG_TAG);
	fprintf(stderr, "Scan devices... \n");
	#endif

	/* chris: TODO usare NULL nella condizione dei for!*/
	bus = usb_get_busses();
	for (bus = usb_get_busses(); bus; bus = bus->next) {
		for (dev = bus->devices; dev; dev = dev->next) {
			#ifdef FLEX_USB_DEBUG
			fprintf(stderr, FLEX_USB_DEBUG_TAG);
			fprintf(stderr, "Device: vendor=%x product=%x\n", 
				dev->descriptor.idVendor, 
				dev->descriptor.idProduct);
			#endif
			if (dev->descriptor.idVendor == FLEX_USB_VID && 
			    dev->descriptor.idProduct == FLEX_USB_PID) 
					return usb_open(dev);
		}
	}
	return NULL;
}


DECLDIR int flex_pc_usb_init()
{
	pthread_attr_t tattr;
	int ret;
	struct sched_param param;
	int policy;
	char tmp=2;

	stop_usb_thread = 0;
	if(initialized)
		return 0;
    
	usb_init(); /* initialize the library */
	/* chris: valore di ritorno ignorato! */
	ret = usb_find_busses(); /* find all busses */
	ret = usb_find_devices(); /* find all connected devices */

	if(!(dev = open_dev()))
	{
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr,"error: device not found!\n");
		#endif
		return 1;
	}

#ifdef LINUX
	usb_detach_kernel_driver_np(dev,0);
#endif
	

	if(usb_set_configuration(dev, 1) < 0)
	{
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr,"error: setting config 1 failed\n");
		#endif
		usb_close(dev);
		return -1;
	}

	if(usb_claim_interface(dev, 0) < 0)
	{
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr,"error: claiming interface 0 failed\n");
		#endif
		usb_close(dev);
		return -2;
	}

	init_channels_queue();

	pthread_mutex_init(&usb_bus_mutex,0);

	/* chris: FIXME; i valori di ritorno sono ignorati, testarli!!! */
	/* initialized with default attributes */
	ret = pthread_attr_init (&tattr);
	/* safe to get existing scheduling param */
	ret = pthread_attr_getschedparam (&tattr, &param);
	/* set the priority; others are unchanged */
	ret = pthread_attr_getschedpolicy(&tattr, &policy);
	param.sched_priority = sched_get_priority_max(policy);
  
	/* setting the new scheduling param */
	ret = pthread_attr_setschedparam (&tattr, &param);
	pthread_create(&usb_tid, &tattr, usb_thread, 0);
	
	/* chris: COSA? */
	/****** DA CANCELLARE BEGIN************/
	

	if(usb_bulk_write(dev, FLEX_USB_EP_OUT, &tmp, sizeof(tmp), 5000) != sizeof(tmp))
	{      
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr, "error: bulk write failed\n");
		#endif
	}
	/****** DA CANCELLARE END************/


	initialized=1;
	return 0;
}





DECLDIR int flex_pc_usb_read(char *buf, int len, unsigned char channel, char readlast)
{
	int retval=get_message(buf, len, channel, readlast);
	empty_channel_queue(channel);
	return retval;
}

DECLDIR int flex_pc_usb_write(char *buf, int len, unsigned char channel)
{
	int byteswritten;
	pthread_mutex_lock(&usb_bus_mutex);

	if((byteswritten=usb_bulk_write(dev, FLEX_USB_EP_OUT, buf, len, 1000)) < 0 ){
		#ifdef FLEX_USB_DEBUG
		fprintf(stderr, FLEX_USB_DEBUG_TAG);
		fprintf(stderr,"error: bulk write failed\n");
		#endif
		return -1;
	}

	pthread_mutex_unlock(&usb_bus_mutex);
	return byteswritten;
}

DECLDIR int flex_pc_usb_close(void){
	
	int closed;
	char buf[64]={5};
	stop_usb_thread = 1;
	closed = flex_pc_usb_write(buf, 1, 0);
	pthread_mutex_lock(&usb_bus_mutex);

	usb_bulk_read(dev, FLEX_USB_EP_IN, buf, 64, 1000);

	pthread_mutex_unlock(&usb_bus_mutex);

	usb_release_interface(dev, 0);

	usb_close(dev);
	
	return closed;
}
