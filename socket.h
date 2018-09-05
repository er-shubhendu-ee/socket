/* necessarry defines and inclusions to work with socket for internet connection in linux and windows  */

#ifndef __socket_h
#define __socket_h


#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#if defined WINDOWS
	#include <ws2tcpip.h>
	#include <winsock2.h>
	#include <ws2tcpip.h>
#endif

/*****************************  Barkley sockets api headers   *************************************/
/*
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
*/
/*************************************************************************************************/

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

#if defined WINDOWS
// Need to link with Ws2_32.lib
	#pragma comment(lib, "ws2_32.lib")
	
	#define WinSockVersion_2_2 MAKEWORD( 1,0 )
	#define DEFAULT_BUFLEN 1024
#endif

typedef enum TagErrSock
{
	ERRNULL,
	ERROR0,
	ERROR1
}ErrSock_t;

typedef enum TagSocketPurpose
{
	SEND,
	RECV,
	GET,
	POST
}SocketPurpose_t;

typedef struct TagSocketFuncDescript
{
	int SocketInstance;
	SocketPurpose_t SockPurp;
	char *pszHostName;
	char *pszSendBuff;	
}SocketFuncDescript_t, *pSocketFuncDescript_t;

/*
*	This using_socket(char *HostName) function accepts 
*
*/
ErrSock_t using_socket(SocketFuncDescript_t *);
void show_resolved_host_data(ADDRINFO *);








#endif

/************    End of socket.h   *********************/
