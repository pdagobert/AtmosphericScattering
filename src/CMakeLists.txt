file( GLOB_RECURSE source_files *.cpp )

include_directories( Library C:/libraries/include )
link_directories( C:/libraries/lib )

add_executable( ${app_name} ${source_files} )

# TODO: add linux support
if( WIN32 )
	target_link_libraries( ${app_name} SOIL glfw3dll opengl32 glew32 )
else()
	target_link_libraries( ${app_name} glfw GL GLEW pthread )
endif()