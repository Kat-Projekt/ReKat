#ifndef ONLINE_COMMON
#define ONLINE_COMMON

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include <time.h>
// #include "crypt_core.hpp"

namespace ReKat {
namespace online {
	#define DPORT 42069
	#define DPORT_W "42069"
	#define DTIMEOUT 1
	#define BUF_LEN 32
	
	enum Status {
		SUCCESS,
		FAILURE,
		FAILED_WSADATA,
		FAILED_HOST_RESOLVE,
		FAILED_SOCKET,
		FAILED_BIND,
		FAILED_LISTEN,
		FAILED_CONNECTION,
		FAILED_SELECT,
		FAILED_SEND,
		FAILED_RECV,
		ALREADY_CONNECTED,
		DISCONNECTED,
		TIMEOUT
	};

	/// @brief Check if the librarys are linked correctly
	/// Twrows if errors occurred
	static int CHECK ( );
	
	/// @brief Start a node with name and id
	/// @param name the name of the node max BUF_LEN as lenth
	/// @param ID the unique ID if the node (possibly HASH"name_password")
	/// @param port between 1024 and 65.535
	static int Start ( std::string name, size_t ID , int port );

	/// @brief Accepts a new connection and adds the transmitted node_name to the Node_Network
	static int New_Connection ( );

    /// @brief Connects this node to the specified node and names it new_name (the original name, ID are stored in the internal infos) 
    static int Connect ( std::string new_name, std::string address, const char* port = DPORT_W );

	/// @brief Sends _buf of lenth size from node and waits time seconds (if time < 0 wait indeterminatly). Sends single buffers of BUF_LEN
	static int Send ( const char* _buf, size_t size, std::string node, int time = DTIMEOUT );

	/// @brief Retrurn the rescived _buf sent from a Send call and sets *size to its lenth
	static char * Recv ( std::string node, int *exit_status, size_t *size = nullptr, int time = DTIMEOUT );

	/// @brief disconnects the node
	static int Close_sock ( std::string node );

	/// @brief Get the name of every entry of the Node_Network (even the disconnected) to prevent this us Refresh before calling
	static std::vector<std::string> Connected ( );

	static void End ( );
} /* Online */ } // ReKat

#endif