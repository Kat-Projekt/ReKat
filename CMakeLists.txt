cmake_minimum_required ( VERSION 3.13 )

project ( ReKat )

option ( REKAT_TEST "Compile the tests" OFF )
option ( REKAT_PROJEKT "Compile the projekts" OFF )
option ( REKAT_LAUNCH "Removes the console window for projekts" ON )
option ( REKAT_COMPONENT "Builds all specified components" ON )
option ( REKAT_DEBUG "Activates DIAGNOSTIC for every file" OFF )
option ( REKAT_DEBUG_EXPANCE "Activates EXPANCE for every file" OFF )

set ( PROJEKTS Pong )
set ( COMPONENTS Comp_Test )

## function to copy the build targets of components to the EXECUTABLE dir
function ( component_package EXECUTABLE )
foreach ( COMPONENT IN LISTS ARGN )

        add_custom_command(
        	TARGET ${EXECUTABLE} POST_BUILD
        	COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:${COMPONENT}>
                $<TARGET_FILE_DIR:${EXECUTABLE}>
        )

endforeach()
endfunction ( )

function ( test TEST )
	file ( GLOB TEST_SOURCES ${CMAKE_SOURCE_DIR}/tests/${TEST}/main.cpp )
	add_executable( ${TEST}	${TEST_SOURCES} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
				${PROJECT_CONFIGS}  ${LIBS_SOURCES})

	target_link_libraries ( ${TEST} ReKat )

	set_target_properties ( ${TEST} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests/${TEST} )

	set ( TEST_RESOURCES ${CMAKE_SOURCE_DIR}/tests/${TEST}/Resources )

	if ( EXISTS ${TEST_RESOURCES_DIR} )
		add_custom_command(
        		TARGET ${TEST} POST_BUILD
       			COMMAND ${CMAKE_COMMAND} -E copy_directory
			${TEST_RESOURCES}
			$<TARGET_FILE_DIR:${TEST}>
		)
	endif()

	component_package ( ${TEST} ${ARGN} )
endfunction ( )


list ( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" )
## this compiles RaKat shared library 
include ( library )

## Build components
if ( REKAT_COMPONENT )
	foreach ( COMP ${COMPONENTS} )
	add_library( ${COMP} SHARED ${CMAKE_SOURCE_DIR}/components/${COMP}/main.cpp )

	target_link_libraries( ${COMP} ReKat )

	set_target_properties ( ${COMP} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/components/
		LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/components/
		PREFIX "" 
	)

	endforeach( COMP )
endif ( )

# compile tests
if ( REKAT_TEST )
	test ( new_core_test Comp_Test )
endif ( )

#[[
if ( REKAT_PROJEKT )
foreach ( PROJEKT ${PROJEKTS} )
	file ( GLOB PROJECT_SOURCES ${CMAKE_SOURCE_DIR}/Projekt/${PROJEKT}/main.cpp )
	add_executable(${PROJEKT} ${PROJECT_SOURCES}  ${PROJECT_HEADERS}
					${PROJECT_CONFIGS}  ${LIBS_SOURCES} ${CMAKE_SOURCE_DIR}/Projekt/${PROJEKT}/favicon.rc)

	target_link_libraries ( ${PROJEKT} glfw freetype OpenAL # OpenAL::OpenAL
				${GLFW_LIBRARIES} ${GLAD_LIBRARIES} ${WINSOCK_LIBRARIES} )

	# removes console
	if(WIN32)
	set_target_properties(${PROJEKT}  
	PROPERTIES
		LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
		LINK_FLAGS_RELEASE "/SUBSYSTEM:windows /ENTRY:mainCRTStartup"
		LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:windows /ENTRY:mainCRTStartup"
		LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:windows /ENTRY:mainCRTStartup"
	)
	endif(WIN32)

	set_target_properties ( ${PROJEKT} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Projekt/${PROJEKT} )

	add_custom_command (
		TARGET ${PROJEKT} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Projekt/${PROJEKT}/Data $<TARGET_FILE_DIR:${PROJEKT}>/Data )

endforeach ( PROJEKT )
endif ( )
]]
