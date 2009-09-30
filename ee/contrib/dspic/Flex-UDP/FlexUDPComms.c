#include "scicos/scicos_block4.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>


#define u(i,j) ((double *)block->inptr[i])[j]
#define y(i,j) ((double *)block->outptr[i])[j]
#define ipar(i) (block->ipar[i])
#define sock_str_ptr ((struct UDPStruct *)(*(block->work)))

void udp_receiver(scicos_block *block,int flag){
    int i,ret;
    int port;
    int err;
    float tmp[MAX_CHANNELS];
    switch (flag){

        //**Init
         case 4:
            for (i=0;i<MAX_CHANNELS;i++)
                y(0,i)=0.0;
            init();
            sock_str_ptr = (struct UDPStruct *)malloc(sizeof(struct UDPStruct));
            port =ipar(4);
            sock_str_ptr->sd = socket(AF_INET, SOCK_DGRAM,0);
            if (sock_str_ptr->sd < 0)
                ; /*ERROR!!!*/
            sock_str_ptr->addr.sin_family = AF_INET;
            sock_str_ptr->addr.sin_port = htons(port);
            /*init_addr(block,sock_str_ptr);*/
            sock_str_ptr->addr.sin_addr.S_un.S_un_b.s_b1=ipar(0);
            sock_str_ptr->addr.sin_addr.S_un.S_un_b.s_b2=ipar(1);
            sock_str_ptr->addr.sin_addr.S_un.S_un_b.s_b3=ipar(2);
            sock_str_ptr->addr.sin_addr.S_un.S_un_b.s_b4=ipar(3);
            if (bind(
                sock_str_ptr->sd,
                (struct sockaddr *) &(sock_str_ptr->addr),
                sizeof(sock_str_ptr->addr)) < 0){
                err = get_error();
            }
            /*set non blocking*/
            err = ioctlsocket(sock_str_ptr->sd, FIONBIO, &ret);
            err = ioctlsocket(sock_str_ptr->sd, FIONBIO, &ret);
            break;

        //** output update
        case 1:
            err = recvfrom(
                    sock_str_ptr->sd,
                    (char *)&tmp[0],
                    sizeof(tmp),
                    0, 0, 0);
            if (err < 0){
   		break;
	    }
            for (i=0;i<MAX_CHANNELS;i++){
                y(0,i)=(double)tmp[i];
            }
            break;

       //** ending
        case 5:
            close_socket(sock_str_ptr->sd);
            free(sock_str_ptr);
            break;
    }

}

void udp_sender(scicos_block *block,int flag){
    int i;
    int port;
    int err;
    float tmp[MAX_CHANNELS];
    switch (flag){

        //**Init
         case 4:
            init();
            sock_str_ptr = (struct UDPStruct *)malloc(sizeof(struct UDPStruct));
            port =ipar(4);
            sock_str_ptr->sd = socket(AF_INET, SOCK_DGRAM,0);
            if (sock_str_ptr->sd < 0)
                ; /*ERROR!!!*/
            sock_str_ptr->addr.sin_family = AF_INET;
            sock_str_ptr->addr.sin_port = htons(port);
            sock_str_ptr->addr.sin_addr.S_un.S_un_b.s_b1=ipar(0);
            sock_str_ptr->addr.sin_addr.S_un.S_un_b.s_b2=ipar(1);
            sock_str_ptr->addr.sin_addr.S_un.S_un_b.s_b3=ipar(2);
            sock_str_ptr->addr.sin_addr.S_un.S_un_b.s_b4=ipar(3);
            /*set non blocking*/
            /*err = ioctlsocket(sock_str_ptr->sd, FIONBIO, &ret);*/
            break;

        //** output update
        case 1:
	    for (i=0;i<MAX_CHANNELS;i++)
		    tmp[i] = (float)(u(0,i));
            err = sendto(
                    sock_str_ptr->sd,
                    (char *)&tmp[0],
                    sizeof(tmp),
                    0,
		    (const struct sockaddr *)&(sock_str_ptr->addr), 
		    sizeof(struct sockaddr_in));
            if (err < 0)
		    ;//ERROR!!
	    break;

        //** ending
        case 5:
            close_socket(sock_str_ptr->sd);
            free(sock_str_ptr);
            break;
    }

}

int get_error(void){
    return WSAGetLastError();
}

void init(void){
    WSADATA data;
    WSAStartup(MAKEWORD( 2, 0 ), &data);
}


void close_socket(int sd){
    closesocket(sd);
}






