message("cok")

include ( Included )

add_library( ReKat SHARED ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${PROJECT_CONFIGS} ${LIBS_SOURCES} )
message ( ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${PROJECT_CONFIGS} ${LIBS_SOURCES} )

target_link_libraries( ReKat glfw freetype OpenAL Boost::config Boost::dll # Boost::filesystem Boost::system 

                       ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} ${WINSOCK_LIBRARIES} )

set_target_properties( ReKat PROPERTIES 
							 VERSION 0.1
							 RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Library
							 C_VISIBILITY_PRESET hidden
							 CXX_VISIBILITY_PRESET hidden
							 OUTPUT_NAME ReKat )

set_target_properties( ReKat PROPERTIES PUBLIC_HEADER Code/Include/engine.hpp )
# target_compile_options( ReKat PRIVATE -O3)
# set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# Export the library's public include path to dependent targets
target_include_directories( ReKat INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories( ReKat PRIVATE Code/Include/ )

include(GNUInstallDirs)

install(TARGETS ReKat
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
