set( REKAT_CXX_FLAGS
	"-Weverything"
	"-Werror"
	"-Wno-c++98-compat"
	"-Wno-c++98-compat-pedantic"
	"-Wno-c++11-extensions"
	"-Wno-c++20-extensions"
	"-Wno-c++23-extensions"
	"-Wno-documentation"
	"-Wno-poison-system-directories"
	"-Wno-weak-vtables"
	"-Wno-padded"
	"-Wno-zero-as-null-pointer-constant"
	## for glad.c 
	"$<$<COMPILE_LANGUAGE:CXX>:-std=c++17>"
	"$<$<COMPILE_LANGUAGE:C>:-Wno-strict-prototypes>"
	"-Wno-nonportable-include-path"
)

