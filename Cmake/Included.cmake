message("balls last")

## remove examples
set(OPENAL_BUILD_EXAMPLES OFF)
set(ALSOFT_UTILS OFF)
set(ALSOFT_EXAMPLES OFF)
set(ALSOFT_INSTALL OFF)

set(LIBTYPE STATIC)
add_subdirectory(Code/Lib/openal-soft)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(Code/Lib/glfw)

add_subdirectory(Code/Lib/freetype)

set(BOOST_INCLUDE_LIBRARIES config dll)
add_subdirectory(Code/Lib/boost)	

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
			Code/Engine/Include/
			Code/Lib/glad/include/
			Code/Lib/freetype/include/
			Code/Lib/glfw/include/
			Code/Lib/glm/
			Code/Lib/stb/
			Code/Lib/openal-soft/include/
			Code/Lib/boost/libs/dll/include/
		)
file(GLOB LIBS_SOURCES		
				Code/Lib/glad/src/glad.c
				Code/Include/Synth/FFT/*.cpp
				Code/Engine/Source/*.cpp
		)
file(GLOB PROJECT_HEADERS	
				Code/Engine/Include/*.hpp
				Code/Engine/Include/*.h
		)
file(GLOB PROJECT_CONFIGS	CMakeLists.txt
				Readme.md
				.gitattributes
				.gitignore
				.gitmodules
		)

source_group("Include" FILES ${PROJECT_HEADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Libs" FILES ${LIBS_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
				-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

