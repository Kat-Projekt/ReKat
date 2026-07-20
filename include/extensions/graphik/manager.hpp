#ifndef GRAPH_MANAGER
#define GRAPH_MANAGER

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
	extern std::unordered_map < std::string, std::shared_ptr < Camera > > _cameras;

	extern inline int Defaults_Load ( void )
	{
		int loaded = 0;

		loaded += Manager::Make < Shader > (
			"sprite_shader_default",
			ReKat::Graphik::sprite_shader_vs, ReKat::Graphik::sprite_shader_fs,
			true, nullptr, nullptr, nullptr
		);
		if ( loaded ) 
		{ DEBUG ( 1, "FAILED LOADING DEFAULT SPRITE SHADER" ); }

		loaded += Manager::Make < Shader > (
			"text_shader_default",
			ReKat::Graphik::text_shader_vs, ReKat::Graphik::text_shader_fs,
			true, nullptr, nullptr, nullptr
		);
		if ( loaded )
		{ DEBUG ( 1, "FAILED LOADING DEFAULT TEXT SHADER" ); }

		loaded += Manager::Make < Shader > (
			"framebuffer_shader_default",
			ReKat::Graphik::framebuffer_shader_vs,
			ReKat::Graphik::framebuffer_shader_fs,
			true, nullptr, nullptr, nullptr
		);
		if ( loaded ) 
		{ DEBUG ( 1, "FAILED LOADING DEFAULT FRAMEBUFFER SHADER" ); }

		return loaded;
	}

	extern inline int Camera_Bind ( std::string name, std::string objekt, Framebuffer* framebuffer = nullptr );
	extern inline std::shared_ptr < Camera > Camera_Get ( std::string name );
};

namespace Manager {
	std::unordered_map < std::string, std::shared_ptr < Camera > > _cameras;

	extern inline int Camera_Bind (
		std::string name,
		std::string objekt,
		Framebuffer* framebuffer
	) {
		std::shared_ptr < Objekt > Obj = Manager::Objekt_Get ( objekt );
		if ( Obj == nullptr )
		{ return 1; }

		auto Cam = Obj->Add_Component < Camera > ( );
		if ( Cam == nullptr )
		{ DEBUG ( 1, "Invalid camera pointer" ); return 1; }

		if ( framebuffer != nullptr ) 
		{ Cam->fb_scale = &framebuffer->_aspect_ratio; }
		
		_cameras [name] = Cam;

		return 0;
	}

	extern inline std::shared_ptr < Camera > Camera_Get ( std::string name )
	{
		if ( name == "" ) { return nullptr; }
		
		auto findit = _cameras.find ( name );
		
		if ( findit != _cameras.end ( ) )
		{ return findit->second; }
		else
		{ return nullptr; }
	}

}


#endif