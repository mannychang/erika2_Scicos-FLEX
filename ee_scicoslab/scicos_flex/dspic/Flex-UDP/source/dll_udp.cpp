// MakeTempFilenameDLL.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "dll_udp.h"
//using namespace std;

FILE* fuscr = fopen("logr.txt","w");
FILE* fuscs = fopen("logs.txt","w");

/*** Definitions ***/
#define u(i,j) ((double *)block->inptr[i])[j]
#define y(i,j) ((double *)block->outptr[i])[j]
#define ipar(i) (block->ipar[i])

/*** Global variables ***/
volatile UDPStruct UDPr;
volatile UDPStruct UDPs;
volatile UDPStruct* sock_str_ptr_r = &UDPr;
volatile UDPStruct* sock_str_ptr_s = &UDPs;
volatile sockaddr_in dest_addr;

/*** udp_receiver ***/
UDP_LIB_API void udp_receiver(scicos_block *block,int flag){
	WSADATA			WSAData;
    int i;
	unsigned long int ret=1;
    int port;
    int err;
    float tmp[MAX_CHANNELS];
	static int udp_receiver_file_is_opened = 1;
	static int udp_receiver_is_initialized = 0;

    switch (flag){
         case 4:	//**Init
			if(!udp_receiver_file_is_opened)
			{
				fuscr = fopen("logr.txt","w");
				udp_receiver_file_is_opened = 1;
			}
			if(!udp_receiver_is_initialized)
			{
				for (i=0;i<MAX_CHANNELS;i++)
					y(0,i)=0.0;
				if(WSAStartup((short)0x0202, &WSAData) != 0)
				{
					fprintf(fuscr,"udp_receiver: WSAStartup failed!\n");
					fprintf(fuscr,"#error: %d\n",WSAGetLastError());
				}
				else
					fprintf(fuscr,"udp_receiver: WSAStartup configured successfully!\n");
				sock_str_ptr_r->sd = socket(AF_INET, SOCK_DGRAM,0);
				if(sock_str_ptr_r->sd == INVALID_SOCKET)
				{
					fprintf(fuscr,"udp_receiver: Unable to create a socket.\n");
					fprintf(fuscr,"#error: %d\n",WSAGetLastError());
					WSACleanup();
					break;
				}
				port =ipar(4);
				sock_str_ptr_r->addr.sin_family = AF_INET;
				sock_str_ptr_r->addr.sin_port = htons(port);
				/*init_addr(block,sock_str_ptr);*/
				sock_str_ptr_r->addr.sin_addr.S_un.S_un_b.s_b1=ipar(0);
				sock_str_ptr_r->addr.sin_addr.S_un.S_un_b.s_b2=ipar(1);
				sock_str_ptr_r->addr.sin_addr.S_un.S_un_b.s_b3=ipar(2);
				sock_str_ptr_r->addr.sin_addr.S_un.S_un_b.s_b4=ipar(3);
				for(int i=0;i<5;i++)
					fprintf(fuscr,"ipar(%d)=%d\n",i,ipar(i));
				err = bind(sock_str_ptr_r->sd,(sockaddr *) &(sock_str_ptr_r->addr),sizeof(sock_str_ptr_r->addr)); 
				if(err == INVALID_SOCKET)
				{
					fprintf(fuscr,"Bind procedure failed.\n");
					fprintf(fuscr,"#error: %d\n",WSAGetLastError());
					WSACleanup();
					break;
				}
				/*set non blocking*/
				err = ioctlsocket(sock_str_ptr_r->sd, FIONBIO, &ret);
				//err = ioctlsocket(sock_str_ptr_r->sd, FIONBIO, &ret);
				udp_receiver_is_initialized = 1;
			}
            break;

        case 1:		//** output update
            err = recvfrom(sock_str_ptr_r->sd,(char *)&tmp[0],sizeof(tmp),0, 0, 0);
			if(err == SOCKET_ERROR)
			{
				fprintf(fuscr,"Socket error while trying to receive.\n");
				fprintf(fuscr,"#error: %d\n",WSAGetLastError());
			}
			if (err < 0)
   				break;
			else
			{
				fprintf(fuscr,"ret: %d\n",err);
				for (i=0;i<MAX_CHANNELS;i++)
				{
					y(0,i)=(double)tmp[i];
					fprintf(fuscr,"y(%d)= %f\n",i,y(0,i));
				}
			}
			break;

        case 5:		//** ending
			if(udp_receiver_is_initialized)
			{
				closesocket(sock_str_ptr_r->sd);
				WSACleanup();
				fprintf(fuscr,"udp_receiver: closing...done!\n");
				udp_receiver_is_initialized = 0;
			}
			if(udp_receiver_file_is_opened)
			{
				fclose(fuscr);
				udp_receiver_file_is_opened = 0;
			}
            break;
    }

}

/*** udp_sender ***/
UDP_LIB_API void udp_sender(scicos_block *block,int flag){
	WSADATA			WSAData;
    int i;
    int port;
    int err;
    float tmp[MAX_CHANNELS];
	static int udp_sender_file_is_opened = 1;
	static int udp_sender_is_initialized = 0;
	

    switch (flag){
         case 4:	//**Init
			if(!udp_sender_file_is_opened)
			{
				fuscs = fopen("logs.txt","w");
				udp_sender_file_is_opened = 1;
			}
			if(!udp_sender_is_initialized)
			{
				if(WSAStartup((short)0x0202, &WSAData) != 0)
				{
					fprintf(fuscs,"udp_sender: WSAStartup failed!\n");
					fprintf(fuscs,"#error: %d\n",WSAGetLastError());
				}
				else
					fprintf(fuscs,"udp_sender: WSAStartup configured successfully!\n");
				sock_str_ptr_s->sd = socket(AF_INET, SOCK_DGRAM,0);
				if(sock_str_ptr_s->sd == INVALID_SOCKET)
				{
					fprintf(fuscs,"udp_sender: Unable to create a socket.\n");
					fprintf(fuscs,"#error: %d\n",WSAGetLastError());
					WSACleanup();
					break;
				}
				port =ipar(4);
				sock_str_ptr_s->addr.sin_family = AF_INET;
				sock_str_ptr_s->addr.sin_port = htons(port);
				sock_str_ptr_s->addr.sin_addr.S_un.S_un_b.s_b1=ipar(0);
				sock_str_ptr_s->addr.sin_addr.S_un.S_un_b.s_b2=ipar(1);
				sock_str_ptr_s->addr.sin_addr.S_un.S_un_b.s_b3=ipar(2);
				sock_str_ptr_s->addr.sin_addr.S_un.S_un_b.s_b4=ipar(3);
				err = bind(sock_str_ptr_s->sd,(sockaddr *) &(sock_str_ptr_s->addr),sizeof(sock_str_ptr_s->addr));
				if(err == INVALID_SOCKET)
				{
					fprintf(fuscs,"Bind procedure failed.\n");
					fprintf(fuscs,"#error: %d\n",WSAGetLastError());
					WSACleanup();
					break;
				}
				/*set non blocking*/
				/*err = ioctlsocket(sock_str_ptr_s->sd, FIONBIO, &ret);*/
				port =ipar(9);
				dest_addr.sin_family = AF_INET;
				dest_addr.sin_port = htons(port);
				dest_addr.sin_addr.S_un.S_un_b.s_b1=ipar(5);
				dest_addr.sin_addr.S_un.S_un_b.s_b2=ipar(6);
				dest_addr.sin_addr.S_un.S_un_b.s_b3=ipar(7);
				dest_addr.sin_addr.S_un.S_un_b.s_b4=ipar(8);

				for(int i=0;i<10;i++)
					fprintf(fuscs,"ipar(%d)=%d\n",i,ipar(i));
				udp_sender_is_initialized = 1;
			}
            break;


        case 1:		//** output update
			for (i=0;i<MAX_CHANNELS;i++)
				tmp[i] = (float)(u(0,i));
            err = sendto(sock_str_ptr_s->sd,(char *)&tmp[0],sizeof(tmp),0,(sockaddr *)&(dest_addr), sizeof(sockaddr_in));
			if (err < 0)
			{
				fprintf(fuscs,"Socket error while trying to transmit.\n");//ERROR!!
				fprintf(fuscs,"#error: %d\n",WSAGetLastError());
			}
			break;

        case 5:		//** ending
			if(udp_sender_is_initialized)
			{
				closesocket(sock_str_ptr_s->sd);
				WSACleanup();
				fprintf(fuscs,"udp_sender: closing...done!\n");
				udp_sender_is_initialized = 0;
			}
			if(udp_sender_file_is_opened)
			{
				fclose(fuscs);
				udp_sender_file_is_opened = 0;
			}
            break;
    }

}

