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

## unicode text
add_definitions(-DUNICODE -D_UNICODE)

## compiler specific flags for debugging
if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /std:c++17")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Weffc++ -std=c++17")
	if(NOT WIN32)
		set(GLAD_LIBRARIES dl)
	else()
		set(WINSOCK_LIBRARIES Ws2_32.lib Mswsock.lib AdvApi32.lib)
	endif()
endif()

include_directories(	
			include/
			libraries/glad/include/
			libraries/freetype/include/
			libraries/glfw/include/
			libraries/glm/
			libraries/stb/
			libraries/openal-soft/include/
		)

file(GLOB LIBS_SOURCES libraries/glad/src/glad.c )
file(GLOB PROJECT_SOURCES source/*.cpp )
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
