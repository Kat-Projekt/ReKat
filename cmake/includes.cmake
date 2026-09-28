## remove openal examples
set(OPENAL_BUILD_EXAMPLES OFF)
set(ALSOFT_UTILS OFF)
set(ALSOFT_EXAMPLES OFF)
set(ALSOFT_INSTALL OFF)
set(LIBTYPE STATIC)
add_subdirectory(libraries/openal-soft)

## clean glfw installation
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(libraries/glfw)

## adds freetype
add_subdirectory(libraries/freetype)

## include config and dll
set(BOOST_INCLUDE_LIBRARIES config dll)
add_subdirectory(libraries/boost)

## for suppressing errors on BOOST and GLAD
include_directories( SYSTEM
	${CMAKE_CURRENT_SOURCE_DIR}/libraries/boost/libs/dll/include
	${CMAKE_CURRENT_SOURCE_DIR}/libraries/boost/libs/filesystem/include
	${CMAKE_CURRENT_SOURCE_DIR}/libraries/glad/include
)

## unicode text
add_definitions(-DUNICODE -D_UNICODE)

## compiler specific flags for debugging
if(MSVC)
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /std:c++17" )
else()
	set( REKAT_CXX_FLAGS
		"${CMAKE_CXX_FLAGS}"
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

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++17")

	if(NOT WIN32)
		set(GLAD_LIBRARIES dl)
	else()
		set(WINSOCK_LIBRARIES Ws2_32.lib Mswsock.lib AdvApi32.lib)
	endif()
endif()

file(GLOB LIBS_SOURCES libraries/glad/src/glad.c )
file(GLOB PROJECT_SOURCES source/*/*.cpp source/*.cpp )
file(GLOB PROJECT_HEADERS include/*.hpp )
file(GLOB PROJECT_CONFIGS	
				CMakeLists.txt
				Readme.md
				.gitattributes
				.gitignore
				.gitmodules
		)

## visual studio display
source_group("Include" FILES ${PROJECT_HEADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Libs" FILES ${LIBS_SOURCES})

message(STATUS "CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CURRENT_LIST_DIR   = ${CMAKE_CURRENT_LIST_DIR}")
