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

// template
#include "online.refernce.hpp"


class Peer : public Behaviour {
private:
	bool connected = false;
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;

    int send_buf ( int sock, const char *buf, size_t sizeof_buf, timeval * wait_time );
	int recv_buf ( int sock, char *buf, size_t sizeof_buf, timeval * wait_time );
public:
	// creates socket
	// and creates listening threads and recv threads managment
	void Start ( ) {

	}

	// connects to a node and creates a recv thread
	int Connect ( std::string server_name );

	// sends a buffer
	int Send ( std::string reciver, const char* _buf, long size ){
		if ( !connected ) { return 1; }

		auto Timeout = timeval();
		Timeout.tv_sec = DTIMEOUT;
		return send_buf ( ConnectSocket, _buf, size, &Timeout );
	}


	// recv a buffer
	int Recv ( std::string sender, char* _buf, long size );

	// closes socket
	void Delete ( );
};