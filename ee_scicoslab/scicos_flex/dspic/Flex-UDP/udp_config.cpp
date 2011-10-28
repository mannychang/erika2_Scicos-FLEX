#include "stdafx.h"
#include "dll_udp.h"
#include "udp_config.h"




/*** udp_config ***/
UDP_LIB_API void udp_config(scicos_block *block,int flag){
	WSADATA			WSAData;
	unsigned long int noblock = 1;
    int err;
	static int udp_config_file_is_opened = 1;
	static int udp_socket_is_initialized = 0;

    switch (flag){
         case SCICOS_BLOCK_INIT:	
			if(!udp_config_file_is_opened) {
				fuscrs = fopen("logrs.txt","w");
				udp_config_file_is_opened = 1;
			}
			if(!udp_socket_is_initialized) {	
				for(int i=0; i<10; i++)
					fprintf(fuscrs,"ipar(%d)=%d\n",i,ipar(i));

				if(WSAStartup((short)0x0202, &WSAData) != 0) {
					fprintf(fuscrs,"udp_config: WSAStartup failed!\n");
					fprintf(fuscrs,"#error: %d\n",WSAGetLastError());
					break;
				}
				else
					fprintf(fuscrs,"udp_config: WSAStartup configured successfully!\n");
				sock_str_ptr_rs->sd = socket(AF_INET, SOCK_DGRAM,0);
				if(sock_str_ptr_rs->sd == INVALID_SOCKET) {
					fprintf(fuscrs,"udp_config: Unable to create a socket.\n");
					fprintf(fuscrs,"#error: %d\n",WSAGetLastError());
					WSACleanup();
					break;
				}
				sock_str_ptr_rs->addr.sin_family = AF_INET;
				sock_str_ptr_rs->addr.sin_port = htons(PC_PORT);
				sock_str_ptr_rs->addr.sin_addr.S_un.S_un_b.s_b1 = PC_IP_ADDRESS_B1;
				sock_str_ptr_rs->addr.sin_addr.S_un.S_un_b.s_b2 = PC_IP_ADDRESS_B2;
				sock_str_ptr_rs->addr.sin_addr.S_un.S_un_b.s_b3 = PC_IP_ADDRESS_B3;
				sock_str_ptr_rs->addr.sin_addr.S_un.S_un_b.s_b4 = PC_IP_ADDRESS_B4;
				//sock_str_ptr_rs->addr.sin_addr.s_addr = inet_addr(PC_IP_ADDRESS);
				err = bind(sock_str_ptr_rs->sd,(sockaddr *) &(sock_str_ptr_rs->addr),sizeof(sock_str_ptr_rs->addr)); 
				if(err == INVALID_SOCKET) {
					fprintf(fuscrs,"Bind procedure failed.\n");
					fprintf(fuscrs,"#error: %d\n",WSAGetLastError());
					WSACleanup();
					break;
				}

				int rxbufsize = 60; 
				err = setsockopt(sock_str_ptr_rs->sd, SOL_SOCKET, SO_RCVBUF, (char *)&rxbufsize, (int)sizeof(rxbufsize));
				fprintf(fuscrs,"setsockopt return value:%d\n", err);
				if(err == SOCKET_ERROR) {
					fprintf(fuscrs,"setsockopt failed.\n");
					fprintf(fuscrs,"#error: %d\n",WSAGetLastError());
					WSACleanup();
					break;
				}
				int sockbufsize, size = sizeof(int); 
				err = getsockopt(sock_str_ptr_rs->sd, SOL_SOCKET, SO_RCVBUF, (char *)&sockbufsize, &size);
				fprintf(fuscrs,"getsockopt return value:%d\n", err);
				fprintf(fuscrs,"getsockopt sockbufsize:%d\n", sockbufsize);
				if(err == SOCKET_ERROR) {
					fprintf(fuscrs,"getsockopt failed.\n");
					fprintf(fuscrs,"#error: %d\n",WSAGetLastError());
					WSACleanup();
					break;
				}
				
				/*set non blocking*/
				err = ioctlsocket(sock_str_ptr_rs->sd, FIONBIO, &noblock);

				/* remote socket */
				dest_addr.sin_family = AF_INET;
				dest_addr.sin_port = htons(REMOTE_PORT);
				dest_addr.sin_addr.S_un.S_un_b.s_b1 = REMOTE_IP_ADDRESS_B1;
				dest_addr.sin_addr.S_un.S_un_b.s_b2 = REMOTE_IP_ADDRESS_B2;
				dest_addr.sin_addr.S_un.S_un_b.s_b3 = REMOTE_IP_ADDRESS_B3;
				dest_addr.sin_addr.S_un.S_un_b.s_b4 = REMOTE_IP_ADDRESS_B4;
				//dest_addr.sin_addr.s_addr = inet_addr(REMOTE_IP_ADDRESS);

				udp_socket_is_initialized = 1;
			}
            break;

        case SCICOS_BLOCK_OUTPUT_UPDATE:		
			break;

        case SCICOS_BLOCK_END:		
			closesocket(sock_str_ptr_rs->sd);
			WSACleanup();
			fprintf(fuscrs,"udp_config: closing...done!\n");
			udp_socket_is_initialized = 0;

			if(udp_config_file_is_opened){
				fclose(fuscrs);
				udp_config_file_is_opened = 0;
			}
            break;
    }

}



