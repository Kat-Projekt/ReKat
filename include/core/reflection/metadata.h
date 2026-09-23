#include "version.h"

namespace Reflection
{
	typedef struct{
		const char * name;
		const char * description;
		const Version version;
	} Metadata;

	static std::ostream& operator << ( std::ostream& os, const Metadata& meta )
	{
		os << meta.name;
		os << " ";
		os << meta.version;
		os << " ( ";
		os << meta.description;
		os << " )";
		return os;
	}
}

/**
 * \brief metadata formatter
 */
#define METADATA(name, description, ...) /* Version numers */	\
	static constexpr Reflection::Metadata metadata = {	\
		#name,						\
		description,					\
		Reflection::Version ( __VA_ARGS__ )		\
	}; 

