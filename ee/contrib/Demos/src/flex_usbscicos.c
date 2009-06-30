/** 
* @file flex_usbscicos.c
* @brief Flex to Scicos communication via usb
* @author Christian Nastasi
* @version 0.1
* @date 2009-06-25
*/
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "flex_usbscicos.h"
#include "flex_usb.h"

#define SCICOS_USB_CHANNELS 15

//#define SINGLE_THREAD
//#define NO_WRITE_THREAD
//#define NO_READ_THREAD

#if (defined NO_WRITE_THREAD) || (defined NO_READ_THREAD)
#ifdef SINGLE_THREAD
#undef SINGLE_THREAD
#endif 
#endif

static char is_initialized = 0;
static char pause_writer = 1;
static char pause_reader = 1;
static int write_block_number = 0;
static int read_block_number = 0;
static char stop_usb_process_io = 1;
static pthread_t usb_process_io_tid; 
static pthread_t usb_process_reader_tid; 
static pthread_t usb_process_writer_tid; 
static pthread_mutex_t tx_buffer_mutex;
static pthread_mutex_t rx_buffer_mutex;
static float tx_buffer_scicos[SCICOS_USB_CHANNELS];
static float rx_buffer_scicos[SCICOS_USB_CHANNELS];
static float buffer_scicos_shadow[SCICOS_USB_CHANNELS];

#ifdef SINGLE_THREAD
static void *usb_process_io_thread(void *ignored_param)
{
	/* USB Reader Thread master loop: process data from flex_usb library */
	while (!stop_usb_process_io)  {
		if (!pause_reader) {
			pthread_mutex_lock(&rx_buffer_mutex);
			memcpy((uint8_t *) rx_buffer_scicos, 
				(uint8_t *) buffer_scicos_shadow, 60);
			pthread_mutex_unlock(&rx_buffer_mutex);
			/* sizeof(float) x SCICOS_USB_CHANNELS = 4 x 15 = 60*/
			//flex_usb_read((uint8_t *) rx_buffer_scicos, 60_shadow, 
			flex_usb_read((uint8_t *) buffer_scicos_shadow, 60, 
					FLEX_USB_READ_BLOCK);		
			//pthread_mutex_unlock(&rx_buffer_mutex);
		}		
		if (!pause_writer) {
			///*
			static int count = 0;
			if (count++ < 10) 
				continue;
			else 
				count = 0;
			//*/
			/*FIXME: temporary solution. Won't work all the time, 
			         study upon the problem!! */
			pthread_mutex_lock(&tx_buffer_mutex);	
			memcpy((uint8_t *) buffer_scicos_shadow, 
				(uint8_t *) tx_buffer_scicos, 60);
			pthread_mutex_unlock(&tx_buffer_mutex);
			/* sizeof(float) x SCICOS_USB_CHANNELS = 4 x 15 = 60*/
			flex_usb_write((uint8_t *) buffer_scicos_shadow, 60);
			//pthread_mutex_unlock(&tx_buffer_mutex);
		}
	} /* End of master loop */
	return NULL;
}
#endif /* SINGLE_THREAD */

#ifndef NO_READ_THREAD
static void *usb_process_reader_thread(void *ignored_param)
{	
	/* USB Reader Thread master loop: process data from flex_usb library */
	while (!stop_usb_process_io)  {
		if (!pause_reader) {
			pthread_mutex_lock(&rx_buffer_mutex);
			/* sizeof(float) x SCICOS_USB_CHANNELS = 4 x 15 = 60*/	
			flex_usb_read((uint8_t *) rx_buffer_scicos, 60, 
				FLEX_USB_READ_BLOCK);		
			pthread_mutex_unlock(&rx_buffer_mutex);
		}		
	} /* End of master loop */
	return NULL;
}
#endif /* NO_READ_THREAD */ 

#ifndef NO_WRITE_THREAD
static void *usb_process_writer_thread(void *ignored_param)
{	
	/* USB Writer Thread master loop: process data from flex_usb library */
	while (!stop_usb_process_io)  {
		if (!pause_writer) {
			/* TODO: need to  find a non-blocking 
				 version of the write function */
			pthread_mutex_lock(&tx_buffer_mutex);
			memcpy((uint8_t *) buffer_scicos_shadow, 
				(uint8_t *) tx_buffer_scicos, 60);
			pthread_mutex_unlock(&tx_buffer_mutex);
			/* sizeof(float) x SCICOS_USB_CHANNELS = 4 x 15 = 60*/
			flex_usb_write((uint8_t *) buffer_scicos_shadow, 60);
			//pthread_mutex_unlock(&tx_buffer_mutex);
		}
	} /* End of master loop */
	return NULL;
}
#endif /* NO_WRITE_THREAD */

DECLDIR
int32_t flex_usbscicos_init(unsigned char block_type) 
{
	int32_t retv;
	int policy;
	struct sched_param param;
	pthread_attr_t t_attr;

	
	if (is_initialized) {
		if (block_type == 0)
			write_block_number++;
		else if (block_type == 1)
			read_block_number++;
		return 1;
	}
	/* Initilize the flex_usb library with default options */	
	/* sizeof(float) x SCICOS_USB_CHANNELS = 4 x 15 = 60
	    Tx/Rx buffer size = 10x60 */		
	retv = flex_usb_init(60, 60, 0);
	if (retv < 0) 
		return retv;
	/* Init local semaphores */
	if (pthread_mutex_init(&tx_buffer_mutex, 0) < 0) 
		return -3;
	if (pthread_mutex_init(&rx_buffer_mutex, 0) < 0) 
		return -3;
	/* init with default attributes */
	if (pthread_attr_init(&t_attr) < 0) 
		return -3;
	/* safe to get existing scheduling param */
	if (pthread_attr_getschedparam(&t_attr, &param) < 0) 		
		return -3;
	/* set the priority; others are unchanged */
	if (pthread_attr_getschedpolicy(&t_attr, &policy) < 0) 
		return -3;
	param.sched_priority = sched_get_priority_max(policy); 
	/* setting the new scheduling param */
	if (pthread_attr_setschedparam(&t_attr, &param) < 0) 
		return -3;
	stop_usb_process_io = 0;
	pause_writer = 1;
	pause_reader = 1;
	#ifdef  SINGLE_THREAD
	if (pthread_create(&usb_process_io_tid, &t_attr, 
			   usb_process_io_thread, 0) < 0)
		return -3;
	#endif
	#ifndef NO_READ_THREAD
	if (pthread_create(&usb_process_reader_tid, &t_attr, 
			   usb_process_reader_thread, 0) < 0)
		return -3;
	#endif
	#ifndef NO_WRITE_THREAD
/*
	if (pthread_create(&usb_process_writer_tid, &t_attr, 
			   usb_process_writer_thread, 0) < 0)
		return -3;
*/
	#endif
	is_initialized = 1;
	if (block_type == 0)
		write_block_number++;
	else if (block_type == 1)
		read_block_number++;
	return 0;
}

#ifdef NO_READ_THREAD
DECLDIR
float flex_usbscicos_read(uint16_t ch)
{
	static expected_channel = 0;
	float val = 0;
	
	if (ch < SCICOS_USB_CHANNELS && is_initialized) {
		// sizeof(float) x SCICOS_USB_CHANNELS = 4 x 15 = 60 
		if (expected_channel == 0) 			
			flex_usb_read((uint8_t *) rx_buffer_scicos, 60, 
				      FLEX_USB_READ_NOBLOCK);		
		expected_channel = (expected_channel + 1) % read_block_number;
		val = rx_buffer_scicos[ch];
	}
	return val;
}
#else
DECLDIR
float flex_usbscicos_read(uint16_t ch)
{
	float val = 0;
	
	if (ch < SCICOS_USB_CHANNELS && is_initialized) {
		if (pause_reader)
			pause_reader = 0;
		pthread_mutex_lock(&rx_buffer_mutex);
		val = rx_buffer_scicos[ch];
		pthread_mutex_unlock(&rx_buffer_mutex);
	}
	return val;
}
#endif /* NO_READ_THREAD */

#ifdef NO_WRITE_THREAD
DECLDIR
void flex_usbscicos_write(uint16_t ch, float val)
{
/*
	static expected_channel = 0;

	if (ch < SCICOS_USB_CHANNELS && is_initialized) {		
		tx_buffer_scicos[ch] = val;		
		if (++expected_channel == write_block_number) {			
			// sizeof(float) x SCICOS_USB_CHANNELS = 4 x 15 = 60
			flex_usb_write((uint8_t *) tx_buffer_scicos, 60);		
			expected_channel = 0;
		}
	}
*/
} 
#else
DECLDIR
void flex_usbscicos_write(uint16_t ch, float val)
{
/*
	if (ch < SCICOS_USB_CHANNELS && is_initialized) {
		if (pause_writer)
			pause_writer = 0;
		pthread_mutex_lock(&tx_buffer_mutex);
		tx_buffer_scicos[ch] = val;
		pthread_mutex_unlock(&tx_buffer_mutex);
	}
*/
} 
#endif

DECLDIR
int32_t flex_usbscicos_close(void) 
{
	int32_t retv;
	
	if (is_initialized == 0)
		return 1;
	retv = flex_usb_close();
	if (retv < 0)
		return retv;
	/* Stop the thread(s) (terminates) */
	stop_usb_process_io = 1;		
	is_initialized = 0;
	write_block_number = 0;
	return 1;
}
