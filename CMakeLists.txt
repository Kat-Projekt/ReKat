cmake_minimum_required ( VERSION 3.13 )

project ( ReKat )

option ( REKAT_TEST "Compile the tests" OFF )
option ( REKAT_LAUNCH "Removes the console window for projekts" ON )
option ( REKAT_COMPONENT "Builds all specified components" ON )
option ( REKAT_DEBUG "Activates DIAGNOSTIC for every file" OFF )
option ( REKAT_DEBUG_EXPANCE "Activates EXPANCE for every file" OFF )

set ( PROJEKTS Pong )
set ( COMPONENTS Comp_Test )

# pack_components ( <EXECUTABLE> <COMPONENTS...> )
#
# Arguments:
# 	<EXECUTABLE>    Name of the target.
# 	<COMPONENTS...> Name of components to pack with target, or ALL_COMPONENTS.
#
# Examples:
# 	pack_components ( MyProject Sprite NewComponent Player )
# 	pack_components ( MyProject ALL_COMPONENTS )
function ( component_package EXECUTABLE )
	## set ( COMPONETDEF "{ " )
	foreach ( COMPONENT ${ARGN} )
		message ( VERBOSE "adding ${COMPONENT} to: ${EXECUTABLE}" )
		## list ( APPEND COMPONETDEF "\"${COMPONENT}\", ")
		add_custom_command(
			TARGET ${EXECUTABLE} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<TARGET_FILE:${COMPONENT}>
			$<TARGET_FILE_DIR:${EXECUTABLE}>
		)

	endforeach ( COMPONENT )
	
	## list ( APPEND COMPONETDEF "}")

	## add definitions for including
	## target_compile_definitions( ReKat PUBLIC COMPONENTS=COMPONETDEF )
	## message( ${COMPONETDEF} )
endfunction ( )

# KAT_project ( <TARGET> <PATH> <COMPONENTS...> )
#
# Arguments:
# 	<TARGET>        Name of the projekt.
# 	<PATH>          Where to find the projekt root. note that main.cpp is expected to be in that PATH
# 	<COMPONENTS...> Name of components to pack with the projekt, or ALL_COMPONENTS.
#
# Examples:
# 	KAT_project ( MyProject PATH ${CMAKE_SOURCE_DIR} COMPONENTS Sprite NewComponent Player )
# 	KAT_project ( Sample_projekt PATH ${CMAKE_SOURCE_DIR}/example ALL_COMPONENTS )
function ( KAT_project TARGET )
	set ( options ALL_COMPONENTS )
	set ( oneValueArgs PATH )
	set ( multiValueArgs ADD_COMPONENTS )

	cmake_parse_arguments (
		KAT
		${options}
		${oneValueArgs}
		${multiValueArgs}
		${ARGN}
	)

	if ( DEFINED KAT_PATH )
		file ( GLOB TEST_SOURCE ${KAT_PATH}/main.cpp )
	else ( )
		file ( GLOB TEST_SOURCE ${CMAKE_SOURCE_DIR}/tests/${TARGET}/main.cpp )
	endif ( )

	add_executable( ${TARGET}	${TEST_SOURCE} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
				${PROJECT_CONFIGS}  ${LIBS_SOURCES})

	target_link_libraries ( ${TARGET} ReKat )

	set_target_properties ( ${TARGET} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests/${TARGET} )

	set ( TEST_RESOURCES ${CMAKE_SOURCE_DIR}/tests/${TARGET}/Resources )

	if ( EXISTS ${TEST_RESOURCES_DIR} )
		add_custom_command(
        		TARGET ${TARGET} POST_BUILD
       			COMMAND ${CMAKE_COMMAND} -E copy_directory
			${TEST_RESOURCES}
			$<TARGET_FILE_DIR:${TARGET}>
		)
	endif()

	if ( KAT_ALL_COMPONENTS )
		message ( STATUS "${TARGET} requres ALL_COMPONENTS" )
		component_package ( ${TARGET} ${COMPONENTS_LIST} )
	else ( )
		message ( STATUS "${TARGET} requres ${KAT_ADD_COMPONENTS}" )
		component_package ( ${TARGET} ${KAT_ADD_COMPONENTS} )
	endif ( )
endfunction ( )

# component ( <COMP> <PATH> )
#
# Arguments:
# 	<COMP> Name of the component.
# 	<PATH> Where to find the component root.
function ( component COMP PATH )
	configure_file(
		${CMAKE_SOURCE_DIR}/cmake/component_wrapper.cpp.in
		${CMAKE_BINARY_DIR}/generated/${COMP}_wrapper.cpp
		@ONLY
	)

	add_library ( ${COMP} SHARED ${CMAKE_BINARY_DIR}/generated/${COMP}_wrapper.cpp )

	target_link_libraries ( ${COMP} ReKat )

	set_target_properties ( ${COMP} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/components/
		LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/components/
		PREFIX "" 
	)
endfunction ( )

list ( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" )
## this compiles RaKat shared library 
include ( library )

set ( COMPONENTS_LIST )
## build components
if ( REKAT_COMPONENT )
	# first the compoents in /component/
	file ( GLOB COMPONENTS
		${CMAKE_SOURCE_DIR}/components/*.cpp
	)
	# then the additional ones
	foreach ( DIR ${ADDITIONAL_COMPONENTS_DIRS} )
		file ( GLOB EXTRA_COMPONENTS
			${DIR}/*.cpp
		)
		foreach ( COMP ${EXTRA_COMPONENTS} )
		list ( APPEND COMPONENTS "${DIR}/${COMP}" )
		endforeach()
	endforeach()

	foreach ( PATH ${COMPONENTS} )
		cmake_path ( GET PATH STEM COMP )
		list ( APPEND COMPONENTS_LIST ${COMP} )
		message( STATUS "Building '${COMP}' from: ${PATH}" )
		component (
			${COMP}
			${PATH}
		)
	endforeach( PATH )
endif ( )

# compile tests
if ( REKAT_TEST )
	KAT_project ( new_core_test ALL_COMPONENTS )
	KAT_project ( transform_verify )
endif ( )
