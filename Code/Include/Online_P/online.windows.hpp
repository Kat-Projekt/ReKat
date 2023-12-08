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
        struct node_infos {
            SOCKET sock = INVALID_SOCKET;
            char* internal_name = nullptr;
            size_t ID;
        };

        node_infos this_node;
        std::unordered_map < std::string, node_infos > node_network;
        static WSADATA wsaData;
		
        static int send_buf ( node_infos sock, const char *buf, size_t sizeof_buf, int *time );
		static int recv_buf ( node_infos sock, char *buf, size_t sizeof_buf, int *time );
	} // private variables
} /* Online */ } // ReKat

int ReKat::online::Start 
( std::string name, size_t ID, int port ) {
    internal::this_node.ID = ID;
    internal::this_node.internal_name = (char*)calloc(BUF_LEN,sizeof(char));
    for (size_t i = 0; i < name.size(); i++) { internal::this_node.internal_name[i] = name[i]; }

    struct addrinfo *result = NULL;
    struct addrinfo hints;
    
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2,2), &internal::wsaData) != 0) 
    { return FAILED_WSADATA; }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    if ( getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result) != 0 ) 
    { WSACleanup(); return FAILED_HOST_RESOLVE; }

    // Create a SOCKET for the server to listen for client connections.
    internal::this_node.sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if ( internal::this_node.sock == INVALID_SOCKET ) {
        // printf("socket failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return FAILED_SOCKET;
    }

    // Setup the TCP listening socket
    if ( bind( internal::this_node.sock, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR ) {
        // printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(internal::this_node.sock);
        WSACleanup();
        return FAILED_BIND;
    }

    freeaddrinfo(result);

    // start lisening
    if ( listen( internal::this_node.sock, SOMAXCONN ) == SOCKET_ERROR ) {
        // printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(  internal::this_node.sock );
        WSACleanup();
        return FAILED_LISTEN;
    }

    return SUCCESS;
}

static int ReKat::online::Connect 
( std::string new_name, std::string address, const char* port ) {
    internal::node_infos new_node;

    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    int erroe;
    if ( erroe = getaddrinfo(address.c_str(), port, &hints, &result) != 0 ) 
    { std::cout << "erroe: "<< WSAGetLastError() << '\n'; return FAILED_HOST_RESOLVE; }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {
        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) 
        { return FAILED_SOCKET; }

        // Connect to server.
        if (connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }
    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) 
    { closesocket(ConnectSocket); return FAILED_CONNECTION; }

    new_node.sock = ConnectSocket;

    // send infos
    if ( internal::send_buf ( new_node, internal::this_node.internal_name, BUF_LEN, nullptr) == FAILED_SEND ) 
    { closesocket(ConnectSocket); return FAILED_SEND; }
    if ( internal::send_buf ( new_node, (char*)&internal::this_node.ID, sizeof(size_t), nullptr) == FAILED_SEND ) 
    { closesocket(ConnectSocket); return FAILED_SEND; }

    // get infos
    new_node.internal_name = (char*) calloc (BUF_LEN,sizeof(char));
    if ( internal::recv_buf ( new_node, new_node.internal_name, BUF_LEN, nullptr) == FAILED_RECV ) 
    { closesocket(ConnectSocket); return FAILED_RECV; }
    if ( internal::recv_buf ( new_node, (char*)&new_node.ID, sizeof(size_t), nullptr) == FAILED_RECV ) 
    { closesocket(ConnectSocket); return FAILED_RECV; }

    internal::node_network.insert( {new_name, new_node} );

	return SUCCESS;
}

static int ReKat::online::New_Connection 
( ) {
	fd_set FD_Listen;
	FD_ZERO ( &FD_Listen ); //Clearing the socket set
	FD_SET  ( internal::this_node.sock, &FD_Listen ); //Adding the master socket to the set 
			
	//Waiting for something to happen on the master socket.
	int act = select( 1 , &FD_Listen , nullptr , nullptr , nullptr /**/);

	if ((act < 0) && (errno!=EINTR)) { return FAILED_SELECT; }
	//Any activity on the master socket is treated as an incoming connection
	if ( FD_ISSET( internal::this_node.sock, &FD_Listen ) ) {
        internal::node_infos new_node;
        
        // Accept a client socket
		new_node.sock = accept( internal::this_node.sock, NULL, NULL );

        if ( new_node.sock == INVALID_SOCKET ) 
        { return FAILED_CONNECTION; }

        std::cout <<  "connecting\n";

        // get infos
        new_node.internal_name = (char*) calloc (BUF_LEN,sizeof(char));
        if ( internal::recv_buf ( new_node, new_node.internal_name, BUF_LEN, nullptr) == FAILED_RECV ) 
        { closesocket(new_node.sock); return FAILED_CONNECTION; }
        if ( internal::recv_buf ( new_node, (char*)&new_node.ID, sizeof(size_t), nullptr) == FAILED_RECV ) 
        { closesocket(new_node.sock); return FAILED_CONNECTION; }
        
        // send infos
        if ( internal::send_buf ( new_node, internal::this_node.internal_name, BUF_LEN, nullptr) == FAILED_SEND ) 
        { closesocket(new_node.sock); return FAILED_CONNECTION; }
        if ( internal::send_buf ( new_node, (char*)&internal::this_node.ID, sizeof(size_t), nullptr) == FAILED_SEND ) 
        { closesocket(new_node.sock); return FAILED_CONNECTION; }

	    internal::node_network.insert( { std::string(new_node.internal_name), new_node } );
	}

    return SUCCESS;
}

static int ReKat::online::Send 
( const char* _buf, size_t size, std::string node, int time ) {
    int *time_p = &time;
    if ( time < 0 ) { time_p = nullptr; }
    
    int result = 0;

    // send size
    result = internal::send_buf ( internal::node_network[node], (char*)&size, sizeof(size_t), time_p );
    if ( result != SUCCESS ) { return result; }

    // send _buf
    return internal::send_buf ( internal::node_network[node], _buf, size, time_p );
}

static char* ReKat::online::Recv 
(  std::string node, int *exit_status, size_t *size, int time ) {
    int *time_p = &time;
    if ( time < 0 ) { time_p = nullptr; }
    
    size_t new_size = 0;
    char * _buf;

    int result = 0;

    result = internal::recv_buf ( internal::node_network[node], (char*)&new_size, sizeof(size_t), time_p );
    if ( result != SUCCESS ) { (*exit_status) = result; return nullptr; }

    _buf = (char*) calloc ( new_size + 1, sizeof(char));
    result = internal::recv_buf ( internal::node_network[node], _buf, new_size, time_p ); 
    if ( result != SUCCESS ) { (*exit_status) = result; return nullptr; }

    if ( size != nullptr ) { (*size) = new_size; }
    (*exit_status) = SUCCESS;
    return _buf;
}

static int ReKat::online::internal::send_buf 
( node_infos sock, const char *buf, size_t sizeof_buf, int *time ) {
	fd_set sending_sock;
	FD_ZERO(&sending_sock);
	FD_SET(sock.sock, &sending_sock);
		
	int act;
    if ( time == nullptr ) 
    { act = select ( 1, nullptr, &sending_sock ,nullptr, nullptr ); } else {
		timeval wait_time = timeval();
		wait_time.tv_sec = *time;
		act = select ( 1, nullptr, &sending_sock ,nullptr, &wait_time );
	}

	if (act < 0) { return TIMEOUT; }
	if (FD_ISSET(sock.sock, &sending_sock)) {
		size_t remaining = sizeof_buf;
		size_t result = 0;
		size_t sent = 0;

		while (remaining > 0) {
			result = send(sock.sock, buf+sent, (int)remaining, 0);

			if (result > 0) { 
				remaining -= result; sent += remaining; 
			}
			else if (result < 0) { return FAILED_SEND; } // std::cout << "\tsend errno: " << errno << '\n';
		}
				
		return SUCCESS;
	}

	return FAILED_SEND;
}

static int ReKat::online::internal::recv_buf 
( node_infos sock, char *buf, size_t sizeof_buf, int *time ) {
	fd_set recive_sock;
	FD_ZERO(&recive_sock);
	FD_SET(sock.sock, &recive_sock);
	
	int act;
    if ( time == nullptr ) 
    { act = select ( 1, &recive_sock, nullptr ,nullptr, nullptr ); } else {
		timeval wait_time = timeval();
		wait_time.tv_sec = *time;
		act = select ( 1, &recive_sock, nullptr ,nullptr, &wait_time );
	}

	if (act < 0) { return TIMEOUT; }
	if (FD_ISSET(sock.sock, &recive_sock)) {
		size_t remaining = sizeof_buf;
		size_t received = 0;
		size_t result = 0;
		
		while (remaining > 0) {
			result = recv(sock.sock, buf+received, (int)remaining, 0);
			if (result > 0) {
				remaining -= result;
				received += result;
			}
			else if (result == 0) { return DISCONNECTED; /*disconnection*/ }
			else if (result  < 0) { return FAILED_RECV; } // std::cout << "\trecv errno: " << errno << '\n';
		}

		return SUCCESS;
	}

	return FAILED_RECV;
}

static void ReKat::online::End 
( ) {
    // stop listeing
    closesocket ( internal::this_node.sock );

    // close sockets
    for ( auto sock : internal::node_network ) 
    { closesocket ( sock.second.sock ); }

	WSACleanup();
}

static std::vector<std::string> ReKat::online::Connected 
( ) {
    std::vector<std::string> nodi;
    for ( auto node : internal::node_network ) { nodi.push_back(node.first); }
    return nodi;
}

static int ReKat::online::CHECK 
( ) { 
    internal::node_infos p;
    p.sock = INVALID_SOCKET;
    std::cout << &p << '\n';
    std::cout << p.sock << '\n'; 
    
std::cout << "check valid"; return SUCCESS; }

static int ReKat::online::Close_sock ( std::string node ) {
	closesocket ( internal::node_network[node].sock );
	internal::node_network.erase ( node );
	return SUCCESS;
}

#endif