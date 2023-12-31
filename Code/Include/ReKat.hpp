#ifndef Rekat
#define Rekat

#ifdef ONLINE_PEER 
	#if (defined (LINUX) || defined (__linux__)) // linux implementation
		#include <Online_P/online.linux.hpp>
	#elif (defined (_WIN32) || defined (_WIN64)) // windows implementaion
		#include <Online_P/online.windows.hpp>
	#elif (defined (__APPLE__)) // mac os implementation
		#include <Online_P/online.apple.hpp>
	#endif
#elif (defined(ONLINE_SERVER))
	#if (defined (LINUX) || defined (__linux__)) // linux implementation
		#include <Online_S/online.linux.hpp>
	#elif (defined (_WIN32) || defined (_WIN64)) // windows implementaion
		#include <Online_S/online.windows.hpp>
	#elif (defined (__APPLE__)) // mac os implementation
		#include <Online_S/online.apple.hpp>
	#endif
#endif

#ifdef OPENGL
	#include <Graphik/graphik.hpp>
	#ifdef ALL_MODULES
		#include <Graphik/shader.h>
		#include <Graphik/text.h>
		//#include <Graphik/texture.h>
		#include <Graphik/framebuffer.h>
		#include <Graphik/camera.h>
		#include <Graphik/sprite.h>
		#include <Graphik/button.h>
		#include <Graphik/tilemap.h>
		#include <Graphik/manager.h>
		#include <Graphik/object.h>
		#include <Graphik/colors.h>
		#include <Graphik/animator.h>
		#include <Graphik/behaviour.h>
		#include <Graphik/scene.h>
		#include <Graphik/collision.h>
	#endif
#endif

#ifdef OPENAL
	#include "Synth/synth.hpp"
#endif



#endif