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

find_package(rediscamclient REQUIRED)
if (NOT REDISCAMCLIENT_FOUND)
	message(FATAL_ERROR, "rediscamclient not found")
endif()

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories($(/usr/local/include/gstreamer-1.0) /gst)
message(${REDISCAMCLIENT_INCLUDE_DIR})
include_directories(${REDISCAMCLIENT_INCLUDE_DIR})

set(GLOB HEADERS include/*.hpp)	

set(CLI_SOURCES
        src/client.cpp)

set(SERV_SOURCES
        src/RedisCameraServer.cpp
        src/server.cpp)

add_executable (rcamcli ${CLI_SOURCES} ${HEADERS})
target_link_libraries (rcamcli rediscamclient ${HIREDIS_LIBS} ${OpenCV_LIBS})

add_executable (rcamserv ${SERV_SOURCES} ${HEADERS})
target_link_libraries(rcamserv rediscamclient ${HIREDIS_LIBS} ${OpenCV_LIBS})
