#include <pthread.h>
#include <string.h>

#define MAX_MESSAGE_LEN 256
#define QUEUE_SIZE 10

/* chris: FIXME: non usare min in questo modo, mettere un nome maiuscole e con
	  suffisso , tipo CHANNEL_MIN
*/
#define min(a,b) (((a)<(b))?(a):(b))

typedef struct {
	char mess[MAX_MESSAGE_LEN];
	int length;
}ScicosMessage;

typedef struct {
	pthread_mutex_t m;
	pthread_cond_t cond;
	ScicosMessage queue[QUEUE_SIZE];
	int first, last; // first == last => empty queue
}Channel;

Channel channels[256];
static initialized=0;

int insert_message(char *buf, int len, unsigned char channel)
{
	if(len>MAX_MESSAGE_LEN)
		len=MAX_MESSAGE_LEN;

	pthread_mutex_lock(&channels[channel].m);
	memcpy(channels[channel].queue[channels[channel].last].mess,buf,len);
	channels[channel].queue[channels[channel].last].length=len;

	channels[channel].last=(channels[channel].last+1) % QUEUE_SIZE;
	
	if(channels[channel].first == channels[channel].last)
		channels[channel].first = (channels[channel].first + 1)% QUEUE_SIZE;
		
	/* chris: non mi convince, perche' lo sblocco solo in questo caso?*/
	if((channels[channel].first+1)%QUEUE_SIZE ==
		channels[channel].last)
		pthread_cond_signal(&channels[channel].cond);

	pthread_mutex_unlock(&channels[channel].m);
	return 0;
}

int get_message(char *buf, int len, unsigned char channel, char readlast)
{
	int retval=0;
	pthread_mutex_lock(&channels[channel].m);
	while(channels[channel].first == channels[channel].last){
		pthread_cond_wait(&channels[channel].cond, 
			&channels[channel].m);
	}
	if(readlast){ //extract last element
		ScicosMessage *mess;
		if(--channels[channel].last<0)
			channels[channel].last+=QUEUE_SIZE;

		mess=&channels[channel].queue[channels[channel].last];
		retval=min(len,mess->length);
		memcpy(buf,
			mess->mess,
			retval);
		
	}else{ //extract first element
		ScicosMessage *mess=&channels[channel].queue[channels[channel].first];
		retval=min(len,mess->length);
		memcpy(buf,
			mess->mess,
			retval);
		
		channels[channel].first=(channels[channel].first+1)%QUEUE_SIZE;
	}
	pthread_mutex_unlock(&channels[channel].m);
	return retval;
}

/* chris: esiste una condizione di errore nel ritorno o posso usare void?*/
int empty_channel_queue(unsigned char channel)
{
	pthread_mutex_lock(&channels[channel].m);
	channels[channel].first=channels[channel].last=0;
	pthread_mutex_unlock(&channels[channel].m);
	return 0;
}

/* chris: esiste una condizione di errore nel ritorno o posso usare void?*/
int init_channels_queue()
{
	int i;
	for(i=0; i<256; i++){
		pthread_cond_init(&channels[i].cond,0);
		pthread_mutex_init(&channels[i].m,0);
		channels[i].first=channels[i].last=0;
	}
	initialized=1;

	return 0;
}

