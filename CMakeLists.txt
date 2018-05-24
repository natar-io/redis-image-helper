cmake_minimum_required (VERSION 3.2)
project (REDISCAM)

set (CMAKE_CXX_STANDARD 11)

find_package( OpenCV REQUIRED core opencv_imgcodecs opencv_highgui opencv_imgproc opencv_videoio)
if (NOT OPENCV_FOUND)
        message(FATAL_ERROR, "OpenCV not found.")
endif()

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories($(/usr/local/include/gstreamer-1.0) /gst)

file (GLOB SOURCES src/*.cpp)
file (GLOB HEADERS include/*.hpp)

set (CLI_SOURCES
        src/RedisCameraClient.cpp
        src/RedisCameraServer.cpp
        src/client.cpp)

set (SERV_SOURCES
        src/RedisCameraClient.cpp
        src/RedisCameraServer.cpp
        src/server.cpp)

file (GLOB LIBRARY_SOURCES
    src/RedisCameraClient.cpp)
file (GLOB LIBRARY_HEADERS
    include/RedisCameraClient.hpp)

add_executable (rcamcli ${CLI_SOURCES} ${HEADERS})
target_link_libraries (rcamcli hiredis ${OpenCV_LIBS})

add_executable (rcamserv ${SERV_SOURCES} ${HEADERS})
target_link_libraries(rcamserv hiredis ${OpenCV_LIBS})

#add_library(rediscamclient SHARED ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
#set_target_properties(rediscamclient PROPERTIES VERSION 0.1
#                                                SOVERSION 1
#                                                PUBLIC_HEADER include/RedisCamClient.hpp)
