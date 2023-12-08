#ifndef KAT_ONLINE
#define KAT_ONLINE

// network
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

// Common
#include "common.hpp"

namespace ReKat {
namespace online {
	namespace internal {
		static bool connected = false;
        static WSADATA wsaData;
        static SOCKET ConnectSocket = INVALID_SOCKET;
		
        static int send_buf ( int sock, const char *buf, size_t sizeof_buf, timeval * wait_time );
		static int recv_buf ( int sock, char *buf, size_t sizeof_buf, timeval * wait_time );
	} // private variables
} /* Online */ } // ReKat

int ReKat::online::Connect ( std::string _servername ) {
	if ( internal::connected ) 
	{ return ALREADY_CONNECTED; }
	
	std::string father_node_ip = _servername;
	// test if on server_list
	for (size_t i = 0; i < ReKat::online::servers.size(); i += 2) {
		if ( servers[i] == _servername ) 
		{ father_node_ip = servers[i+1]; break; } }
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    const char *sendbuf = "this is a test";
    char recvbuf[10];
    int iResult;
    int recvbuflen = 10;
 
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(father_node_ip.c_str(), "42069", &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // Send an initial buffer
    iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);

    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Receive until the peer closes the connection
    do {

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if ( iResult > 0 )
            printf("Bytes received: %d\n", iResult);
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while( iResult > 0 );

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2,2), &internal::wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    internal::ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
    ptr->ai_protocol);

    if (internal::ConnectSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
	// connect
    if (connect( internal::ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
        closesocket(internal::ConnectSocket);
        internal::ConnectSocket = INVALID_SOCKET;
    }

    // Should really try the next address returned by getaddrinfo
    // if the connect call failed
    // But for this simple example we just free the resources
    // returned by getaddrinfo and print an error message

    if (internal::ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

	internal::connected = true;
	return SUCCESS;
}

static int ReKat::online::Send ( const char* _buf, long size ) {
	if ( !internal::connected ) { return FAILED_CONNECTION; }

	auto Timeout = timeval();
	Timeout.tv_sec = DTIMEOUT;
	return internal::send_buf ( internal::ConnectSocket, _buf, size, &Timeout );
}

static int ReKat::online::Recv ( char* _buf, long size ) {
	if ( !internal::connected ) { return FAILED_CONNECTION; }

	auto Timeout = timeval();
	Timeout.tv_sec = DTIMEOUT;
	return internal::recv_buf ( internal::ConnectSocket, _buf, size, &Timeout );
}

static int ReKat::online::internal::send_buf 
( int sock, const char *buf, size_t sizeof_buf, timeval *wait_time ) {
	fd_set sending_sock;
	FD_ZERO(&sending_sock);
	FD_SET(sock, &sending_sock);
		
	int act = select(sock + 1, nullptr, &sending_sock ,nullptr, wait_time);
	std::cout << "\tselect send: " << act << '\n';

	if (act < 0) { return TIMEOUT; }
	if (FD_ISSET(sock, &sending_sock)) {
		std::cout << "\tstart send\n";
		long remaining = sizeof_buf;
		long result = 0;
		long sent = 0;

		while (remaining > 0) {
			result = send(sock, buf+sent, remaining, 0);

			if (result > 0) { 
				std::cout << "\t\tsend byte: " << result << '\n'; 
				remaining -= result; sent += remaining; 
			}
			else if (result < 0) { std::cout << "\tsend errno: " << errno << '\n'; return FAILED_SEND; }
		}
				
		return SUCCESS;
	}

	return FAILED_SEND;
}

static int ReKat::online::internal::recv_buf 
( int sock, char *buf, size_t sizeof_buf, timeval * wait_time ) {
	fd_set recive_sock;
	FD_ZERO(&recive_sock);
	FD_SET(sock, &recive_sock);
			
	int act = select(sock + 1, &recive_sock, nullptr ,nullptr, wait_time);
	std::cout << "\tselect recv: " << act << '\n';

	if (act < 0) { return TIMEOUT; }
	if (FD_ISSET(sock, &recive_sock)) {
		std::cout << "\tstart recv\n";
		long remaining = sizeof_buf;
		long received = 0;
		long result = 0;
		
		while (remaining > 0) {
			result = recv(sock, buf+received, remaining, 0);
			if (result > 0) {
				std::cout << "\t\trecv byte: " << result << '\n';
				remaining -= result;
				received += result;
			}
			else if (result == 0) { return DISCONNECTED; /*disconnection*/ }
			else if (result  < 0) { std::cout << "\trecv errno: " << errno << '\n'; return FAILED_RECV; }
		}

		return SUCCESS;
	}

	return FAILED_RECV;
}

static void ReKat::online::End 
( ) { 
	WSACleanup();
	internal::connected = false;
}

#endif