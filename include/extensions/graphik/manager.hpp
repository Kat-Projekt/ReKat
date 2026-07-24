#ifndef GRAPH_MANAGER
#define GRAPH_MANAGER
#pragma once

#include "graphik_resource.hpp"
#include "objekt/manager.hpp"

#include "resources/shader.h"
#include "resources/texture.h"
#include "resources/font.h"
// #include "resources/tilemap_map.h"
// #include "resources/instance.h"
#include "components/framebuffer.h"
#include "components/camera.h"
#include "resources/default_shaders.h"

namespace Manager {
	extern std::unordered_map < std::string, Camera* > _cameras;

	extern int Defaults_Load ( void );

	extern int Camera_Rename ( std::string name, Camera* camera );
	extern int Camera_Bind ( std::string name, std::string objekt, Framebuffer* framebuffer = nullptr );
	extern Camera* Camera_Get ( std::string name );
};

#endif