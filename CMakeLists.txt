cmake_minimum_required ( VERSION 3.13 )

project ( ReKat )

option ( REKAT_LIBRARY "Compile the Engine as a library, else the headers will be included in the executable" OFF )
option ( REKAT_TEST "Compile the test suite" OFF )
option ( REKAT_PROJEKT "Compile the projekts" OFF )
option ( REKAT_ENGINE "Compile the Engine interpreter" OFF )
option ( REKAT_LAUNCH "Removes the console window" ON )

# unicode text
add_definitions(-DUNICODE -D_UNICODE)

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
set ( TESTS Render_Test Core_Test Collision_Test )
set ( TESTS Multi_Window_Test Button_Test )
set ( TESTS Core_Test )

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

set ( PROJEKTS Wordle )

if ( REKAT_PROJEKT )

    foreach ( PROJEKT ${PROJEKTS} )
    file ( GLOB PROJECT_SOURCES ${CMAKE_SOURCE_DIR}/Projekt/${PROJEKT}/main.cpp )
    add_executable(${PROJEKT}   ${PROJECT_SOURCES}  ${PROJECT_HEADERS}
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

if ( REKAT_ENGINE )
    file ( GLOB PROJECT_SOURCES ${CMAKE_SOURCE_DIR}/Src/main.cpp )
    add_executable(Engine	    ${PROJECT_SOURCES}  ${PROJECT_HEADERS}
                                ${PROJECT_CONFIGS}  ${LIBS_SOURCES})

    target_link_libraries ( Engine glfw freetype OpenAL # OpenAL::OpenAL
                        ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} ${WINSOCK_LIBRARIES} )

    set_target_properties ( Engine PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Engine_Interpreter )

endif ( )