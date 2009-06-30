#ifdef WIN32 
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#endif
#define MAX_CHANNELS 15

struct UDPStruct{
	SOCKET sd;
	struct sockaddr_in addr;
};



int get_error(void);
void init(void);
void init_addr(scicos_block *block,struct UDPStruct *udpS);
void close_socket(sd);
