#include "../components/Renderers/framebuffer.h"
namespace Manager {
	static std::unordered_map < std::string, Camera* > cameras;
	
	static int Camera_Load ( std::string name, Objekt* pointer, Framebuffer* fb ) {
		auto C = pointer->Add_Component < Camera > ( );
		if ( pointer == nullptr ) 
		{ DEBUG ( 1, "Invalid camera pointer" ); return 1; }
		if ( fb != nullptr ) 
		{ C->fb_scale = &fb->_aspect_ratio; }
		cameras.insert ( {name, C} );
		return 0;
	}
	static int Camera_Load ( std::string name, std::string pointer ) {
		if ( pointer == "" ) 
		{ DEBUG ( 1, "Invalid camera pointer" ); return 1; }
		auto C = Manager::Objekt_Get ( pointer )->Add_Component < Camera > ( );
		cameras.insert ( {name, C} );
		return 0;
	}
	static Camera * Camera_Get ( std::string name ) {
		if ( name == "" ) { return nullptr; }
		auto findit = cameras.find(name);
		if ( findit != cameras.end() ) {
			return findit->second;
		} else { return nullptr; }
	}
}