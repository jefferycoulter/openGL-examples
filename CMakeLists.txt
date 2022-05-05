cmake_minimum_required( VERSION 3.16 )

# project name
project( OPENGL_EXAMPLES )

# cmake settings
set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

# glfw settings
set( GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE )
set( GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE )
set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE )

# warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# other directories with cmake files
add_subdirectory( inc/glad )
add_subdirectory( inc/glfw )

# source code
FILE( GLOB src_files src/*.cpp )

add_executable( OPENGL_EXAMPLES ${src_files} )

# link other libraries
target_link_libraries( OPENGL_EXAMPLES
	PUBLIC
		GLAD
		glfw # glfw needs to be lowercase
)

# include directories
target_include_directories( OPENGL_EXAMPLES
	PRIVATE
		inc/glad/include
		inc/GLFW/include
)