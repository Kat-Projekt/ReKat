## Include the CATCH FRAMEWORK
set(CATCH_BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(CATCH_INSTALL_DOCS OFF CACHE BOOL "" FORCE)
set(CATCH_INSTALL_EXTRAS OFF CACHE BOOL "" FORCE)

add_subdirectory ( ${CMAKE_SOURCE_DIR}/tests/Catch2 )

## For v3
list ( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/tests/Catch2/extras )

include ( CTest )
include ( Catch )

## function for testing equal to KAT_projekt
function(KAT_test TARGET)
	set ( options ALL_COMPONENTS )
	set ( oneValueArgs PATH RESOURCES_PATH OUTPUT_PATH )
	set ( multiValueArgs ADD_COMPONENTS ADD_COMPONENTS_PATHS )

	cmake_parse_arguments(
		PARSE_ARGV 1
		KAT
		"${options}"
		"${oneValueArgs}"
		"${multiValueArgs}"
	)

	## Get Source path for test
	if ( DEFINED KAT_PATH )
		set ( TARGET_PATH "${KAT_PATH}/main.cpp" )
	else ( )
		set ( TARGET_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tests/${TARGET}/main.cpp" )
	endif ( )

	if ( NOT EXISTS "${TARGET_PATH}" )
		message ( FATAL_ERROR "file: ${TARGET_PATH} does not exist" )
	endif ( )

	message ( STATUS "Building test: '${TARGET}' from ${TARGET_PATH}" )
	file(GLOB TARGET_SOURCE ${TARGET_PATH} )

	add_executable( ${TARGET} ${TARGET_SOURCE} ${PROJECT_SOURCES}
		${PROJECT_HEADERS} ${PROJECT_CONFIGS} ${LIBS_SOURCES} )

	target_link_libraries ( ${TARGET} ReKat Catch2::Catch2WithMain )
	
	## Output directory
	if ( DEFINED KAT_OUTPUT_PATH )
		set_target_properties ( ${TARGET} PROPERTIES
			RUNTIME_OUTPUT_DIRECTORY ${KAT_OUTPUT_PATH} )
	else ( )
		set_target_properties ( ${TARGET} PROPERTIES
			RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests/${TARGET} )
	endif ( )

	## Resource path
	if ( DEFINED KAT_RESOURCES_PATH )
		set ( TEST_RESOURCES ${KAT_RESOURCES_PATH} )
	else ( )
		set ( TEST_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/${TARGET}/Resources )
	endif ( )

	if ( EXISTS ${TEST_RESOURCES} )
		add_custom_command(
			TARGET ${TARGET} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			${TEST_RESOURCES}
			$<TARGET_FILE_DIR:${TARGET}>
		)
	endif ( )

	## Add eventuall extra components
	if ( DEFINED KAT_ADD_COMPONENTS_PATHS )
		file ( GLOB COMPONENTS ${KAT_ADD_COMPONENTS_PATHS}/*.cpp )

		foreach ( PATH ${COMPONENTS} )
			cmake_path ( GET PATH STEM COMP )
			list ( APPEND COMPONENTS_LIST ${COMP} )
			list ( APPEND KAT_ADD_COMPONENTS ${COMP} )

			message( STATUS "Building '${COMP}' from: ${PATH}" )
			component (
				${COMP}
				${PATH}
			)
		endforeach( PATH )
	endif ( )

	if ( KAT_ALL_COMPONENTS )
		message ( STATUS "${TARGET} requres ALL_COMPONENTS" )
		component_package ( ${TARGET} ${COMPONENTS_LIST} )
	else ( )
		message ( STATUS "${TARGET} requres ${KAT_ADD_COMPONENTS}" )
		component_package ( ${TARGET} ${KAT_ADD_COMPONENTS} )
	endif ( )

	## Ctest discorver
	catch_discover_tests (
		${TARGET}
		WORKING_DIRECTORY
		"$<TARGET_FILE_DIR:${TARGET}>"
	)
endfunction ( )

