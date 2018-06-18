cmake_minimum_required (VERSION 3.2)
project(redisimagehelper VERSION 0.1)

set (CMAKE_CXX_STANDARD 11)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake/module)

find_package(hiredis REQUIRED)
if (NOT HIREDIS_FOUND)
    message(FATAL_ERROR "hiredis not found.")
endif()

find_package(ev REQUIRED)
if (NOT EV_FOUND)
    message(FATAL_ERROR "ev not found.")
endif()

find_package(curlpp)
if (NOT CURLPP_FOUND)
    message("Warning: cURLpp not found.")
endif()

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include )
include_directories($(/usr/local/include/gstreamer-1.0) /gst)

file(GLOB HEADERS include/*.hpp)

file(GLOB LIBRARY_SOURCES
    src/RedisImageHelper.cpp)
file(GLOB LIBRARY_HEADERS
    include/Image.hpp
    include/RedisImageHelper.hpp)

#Used to have access to CMAKE_INSTALL_*DIR
include(GNUInstallDirs)
add_library(redisimagehelper SHARED ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
set_target_properties(redisimagehelper PROPERTIES VERSION 0.1
                                                  SOVERSION 1)
set_target_properties(redisimagehelper PROPERTIES PUBLIC_HEADER "${LIBRARY_HEADERS}")
# This line will find the .pc.in tempalte file and replace @variable with their equivalent here
# For example in redisimagehelper.pc.in the variable @PROJECT_NAME@ will be replaced with
configure_file(redisimagehelper.pc.in redisimagehelper.pc @ONLY)

install(TARGETS redisimagehelper
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/redisimagehelper)

install(FILES ${CMAKE_BINARY_DIR}/redisimagehelper.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

#add_subdirectory(example)
