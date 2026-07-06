include( includes )

add_library( ReKat SHARED ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${PROJECT_CONFIGS} ${LIBS_SOURCES} )

## for glfw and glad
target_compile_definitions( ReKat PRIVATE GLFW_INCLUDE_NONE )

## for debugging symbols
if ( REKAT_DEBUG )
	target_compile_definitions( ReKat PUBLIC DIAGNOSTIC )

	if ( REKAT_DEBUG_EXPANCE )
		target_compile_definitions( ReKat PUBLIC EXPANCE )

		message( STATUS "Entering in Expanded Debug mode" )
	else ( )
		message( STATUS "Entering in Debug mode" )
	endif( )
else ( )
	message( STATUS "Engine will be optimized" )
	target_compile_options( ReKat PRIVATE -O3 )
	set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ( )

target_link_libraries( ReKat glfw freetype OpenAL Boost::config Boost::dll 
                       ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} ${WINSOCK_LIBRARIES} )

set_target_properties(	ReKat PROPERTIES 
			VERSION 2.0
			LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/
			C_VISIBILITY_PRESET hidden
			CXX_VISIBILITY_PRESET hidden
			OUTPUT_NAME ReKat 
			PREFIX "" )

set_target_properties( ReKat PROPERTIES PUBLIC_HEADER Include/engine.hpp )

## export the library's public include path to dependent targets
target_include_directories( ReKat PUBLIC include )

## for library installation
include(GNUInstallDirs)
install(TARGETS ReKat
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ReKat
)