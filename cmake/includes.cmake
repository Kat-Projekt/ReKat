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

## detect os
if ( WIN32 )
	set ( REKAT_OS "windows" )
elseif ( APPLE )
	set ( REKAT_OS "macos" )
elseif ( UNIX )
	set ( REKAT_OS "linux" )
else ( )
	message ( FATAL_ERROR "UNKNOWN OS" )
endif ( )

## detect compiler
if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
	set ( REKAT_COMPILER "clang" )
elseif ( CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set ( REKAT_COMPILER "gcc" )
elseif ( CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set ( REKAT_COMPILER "visual_studio" )
else ( )
	message ( FATAL_ERROR "UNKNOWN COMPILER ${CMAKE_CXX_COMPILER_ID}" )
endif ( )

set ( REKAT_COMPILER_FLAGS_LOCATION "${CMAKE_SOURCE_DIR}/cmake/compilers/${REKAT_OS}-${REKAT_COMPILER}.cmake" )
message ( STATUS "os: ${REKAT_OS}, compiler: ${REKAT_COMPILER} -> ${REKAT_COMPILER_FLAGS_LOCATION}" )

## import compiler flags
include ( "${REKAT_COMPILER_FLAGS_LOCATION}" )

## Extra windows libraries / glad
if ( NOT WIN32 )
	set ( GLAD_LIBRARIES dl )
else ( )
	set ( WINSOCK_LIBRARIES Ws2_32.lib Mswsock.lib AdvApi32.lib )
endif ( )


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
