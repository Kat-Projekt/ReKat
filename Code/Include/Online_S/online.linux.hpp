#ifndef KAT_ONLINE
#define KAT_ONLINE

// network
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

// Common
#include "common.hpp"

namespace ReKat {
namespace online {
	namespace internal {
		static bool connected = false;
		static int node_sock = -1;
		static int fath_sock = -1;

		static int send_buf ( int sock, const char *buf, size_t sizeof_buf, timeval *__restrict__ wait_time );
		static int recv_buf ( int sock, char *buf, size_t sizeof_buf, timeval *__restrict__ wait_time );
	} // private variables
} /* Online */ } // ReKat

int ReKat::online::Connect ( std::string _servername ) {
	if ( internal::connected ) 
	{ return ALREADY_CONNECTED; }
	
	std::string ip = _servername;
	// test if on server_list
	for (size_t i = 0; i < ReKat::online::servers.size(); i += 2) {
		if ( servers[i] == _servername ) 
		{ ip = servers[i+1]; break; } }

	std::string father_node_ip;
	sockaddr_in father_node_addr;
		
	// set up socket
	father_node_addr.sin_family = AF_INET;
	father_node_addr.sin_port = htons(DPORT);
	inet_pton(AF_INET, father_node_ip.c_str(), &father_node_addr.sin_addr);

	if( ( internal::node_sock = socket ( AF_INET , SOCK_STREAM , 0 ) ) < 0 )
	{ return FAILED_SOCKET; }

	// connect
	if ( ( internal::fath_sock = connect ( internal::node_sock, (struct sockaddr*)&father_node_addr, sizeof(father_node_addr) ) ) < 0) 
	{ return FAILED_CONNECTION; }

	internal::connected = true;
	return SUCCESS;
}

static int ReKat::online::Send ( const char* _buf, long size ) {
	if ( !internal::connected ) { return FAILED_CONNECTION; }

	auto Timeout = timeval();
	Timeout.tv_sec = DTIMEOUT;
	return internal::send_buf ( internal::fath_sock, _buf, size, &Timeout );
}

static int ReKat::online::Recv ( char* _buf, long size ) {
	if ( !internal::connected ) { return FAILED_CONNECTION; }

	auto Timeout = timeval();
	Timeout.tv_sec = DTIMEOUT;
	return internal::recv_buf ( internal::fath_sock, _buf, size, &Timeout );
}

static int ReKat::online::internal::send_buf 
( int sock, const char *buf, size_t sizeof_buf, timeval *__restrict__ wait_time ) {
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
( int sock, char *buf, size_t sizeof_buf, timeval *__restrict__ wait_time ) {
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
	close ( internal::node_sock );
	close ( internal::fath_sock );
	internal::connected = false;
} 

#endif