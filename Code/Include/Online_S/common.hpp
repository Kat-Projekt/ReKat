#ifndef ONLINE_COMMON
#define ONLINE_COMMON

#include <iostream>
#include <string>
#include <vector>

#include <time.h>

namespace ReKat {
namespace online {
	#define DPORT 42069
	#define DTIMEOUT 1
	
	enum Status {
		SUCCESS,
		FAILURE,
		FAILED_SOCKET,
		FAILED_CONNECTION,
		FAILED_SEND,
		FAILED_RECV,
		ALREADY_CONNECTED,
		DISCONNECTED,
		TIMEOUT
	};

	namespace {
		std::vector<std::string> servers = {
			"main", "127.0.0.1", "test", "192.168.250.140"
		};
	} // private variables
	
	static int Connect ( std::string _servername );

	static int Send ( const char* _buf, long size);

	static int Recv ( char* _buf, long size);

	static std::vector<std::string> Get ( );

	static void End ( );
} /* Online */ } // ReKat

static std::vector<std::string> ReKat::online::Get ( ) {
	std::vector<std::string> p;
	for (size_t i = 0; i < ReKat::online::servers.size(); i += 2) {
		p.push_back ( servers[i] );
	}

	return p;	
}

#endif