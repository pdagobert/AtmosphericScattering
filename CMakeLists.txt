cmake_minimum_required( VERSION 3.2.0 )

set( app_name "demo" )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin )
set( CMAKE_CXX_STANDARD 14 )

add_compile_options( -W -Wall )

project( project )
add_subdirectory( src )