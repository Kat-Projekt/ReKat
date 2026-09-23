#include <cstdint>
#include <string>
#include <memory>
#include <unordered_map>
#include <list>

typedef std::uint32_t identifier;
constexpr identifier Unidentified = 0;

class Objekt;
class Resource;
class Manager;

/**
 * The Scene owns everying except components in an envoiremnt
 * So Resources and Objekts
 * There can be any amount of scenes.
 */
class Scene {
private:
	std::unordered_map < std::string, std::list < identifier > > _names_table;
	std::unordered_map < identifier, std::unique_ptr < Objekt > > _objekts;
	std::unordered_map < identifier, std::unique_ptr < Resource > > _resources;
	Objekt * _active_objekt = nullptr;

	template < class T >
	inline std::string Format_Name ( const std::string& name );

	Scene
	( ) { }

	Scene ( const std::string& path )
	{ Load_Scene ( path ); }
public:

	template < class T, typename ... Args >
	[[nodiscard]] identifier Add ( const std::string& name, Args&&... args );

	template < class T >
	[[nodiscard]] T* Get ( const std::string& name );
	
	template < class T >
	[[nodiscard]] T* Get ( identifier id );

	template < class T >
	[[nodiscard]] std::list < T* > Get_All ( const std::string& name );

	template <>
	[[nodiscard]] Objekt* Get <Objekt> ( const std::string& name );

	template <>
	[[nodiscard]] Objekt* Get <Objekt> ( identifier id );

	template < class T >
	void Free ( const std::string& name );

	template < class T >
	void Free ( identifier id );

	void Set_Active_Objekt ( const std::string& name );

	void Set_Active_Objekt ( identifier id );

	void Load_Scene ( const std::string& path );

	void Dump_Scene ( const std::string& path );

	friend class Manager;
};
