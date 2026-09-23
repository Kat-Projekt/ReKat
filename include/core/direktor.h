#include <memory>
#include <string>
#include <unordered_map>

class Aktor;
class Scene;
class Resource;

class Director
{
	static std::unordered_map < std::string, std::unique_ptr < Aktor > >	aktors;
	static std::unordered_map < std::string, std::unique_ptr < Scene > >	scenes;
	static std::unordered_map < std::string, std::unique_ptr < Resource > >	resources;
	static Scene* active_scene;

public:
	// Load 
	template < class R >
	static R* Load ( std::string name );
	// Gets 
	template < class R >
	static R* Find ( std::string name );
	// Destroys
	template < class R >
	static R* Drop ( std::string name );

	static void Set_Active_Scene ( std::string name );

	static void Start  ( );
	static void Update ( );
	static void Close  ( );
};
