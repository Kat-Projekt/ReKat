#ifndef ONLINE_COMMON
#define ONLINE_COMMON

#include "../../../objekt.hpp"
#include <thread>

#define DPORT 42069
#define DTIMEOUT 1

class _Peer : public Behaviour {
	// creates socket
	// and creates listening threads and recv threads managment
	virtual void Start ( );

	// connects to a node and creates a recv thread
	int Connect ( std::string server_name );

	// sends a buffer
	int Send ( std::string reciver, const char* _buf, long size );

	// recv a buffer
	int Recv ( std::string sender, char* _buf, long size );

	// closes socket
	void Delete ( );
};
#endif