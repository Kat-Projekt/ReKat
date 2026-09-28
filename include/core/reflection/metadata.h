#include <stdint.h>
#include <string>
#include <ostream>

namespace Reflection
{
	typedef struct {
		const char * name;
		const char * description;
		uint64_t major;
		uint64_t minor;
		uint64_t patch;
		bool stable;
	} Metadata;

	bool Is_Version_Major ( const Metadata& lower, const Metadata& upper );

	std::string Version_Number ( const Metadata& meta );

	std::ostream& operator << ( std::ostream& os, const Metadata& meta );

	constexpr Metadata Construct ( const char * name, const char * desc,
			      uint64_t major, uint64_t minor = 0,
			      uint64_t patch = 0, bool stable = false )
	{
		return ( Reflection::Metadata {name,desc,major,minor,patch,stable} );
	}
}

/**
 * \brief metadata formatter
 */
#define METADATA(name, description, ...) /* Version numers */	\
	static constexpr Reflection::Metadata metadata = 	\
	Reflection::Construct ( #name, description, __VA_ARGS__ )
