
#ifndef WINDOWS
	#define WINDOWS
#endif

#ifndef UNICODE
	#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "socket.h"




int main(int argc, char *argv[])
{
	CHAR FormattedURL[URLLENGTH_MAX]="test";
//	ZeroMemory(&FormattedURL, strlen(FormattedURL));
	ErrSock_t ErrThisSock;
	NetAccessStruct_t ThisConnection;
	ZeroMemory(&ThisConnection, sizeof(ThisConnection));
	ThisConnection.ConnectMethod=GET;
	ThisConnection.pszQueryString="index.html";
	ThisConnection.pszApplicationProtocol="HTTP/1.1";
	ThisConnection.pszHostName="www.google.com";
	ThisConnection.pszContentType="text/plain";
	ThisConnection.pszFormedReqURL=FormattedURL;			

	printf("Testing socket connection.......\n");
	ErrThisSock=using_socket(&ThisConnection);

	return 0;
}


