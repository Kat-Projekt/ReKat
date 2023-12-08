#ifndef Audio
#define Audio

#define compl_bus int
#define uncomp_bus int

namespace ReKat {
namespace audio {
	int play ( uncomp_bus _buf);
	compl_bus compress ( uncomp_bus _buf );
	uncomp_bus decompress ( compl_bus _buf );
} /* audio */ } // ReKat

#endif