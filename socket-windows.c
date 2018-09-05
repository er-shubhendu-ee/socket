
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
	ErrSock_t ErrThisSock;
	SocketFuncDescript_t ThisSocket;
	ThisSocket.pszHostName="www.google.com";
	printf("Testing socket connection.......\n");
	ErrThisSock=using_socket(&ThisSocket);
	return 0;
}


