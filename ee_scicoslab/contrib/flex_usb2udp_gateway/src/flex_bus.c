#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "flex_bus.h"
#include "flex_usb.h"

//#define FLEX_BUS_DEBUG
#define FLEX_BUS_DEBUG_TAG "flex_bus DEBUG: "

struct flex_bus_channel_t {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	uint16_t size;
	struct flex_bus_message_t *queue;
	int32_t first, last; // first == last => empty queue
};

union flex_bus_stat_t {
	struct {
		unsigned init : 1;
		unsigned stop_usb_reader : 1;
		//unsigned stop_rs232_reader : 1;
	} stat;
	uint8_t stats;
};

static union flex_bus_stat_t stat_info = {0, 0};
static struct flex_bus_channel_t *channels = NULL;
static pthread_t usb_reader_tid; 
// static pthread_t rs232_reader_tid; // TODO

static int32_t queue_insert(uint8_t *buf, uint16_t len, uint16_t ch)
{
	/* chris: current policy is truncate the message, FIXME!!!! */
	if (len > FLEX_BUS_MESSAGE_SIZE)
		len = FLEX_BUS_MESSAGE_SIZE;
	/* chris: current policy is to avoid null packet to be sent! */
	if (len == 0)
		return -1;
	pthread_mutex_lock(&(channels[ch].mutex));
	memcpy(channels[ch].queue[channels[ch].last].data, buf, len);
	channels[ch].queue[channels[ch].last].length = len;
	channels[ch].last = (channels[ch].last + 1) % channels[ch].size;
	/* chris: perche' faccio questo if ? */
	if (channels[ch].first == channels[ch].last)
		channels[ch].first = (channels[ch].first + 1) % 
				     channels[ch].size;
	/* chris: non mi convince, perche' lo sblocco solo in questo caso? */
	if ((channels[ch].first + 1) % channels[ch].size == channels[ch].last)
		pthread_cond_signal(&(channels[ch].cond));
	pthread_mutex_unlock(&(channels[ch].mutex));
	return 0;
}

static int32_t queue_get(uint8_t *buf, uint16_t len, uint16_t ch, uint8_t lifo)
{
	int32_t retv= 0;
	struct flex_bus_message_t *msg;

	pthread_mutex_lock(&(channels[ch].mutex));
	while (channels[ch].first == channels[ch].last) {
		#ifdef FLEX_BUS_DEBUG
		fprintf(stderr, FLEX_BUS_DEBUG_TAG);
		fprintf(stderr, "QUEUE %d get: LOCK (empty)\n", ch);
		#endif
		pthread_cond_wait(&(channels[ch].cond), &(channels[ch].mutex));
	}
	if (lifo) { //extract last element
		/* chris : che vuol dire questa cosa???? */
		if (--channels[ch].last < 0)
			channels[ch].last += channels[ch].size;
		msg = &(channels[ch].queue[channels[ch].last]);
		retv = len < msg->length ? len : msg->length;
		memcpy(buf, msg->data, retv);
		
	} else { //extract first element
		msg = &(channels[ch].queue[channels[ch].first]);
		retv = len < msg->length ? len : msg->length;
		memcpy(buf, msg->data, retv);
		channels[ch].first = (channels[ch].first + 1) % 
				     channels[ch].size;
	}
	pthread_mutex_unlock(&(channels[ch].mutex));
	return retv;
}

///* chris: esiste una condizione di errore nel ritorno o posso usare void?*/
//int empty_channel_queue(unsigned char channel)
//{
//	pthread_mutex_lock(&channels[channel].m);
//	channels[channel].first=channels[channel].last=0;
//	pthread_mutex_unlock(&channels[channel].m);
//	return 0;
//}

static int32_t queue_init(uint16_t n_channels, uint16_t queues_size)
{
	uint16_t i;

	/* check wether to use the dafult values */
	if (n_channels == 0)
		n_channels = FLEX_BUS_DAFAULT_CHANNEL_NUMBER;
	if (queues_size == 0)
		queues_size = FLEX_BUS_DAFAULT_QUEUES_SIZE;

	if (channels != NULL)
		free(channels);
	channels = (struct flex_bus_channel_t *) malloc(n_channels * 
					sizeof(struct flex_bus_channel_t));
	if (channels == NULL) {
		#ifdef FLEX_BUS_DEBUG
		fprintf(stderr, FLEX_BUS_DEBUG_TAG);
		fprintf(stderr, "Error, unable to allocate channels structs.");
		#endif
		return -5;
	}
	for (i = 0; i < n_channels; i++) {
		channels[i].queue = (struct flex_bus_message_t *) 
					malloc(queues_size * 
					     sizeof(struct flex_bus_message_t));
		if (channels[i].queue == NULL) {
			#ifdef FLEX_BUS_DEBUG
			fprintf(stderr, FLEX_BUS_DEBUG_TAG);
			fprintf(stderr, "Error, allocate of channels queues");
			#endif
			return -5;
		}
		channels[i].size = queues_size;
		channels[i].first = 0;
		channels[i].last = 0;
		if (pthread_mutex_init(&(channels[i].mutex) ,0) < 0) {
			#ifdef FLEX_BUS_DEBUG
			fprintf(stderr, FLEX_BUS_DEBUG_TAG);
			fprintf(stderr, "Error, pthread init");
			#endif
			return -55;
		}
		if (pthread_cond_init(&(channels[i].cond), 0) < 0) {
			#ifdef FLEX_BUS_DEBUG
			fprintf(stderr, FLEX_BUS_DEBUG_TAG);
			fprintf(stderr, "Error, pthread init");
			#endif
			return -55;
		}
	}

	return 0;
}


static void *usb_reader_thread(void *ignored_param)
{
	struct flex_bus_packet_t pkt;
	int32_t retv;

	/* USB Reader Thread master loop: process data from flex_usb library */
	while (!stat_info.stat.stop_usb_reader)  {
		#ifdef FLEX_BUS_DEBUG
		fprintf(stderr, FLEX_BUS_DEBUG_TAG);
		fprintf(stderr, "QUEUE reader runs\n");
		#endif
		retv = 	flex_usb_read((uint8_t *) &pkt, 
				      sizeof(struct flex_bus_packet_t), 
				      FLEX_USB_READ_BLOCK);
		if (retv != sizeof(struct flex_bus_packet_t)) {
			#ifdef FLEX_BUS_DEBUG
			fprintf(stderr, FLEX_BUS_DEBUG_TAG);
			fprintf(stderr, "error: read few bytes (%d)\n", retv);
			fflush(stderr);
			#endif
			continue;
		}
		retv = queue_insert(pkt.payload.data, pkt.payload.length, 
				    pkt.channel);
		#ifdef FLEX_BUS_DEBUG
		fprintf(stderr, FLEX_BUS_DEBUG_TAG);
		fprintf(stderr, "QUEUE %d : insert\n", pkt.channel);
		#endif
		if (retv < 0) {
			#ifdef FLEX_BUS_DEBUG
			fprintf(stderr, FLEX_BUS_DEBUG_TAG);
			fprintf(stderr, "error: queue insert error\n");
			fflush(stderr);
			#endif
			continue;
		}
	} /* End of master loop */
	return NULL;
}


DECLDIR
int32_t flex_bus_init(void) 
{
	int32_t retv;
	int policy;
	struct sched_param param;
	pthread_attr_t t_attr;

	if (stat_info.stat.init)
		return 1;
	/* Init channels queues */
	retv = queue_init(0, 0);
	if (retv < 0) {
		#ifdef FLEX_BUS_DEBUG
		fprintf(stderr, FLEX_BUS_DEBUG_TAG);
		fprintf(stderr, "Error, queue_init fails.");
		#endif
		return retv;
	}
	/* Initilize the flex_usb library with default options */
	retv = flex_usb_init(sizeof(struct flex_bus_packet_t) * 10, 
			     sizeof(struct flex_bus_packet_t) * 10, 0);
	if (retv < 0) {
		#ifdef FLEX_BUS_DEBUG
		fprintf(stderr, FLEX_BUS_DEBUG_TAG);
		fprintf(stderr, "Error, flex_usb_init fails.");
		#endif
		return retv;
	}
	/* init with default attributes */
	if (pthread_attr_init(&t_attr) < 0) {
		#ifdef FLEX_BUS_DEBUG
		fprintf(stderr, FLEX_BUS_DEBUG_TAG);
		fprintf(stderr,"error: pthread init error\n");
		perror(FLEX_BUS_DEBUG_TAG);
		#endif
		return -3;
	}
	/* safe to get existing scheduling param */
	if (pthread_attr_getschedparam(&t_attr, &param) < 0) {
		#ifdef FLEX_BUS_DEBUG
		fprintf(stderr, FLEX_BUS_DEBUG_TAG);
		fprintf(stderr,"error: pthread sched_policy error\n");
		perror(FLEX_BUS_DEBUG_TAG);
		#endif
		return -3;
	}
	/* set the priority; others are unchanged */
	if (pthread_attr_getschedpolicy(&t_attr, &policy) < 0) {
		#ifdef FLEX_BUS_DEBUG
		fprintf(stderr, FLEX_BUS_DEBUG_TAG);
		fprintf(stderr,"error: pthread sched_policy error\n");
		perror(FLEX_BUS_DEBUG_TAG);
		#endif
		return -3;
	}
	//retv = sched_get_priority_max(policy) - 1;
	//param.sched_priority = retv > sched_get_priority_min(policy) ?
	//		       retv : sched_get_priority_min(policy);
	param.sched_priority = sched_get_priority_max(policy); 
//printf("\n\nPROVA 0000000000 -  %d\n\n", param.sched_priority);
//fflush(stdout);
	/* setting the new scheduling param */
	if (pthread_attr_setschedparam(&t_attr, &param) < 0) {
		#ifdef FLEX_BUS_DEBUG
		fprintf(stderr, FLEX_BUS_DEBUG_TAG);
		fprintf(stderr,"error: pthread sched_policy error\n");
		perror(FLEX_BUS_DEBUG_TAG);
		#endif
		return -3;
	}
	stat_info.stat.stop_usb_reader = 0;
	if (pthread_create(&usb_reader_tid, &t_attr, usb_reader_thread, 0)<0) {
		#ifdef FLEX_BUS_DEBUG
		fprintf(stderr, FLEX_BUS_DEBUG_TAG);
		fprintf(stderr,"error: pthread create error\n");
		perror(FLEX_BUS_DEBUG_TAG);
		#endif
		return -3;
	}
	stat_info.stat.init = 1;
	return 0;
}

DECLDIR
int32_t flex_bus_read(uint8_t *buf, uint16_t len, uint16_t ch)
{
	/* chris: TODO: do more than this!!! */

	/* chris: current policy is to avoid null packet to be sent! */
	if (len == 0)
		return -1;
	/* chris: current policy is truncate the message, FIXME!!!! */
	if (len > FLEX_BUS_MESSAGE_SIZE)
		len = FLEX_BUS_MESSAGE_SIZE;
	return queue_get(buf, len, ch, 0);
}

DECLDIR
int32_t flex_bus_write(uint8_t *buf, uint16_t len, uint16_t ch)
{
	/* chris: TODO: do more than this!!! */
	struct flex_bus_packet_t pkt;

	/* chris: current policy is to avoid null packet to be sent! */
	if (len == 0)
		return -1;
	/* chris: current policy is truncate the message, FIXME!!!! */
	if (len > FLEX_BUS_MESSAGE_SIZE)
		len = FLEX_BUS_MESSAGE_SIZE;
	pkt.channel = ch;
	pkt.payload.length = len;
	memcpy(pkt.payload.data, buf, len);
	return flex_usb_write((uint8_t*)&pkt, sizeof(struct flex_bus_packet_t));
} 

DECLDIR
int32_t flex_bus_close(void) 
{
	int32_t retv;
	
	if (stat_info.stat.init == 0)
		return 1;
	retv = flex_usb_close();
	if (retv < 0)
		return retv;
	/* Stop the stop_reader thread (terminates) */
	stat_info.stat.stop_usb_reader = 1;	
	stat_info.stat.init = 0;
	return 1;
}
