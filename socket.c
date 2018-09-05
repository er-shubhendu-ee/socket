#include "socket.h"



ErrSock_t using_socket(NetAccessStruct_t *ThisConnectionInstance)
{
	char *pszHostName=ThisConnectionInstance->pszHostName;
	int SizeHostName=strlen(pszHostName);
	int ResponseWSAStartup;
	int ResponseConnect;
	int ResponseCloseSocket;
	int ResponseGetAddrInfo;
	SOCKET SocketDescriptor;
	WSADATA SocketData;
			SocketData.wVersion	 	  = WinSockVersion;
			SocketData.wHighVersion	  = WinSockVersion;
	char WinSocImplementationInfo[255];
	char WinSocConfigInfo[255];
	SOCKADDR_IN saServer;
	ADDRINFO ServerAddrInfo_IN;
	ZeroMemory( &ServerAddrInfo_IN, sizeof(ServerAddrInfo_IN) );
			 ServerAddrInfo_IN.ai_family=AF_UNSPEC;
			 ServerAddrInfo_IN.ai_socktype=SOCK_STREAM;
			 ServerAddrInfo_IN.ai_protocol=IPPROTO_TCP;
	ADDRINFO *ServerAddrInfo_OUT=NULL;
    ADDRINFO *pAddrInfoNULL = NULL;
		
	ResponseWSAStartup=WSAStartup(
								WinSockVersion,
  								&SocketData
								);
	if(ResponseWSAStartup!=0)
	{
		printf("Winsock DLL initialization failed.\n");
		printf("Error code in response: %d\n", ResponseWSAStartup);
	}
	else
	{
		printf("Winsock DLL has been initialized.\n");
	
		SocketDescriptor=socket(
							AF_INET,
							SOCK_STREAM,
							IPPROTO_TCP
							);	
		if (SocketDescriptor == INVALID_SOCKET)
		{
			int ErrCode=WSAGetLastError();
			printf("Error: unable to create socket.\nError code: %d\n",ErrCode);
		}
		else
		{
			printf("Socket creation success.\n");
			
			printf("Host: %s\n", pszHostName);
			printf("Resolving host name.\n");

			ResponseGetAddrInfo=getaddrinfo(pszHostName,"http", &ServerAddrInfo_IN, &ServerAddrInfo_OUT);	

			if(ResponseGetAddrInfo!=0)
			{
				printf("Error resolving host name.\n");
				printf("Error code: %d\n", ResponseGetAddrInfo);
			}
			else
			{
				
				printf("Resolving host name success\n");
				show_resolved_host_data(ServerAddrInfo_OUT);				
				
				/*  google ip http://74.125.24.147/*/
				printf("Connecting using this socket..........\n");
				ResponseConnect=connect(
									SocketDescriptor,
									ServerAddrInfo_OUT->ai_addr,
									(int) ServerAddrInfo_OUT->ai_addrlen
									);
				if(ResponseConnect==SOCKET_ERROR)
				{
					printf("Error in connecting using this socket.\n");
					printf("Error No.: %d\n", WSAGetLastError());
				}
				else
				{
					printf("Connection success using this socket.\n");
					
					int recvbuflen = DEFAULT_BUFLEN;
					form_send_buffer(ThisConnectionInstance);
					char *sendbuf = ThisConnectionInstance->pszFormedReqURL;
					
					printf("Get Request in sendbuf:\n");
					printf("%s\n", sendbuf);
	
					char recvbuf[DEFAULT_BUFLEN];
					int iResult;
// Send an initial buffer
					iResult = send(SocketDescriptor, sendbuf, (int) strlen(sendbuf), 0);
					if (iResult == SOCKET_ERROR)
					{
				    	printf("send failed: %d\n", WSAGetLastError());
				    	closesocket(SocketDescriptor);
					}

					printf("Bytes Sent: %ld\n", iResult);

// shutdown the connection for sending since no more data will be sent
// the client can still use the ConnectSocket for receiving data
					iResult = shutdown(SocketDescriptor, SD_SEND);
					if (iResult == SOCKET_ERROR)
					{
				    	printf("shutdown failed: %d\n", WSAGetLastError());
				    	closesocket(SocketDescriptor);
					}

// Receive data until the server closes the connection
					do
					{
					    iResult = recv(SocketDescriptor, recvbuf, recvbuflen, 0);
					    if (iResult > 0)
					    {
					    	printf("Bytes received: %d\n", iResult);
					    	printf("Data: %s", recvbuf);
						}
				    	    
					    else if (iResult == 0)
        					printf("Connection closed\n");
					    else
					        printf("recv failed: %d\n", WSAGetLastError());
					} while (iResult > 0);
				}
			}						
		}
	}
	
	ResponseCloseSocket = closesocket(SocketDescriptor);
    if (ResponseCloseSocket == SOCKET_ERROR)
	{
        wprintf(L"closesocket function failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
    }
    else
    	printf("Successfully closed the socket.");
    return ERRNULL;
}


void show_resolved_host_data(ADDRINFO *result)
{
//	Declare and initialize variables
    WSADATA wsaData;
    int iResult;
    INT iRetval;
	ADDRINFO *ptr;
	int i;
	struct sockaddr_in  *sockaddr_ipv4;
//	struct sockaddr_in6 *sockaddr_ipv6;
    LPSOCKADDR sockaddr_ip;

    char ipstringbuffer[46];
    DWORD ipbufferlength = 46;
	 // Retrieve each address and print out the hex bytes
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next)
	{
        printf("getaddrinfo response %d\n", i++);
        printf("\tFlags: 0x%x\n", ptr->ai_flags);
        printf("\tFamily: ");
        switch (ptr->ai_family)
		{
            case AF_UNSPEC:
                printf("Unspecified\n");
                break;
            case AF_INET:
                printf("AF_INET (IPv4)\n");
                sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
                printf("\tIPv4 address %s\n",
                    inet_ntoa(sockaddr_ipv4->sin_addr) );
                break;
            case AF_INET6:
                printf("AF_INET6 (IPv6)\n");
                // the InetNtop function is available on Windows Vista and later
                // sockaddr_ipv6 = (struct sockaddr_in6 *) ptr->ai_addr;
                // printf("\tIPv6 address %s\n",
                //    InetNtop(AF_INET6, &sockaddr_ipv6->sin6_addr, ipstringbuffer, 46) );
                
                // We use WSAAddressToString since it is supported on Windows XP and later
                sockaddr_ip = (LPSOCKADDR) ptr->ai_addr;
                // The buffer length is changed by each call to WSAAddresstoString
                // So we need to set it for each iteration through the loop for safety
                ipbufferlength = 46;
                iRetval = WSAAddressToString(sockaddr_ip, (DWORD) ptr->ai_addrlen, NULL, 
                    ipstringbuffer, &ipbufferlength );
                if (iRetval)
                    printf("WSAAddressToString failed with %u\n", WSAGetLastError() );
                else    
                    printf("\tIPv6 address %s\n", ipstringbuffer);
                break;
            case AF_NETBIOS:
                printf("AF_NETBIOS (NetBIOS)\n");
                break;
            default:
                printf("Other %ld\n", ptr->ai_family);
                break;
        }
        printf("\tSocket type: ");
        switch (ptr->ai_socktype)
		{
            case 0:
                printf("Unspecified\n");
                break;
            case SOCK_STREAM:
                printf("SOCK_STREAM (stream)\n");
                break;
            case SOCK_DGRAM:
                printf("SOCK_DGRAM (datagram) \n");
                break;
            case SOCK_RAW:
                printf("SOCK_RAW (raw) \n");
                break;
            case SOCK_RDM:
                printf("SOCK_RDM (reliable message datagram)\n");
                break;
            case SOCK_SEQPACKET:
                printf("SOCK_SEQPACKET (pseudo-stream packet)\n");
                break;
            default:
                printf("Other %ld\n", ptr->ai_socktype);
                break;
        }
        printf("\tProtocol: ");
        switch (ptr->ai_protocol)
		{
            case 0:
                printf("Unspecified\n");
                break;
            case IPPROTO_TCP:
                printf("IPPROTO_TCP (TCP)\n");
                break;
            case IPPROTO_UDP:
                printf("IPPROTO_UDP (UDP) \n");
                break;
            default:
                printf("Other %ld\n", ptr->ai_protocol);
                break;
        }
        printf("\tLength of this sockaddr: %d\n", ptr->ai_addrlen);
        printf("\tCanonical name: %s\n", ptr->ai_canonname);
    }

}


void form_send_buffer(NetAccessStruct_t *ThisConnection)
{
	switch(ThisConnection->ConnectMethod)
	{
		case GET:
		{
			ThisConnection->pszFormedReqURL;
			sprintf(ThisConnection->pszFormedReqURL, "GET /%s %s\r\nHost:%s\r\nContentType:%s\r\n\r\n\r\n", ThisConnection->pszQueryString, ThisConnection->pszApplicationProtocol,ThisConnection->pszHostName, ThisConnection->pszContentType);
			break;
		}
		default:
		{
			printf("Error: Unable to form a GET request URL.");
			break;
		}
	}
}

/************    End of socket.c   *********************/
