cmake_minimum_required( VERSION 3.16 )

# project name
project( OPENGL_EXAMPLES )

# set output directory
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/examples )

# cmake settings
set( CMAKE_CXX_STANDARD 17 )
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

# macro to loop through example directories
macro(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        SET(dirlist ${dirlist} ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
endmacro()

# get a list of the examples (subdirectories in src)
SUBDIRLIST(subdirs ${CMAKE_SOURCE_DIR}/src)

# create executable for each example
foreach(subdir ${subdirs})

    # source code
    FILE( GLOB src_files "src/${subdir}/*.cpp" )
    FILE( GLOB util_files "src/${subdir}/resources/*.h" "src/${subdir}/resources/*.cpp")

    # get the name for the executable
    set(path "${src_files}")
    cmake_path(GET path STEM executable_name)

    # add the executable
    add_executable(${executable_name} ${src_files} ${util_files})

    # link other libraries
    target_link_libraries( ${executable_name}
    PUBLIC
        GLAD
        glfw # glfw needs to be lowercase
    )

    # include directories
    target_include_directories( ${executable_name}
    PUBLIC
        src/${subdir}/resources
        src/${subdir}/resources/shaders
    PRIVATE
        inc/glad/include
        inc/GLFW/include
    )

endforeach(subdir ${subdirs})