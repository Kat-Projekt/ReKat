cmake_minimum_required(VERSION 3.13)

project(ReKat)

## remove examples
option(OPENAL_BUILD_EXAMPLES OFF)
add_subdirectory(Code/Lib/openal-soft)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(Code/Lib/glfw)

add_subdirectory(Code/Lib/freetype)

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /std:c++17")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++17")
	if(NOT WIN32)
		set(GLAD_LIBRARIES dl)
	else()
		set(WINSOCK_LIBRARIES Ws2_32.lib Mswsock.lib AdvApi32.lib)
	endif()
endif()

include_directories(Code/Include/
					Code/Lib/glad/include/
					Code/Lib/freetype/include/
					Code/Lib/glfw/include/
					Code/Lib/glm/
					Code/Lib/stb/
					Code/Lib/openal-soft/include )
file(GLOB LIBS_SOURCES	Code/Lib/glad/src/glad.c
						Code/Include/Synth/FFT/*.cpp)
file(GLOB PROJECT_HEADERS Code/Include/*.hpp
						  Code/Inlcude/*.h)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
						  Readme.md
						 .gitattributes
						 .gitignore
						 .gitmodules)
source_group("Include" FILES ${PROJECT_HEADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Libs" FILES ${LIBS_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
				-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

set(PROJEKTS Clique)

foreach(PROJEKT ${PROJEKTS})
	file(GLOB PROJECT_SOURCES Projekts/${PROJEKT}/main.cpp )
	add_executable(${PROJEKT}	${PROJECT_SOURCES} ${PROJECT_HEADERS}
	 							${PROJECT_CONFIGS} ${LIBS_SOURCES})

	target_link_libraries(${PROJEKT} glfw freetype OpenAL::OpenAL
						${GLFW_LIBRARIES} ${GLAD_LIBRARIES} ${WINSOCK_LIBRARIES} )

	set_target_properties(${PROJEKT} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Projekts/${PROJEKT})

	add_custom_command(
		TARGET ${PROJEKT} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Projekts/${PROJEKT}/Resources $<TARGET_FILE_DIR:${PROJEKT}>)
endforeach(PROJEKT)



