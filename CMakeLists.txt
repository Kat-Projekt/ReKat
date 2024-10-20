cmake_minimum_required ( VERSION 3.13 )

project ( ReKat )

option ( REKAT_LIBRARY "Compile the Engine as a library, else the headers will be included in the executable" OFF )
option ( REKAT_TEST "Compile the test suite" ON )

list ( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/Cmake" )

# compile the libray
if ( REKAT_LIBRARY )
    message ( "Creating library" )
    include ( Library )
else ( )
    message ( "Linking filling" )
    include ( Included )
endif ( )

# compile the examples
set ( TESTS Render_Test Core_Test Text_Display_Test Button_Test Collision_Test Pong_Test Phisiks_Test )
set ( TESTS Pong_Test )

if ( REKAT_TEST )
    foreach ( TEST ${TESTS} )
    file ( GLOB PROJECT_SOURCES ${CMAKE_SOURCE_DIR}/Test/${TEST}/main.cpp )
    add_executable(${TEST}	    ${PROJECT_SOURCES}  ${PROJECT_HEADERS}
                                ${PROJECT_CONFIGS}  ${LIBS_SOURCES})

    target_link_libraries ( ${TEST} glfw freetype OpenAL # OpenAL::OpenAL
                        ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} ${WINSOCK_LIBRARIES} )

    set_target_properties ( ${TEST} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Test/${TEST} )

    add_custom_command (
        TARGET ${TEST} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Test/${TEST}/Resources $<TARGET_FILE_DIR:${TEST}> )

    endforeach ( TEST )
endif ( )