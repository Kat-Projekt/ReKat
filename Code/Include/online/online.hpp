#ifndef ONLINE
#define ONLINE

#if (defined (LINUX) || defined (__linux__) || defined (__APPLE__)) // linux implementation
	#include "system-specific-implementation/online.linux.hpp"
#elif (defined (_WIN32) || defined (_WIN64)) // windows implementaion
	#include "system-specific-implementation/online.windows.hpp"
#endif

#endif