#ifndef CAMERA_H
#define CAMERA_H

#include "../../objekt.hpp"
#include "../graphik.hpp"
#include "../graphik_debugger.hpp"

class Camera : public Behaviour {
public:
	float Scale = 1;

	auto Projkection ( ) {
		float X = ReKat::grapik::Input::screen_ration * 500.0f * Scale;
		float Y = 500.0f * Scale;
		vec3 pos = obj->Get_Pos ( );
        return glm::ortho ( pos.x - X, pos.x + X, pos.y - Y, pos.y + Y, -1.0f, 1.0f );
	}

	static auto UI_Projkection ( ) {
        DEBUG ( 5, "Getting_UI_Projkection" );
		float X = ReKat::grapik::Input::screen_ration * 500.0f;
        return glm::ortho ( - X, + X, - 500.0f, 500.0f, -1.0f, 1.0f );
	}
};

namespace Manager {
	static std::unordered_map < std::string, Camera* > cameras;
	
	static Camera * Camera_Load ( std::string name, Objekt* pointer ) {
		if ( pointer == nullptr ) 
		{ DEBUG ( 1, "Invalid camera pointer" ); return nullptr; }
		auto C = pointer->Add_Component < Camera > ( );
		cameras.insert ( {name, C} );
	}
	static Camera * Camera_Get ( std::string name ) {
		if ( name == "" ) { return nullptr; }
		auto findit = cameras.find(name);
		if ( findit != cameras.end() ) {
			return findit->second;
		} else { return nullptr; }
	}
}

#endif