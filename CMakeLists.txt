cmake_minimum_required (VERSION 3.2)
project(rediscamclient)

set (CMAKE_CXX_STANDARD 11)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake/module)

find_package(OpenCV REQUIRED core opencv_imgcodecs opencv_highgui opencv_imgproc opencv_videoio)
if (NOT OPENCV_FOUND)
        message(FATAL_ERROR, "OpenCV not found.")
endif()

find_package(hiredis REQUIRED)
if (NOT HIREDIS_FOUND)
    message(FATAL_ERROR, "hiredis not found.")
endif()

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include )
include_directories($(/usr/local/include/gstreamer-1.0) /gst)

file(GLOB HEADERS include/*.hpp)

file(GLOB LIBRARY_SOURCES
    src/RedisCameraClient.cpp)
file(GLOB LIBRARY_HEADERS
    include/CameraFrame.hpp
    include/RedisCameraClient.hpp)

#Used to have access to CMAKE_INSTALL_*DIR
include(GNUInstallDirs)
add_library(rediscamclient SHARED ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
set_target_properties(rediscamclient PROPERTIES VERSION 0.1
                                                SOVERSION 1)
set_target_properties(rediscamclient PROPERTIES PUBLIC_HEADER "${LIBRARY_HEADERS}")
# This line will find the .pc.in tempalte file and replace @variable with their equivalent here
# For example in rediscamclient.pc.in the variable @PROJECT_NAME@ will be replaced with
configure_file(rediscamclient.pc.in rediscamclient.pc @ONLY)

install(TARGETS rediscamclient
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/rediscamclient)

install(FILES ${CMAKE_BINARY_DIR}/rediscamclient.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
