/* necessarry defines and inclusions to work with socket for internet connection in linux and windows  */

#ifndef __socket_h
#define __socket_h

#ifndef WINDOWS
	#define WINDOWS
#endif



#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#if defined WINDOWS
	#include <ws2tcpip.h>
	#include <winsock2.h>
	#include <ws2tcpip.h>
	// Need to link with Ws2_32.lib
	#pragma comment(lib, "ws2_32.lib")
	
	#define WinSockVersion MAKEWORD( 1,0 )
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

#define URLLENGTH_MAX 200
#define DEFAULT_BUFLEN 99999

typedef enum TagErrSock
{
	ERRNULL,
	ERROR0,
	ERROR1
}ErrSock_t;

typedef enum TagSocketMethod
{
    GET,
    POST,
    PUT,
    HEAD,
    DEL,
    PATCH,
    OPT
}SocketMethod_t;

typedef struct TagNetAccessStruct
{
	int SocketInstance;
	SocketMethod_t ConnectMethod;
	char *pszQueryString;
	char *pszApplicationProtocol;
	char *pszHostName;
	char *pszContentType;
	char *pszFormedReqURL;	
}NetAccessStruct_t, *pNetAccessStruct_t;

/*
*	This using_socket(char *HostName) function accepts 
*
*/

void form_send_buffer(NetAccessStruct_t *);
ErrSock_t using_socket(NetAccessStruct_t *);
void show_resolved_host_data(ADDRINFO *);



#endif

/************    End of socket.h   *********************/
