cmake_minimum_required(VERSION 3.22)
project( DisplayImage )
# path to all bins
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( DisplayImage laba4.cpp )
target_link_libraries( DisplayImage ${OpenCV_LIBS} )
# copy assets to bin folder
add_custom_target(
    copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/images"
        "${CMAKE_BINARY_DIR}/bin"
    COMMENT "Copy assets from ${PROJECT_SOURCE_DIR}/assets to ${CMAKE_BINARY_DIR}/bin"
)
add_dependencies( DisplayImage copy_assets)
