
/*
 * initialize channels data structure
 */
int init_channels_queue();


/*
 * insert a message in a channel queue
 * return: 0 if ok
 */
int insert_message(char *buf, int len, unsigned char channel);

/*
 * get a message from a channel queue: if readlast != 0 read the last message in queue
 */
int get_message(char *buf, int len, unsigned char channel, char readlast);

/*
 * empty a channel queue
 */
int empty_channel_queue(unsigned char channel);

