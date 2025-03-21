#include "graphik.hpp"

#include "resources/manager.hpp"
#include "components/fps.h"
#include "components/camera.h"
#include "components/button.h"
#include "components/text_input.h"
#include "components/Renderers/sprite.h"
#include "components/Renderers/text.h"
#include "components/Renderers/tilemap.h"
#include "components/Renderers/framebuffer.h"

namespace Manager {
	static int Defaults_Load ( ) {
		int loaded = 0;

		loaded += Shader_Load ( "sprite_shader_default", sprite_shader_vs, sprite_shader_fs, nullptr,nullptr,nullptr, true );
		if ( loaded ) ( DEBUG ( 1, "sprite" ) );
		loaded += Shader_Load ( "text_shader_default", text_shader_vs, text_shader_fs, nullptr,nullptr,nullptr, true );
		if ( loaded ) ( DEBUG ( 1, "text" ) );
		loaded += Shader_Load ( "framebuffer_shader_default", framebuffer_shader_vs, framebuffer_shader_fs, nullptr,nullptr,nullptr, true );
		if ( loaded ) ( DEBUG ( 1, "frame" ) );

		return loaded;
	}
};