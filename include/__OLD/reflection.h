#include <string>
#include <unordered_map>
#include <type_traits>
#include <utility>

#include "utility/debugger.h"
#include "utility/array_view.h"

#include "editor_types.h"
#include "component_arguments.h"

class Behaviour;

/******************************************************
 * \brief This macro is used for exposing the component
 * 
 * It is supposed to be used like
 * ```cpp
 * REFLECT ( NewComponent );
 * ```
 *****************************************************/
#define REFLECT(type) using _internal_reflect_component = type

struct ComponentVersion
{
	u_int32_t major = 0;
	u_int32_t minor = 0;
	u_int32_t patch = 0;
	bool stable = false;

	constexpr ComponentVersion ( u_int32_t major = 0, u_int32_t minor = 0, u_int32_t patch = 0, bool stable = false )
	: major ( major ), minor ( minor ), patch ( patch ), stable ( stable )
	{ }

	constexpr bool operator > ( const ComponentVersion& other ) const
	{
		if ( major > other.major )
		{ return true; }
		else if ( minor > other.minor )
		{ return true; }
		else if ( patch > other.patch )
		{ return true; }
		else
		{ return false; }
	}

	constexpr bool operator == ( const ComponentVersion& other ) const
	{
		return (
			major == other.major &&
			minor == other.minor &&
			patch == other.patch
		);
	}

	constexpr bool operator < ( const ComponentVersion& other ) const
	{ return other > *this; }
	constexpr bool operator != ( const ComponentVersion& other ) const
	{ return ! ( *this == other ); }

	constexpr const char * To_String ( ) const 
	{
		std::string lavoration = "";
		lavoration += major;
		lavoration += ".";
		lavoration += minor;
		lavoration += ".";
		lavoration += patch;
		return lavoration.c_str ( );
	}
};

/*****************************************
 * \brief Stores the component metadata
 * 	like name, version and description
 ****************************************/
struct ComponentMetadata
{
	const char* name;
	ComponentVersion version;
	const char* description;
};

/********************************************************
 * \brief This macro is used for describing the component
 * 
 * It is supposed to be used like
 * ```cpp
 * METADATA ( "NewComponent", version, "desciption" );
 * ```
 *******************************************************/
#define METADATA(component_name,version,description)		\
inline static constexpr ComponentMetadata metadata		\
{								\
	component_name, version, description 			\
};								\
const ComponentMetadata& Get_Metadata ( ) const			\
{								\
	return _internal_reflect_component::metadata;		\
}								\
const const char* Get_Type ( ) const				\
{								\
	return _internal_reflect_component::metadata.name;	\
}

/***************************************
 * \brief Describes an exposed parameter
 * 	providing a setter and a getter
 **************************************/
struct ParameterMetadata
{
	const char* name;
	void ( * Set ) ( Behaviour *, const ComponentArguments& );
	void ( * Get ) ( const Behaviour *, ComponentArguments& );
	EditorType type;
};

/***************************************
 * \brief Describes an exposed method
 * 	of type (void)(*)(void)
 **************************************/
struct MethodMetadata
{
	const char* name;
	void ( * Call ) ( Behaviour * );
};

#define PARAMETERS(...)							\
inline static constexpr ParameterMetadata parameters[] =		\
{									\
	__VA_ARGS__							\
};									\
const ArrayView < ParameterMetadata > Get_Parameters ( ) const		\
{									\
	return {							\
		_internal_reflect_component::parameters,		\
		std::size ( _internal_reflect_component::parameters )	\
	};								\
}

/***********************************************************************
 * \brief This macro exposes to the editor the parameter field
 * 	with the name, automaticaly constructs a getter and setter
 * 
 * note that if when setting the component the named field is not setted
 * the parameter will not be modified but Modify will be called
 **********************************************************************/
#define PARAMETER(name,field) ParameterMetadata {						\
	name,											\
	[ ] ( Behaviour * pass_this, const ComponentArguments& args )				\
	{											\
		auto * t = static_cast < _internal_reflect_component * > ( pass_this );		\
		t->field = args.Get ( name, t->field );						\
		t->Modify ();									\
	},											\
	[ ] ( const Behaviour * pass_this, ComponentArguments& args )				\
	{											\
		auto * t = static_cast <const _internal_reflect_component * > ( pass_this );	\
		args.Set ( name, t->field );							\
	},											\
	Type <	std::remove_cv_t <								\
		std::remove_reference_t < 							\
			decltype ( field ) > > > :: value					\
}

/************************************************************************
 * \brief This macro exposes to the editor the parameter field
 * 	with the name, but you need to give an explicit setter and getter
 * 
 * since is the user to define the setter and getter it's the user
 * responsability to andle _modified
 * 
 * this is the case because you can choose not to invalidate
 * the component after modification
 * 
 * note that getter must be like const <type> getter ( void ) const;
 * note that setter must be callable with only one parameter
 ***********************************************************************/
#define PROPERTY(name,setter,getter) ParameterMetadata {					\
	name,											\
	[ ] ( Behaviour * pass_this, const ComponentArguments& args )				\
	{											\
		static_assert (									\
			std::is_invocable_v <							\
				decltype ( &_internal_reflect_component::setter ),		\
				_internal_reflect_component *,					\
				decltype (							\
					std::declval 						\
					< _internal_reflect_component > ( ).getter ( ) )	\
			>									\
		);										\
		auto * t = static_cast < _internal_reflect_component * > ( pass_this );		\
		t->setter ( args.Get ( name, t->getter ( ) ) );					\
	},											\
	[ ] ( const Behaviour * pass_this, ComponentArguments& args )				\
	{											\
		auto * t = static_cast <const _internal_reflect_component * > ( pass_this );	\
		args.Set ( name, t->getter ( ) );						\
	},											\
	Type <	std::remove_cv_t <								\
		std::remove_reference_t <							\
			decltype (								\
				std::declval							\
					< const _internal_reflect_component > ( ).getter ( ) >	\
			) > > :: value								\
}

#define METHODS(...)						\
inline static constexpr MethodMetadata methods[] =			\
{								\
	__VA_ARGS__						\
};								\
const ArrayView < MethodMetadata > Get_Methods ( ) const	\
{								\
	return {						\
		_internal_reflect_component::methods,		\
		std::size ( _internal_reflect_component::methods )	\
	};							\
}

#define METHOD(name,method) MethodMetadata {						\
	name,										\
	[ ] ( Behaviour * pass_this )							\
	{										\
		auto * t = static_cast < _internal_reflect_component * > ( pass_this );	\
		t->method ( );								\
	}										\
}