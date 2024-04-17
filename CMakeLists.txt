cmake_minimum_required(VERSION 3.22)
project( DisplayImage )

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK2 REQUIRED gtk+-2.0)

# Include directories
include_directories(${GTK2_INCLUDE_DIRS})

# Link directories
link_directories(${GTK2_LIBRARY_DIRS})

# path to all bins
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -fsanitize=address -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -fsanitize=address -g")
endif()

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( DisplayImage laba4.cpp )
target_link_libraries( DisplayImage ${OpenCV_LIBS} ${GTK2_LIBRARIES})
# copy assets to bin folder
add_custom_target(
    copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/images"
        "${CMAKE_BINARY_DIR}/bin"
    COMMENT "Copy assets from ${PROJECT_SOURCE_DIR}/assets to ${CMAKE_BINARY_DIR}/bin"
)
add_dependencies( DisplayImage copy_assets)
