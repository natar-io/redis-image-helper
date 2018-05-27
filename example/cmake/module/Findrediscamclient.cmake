# - Find rediscamclient library
# Once done this will define
#
#  REDISCAMCLIENT_FOUND - This defines if we found hiredis
#  REDISCAMCLIENT_INCLUDE_DIR - hiredis include directory
#  REDISCAMCLIENT_LIBS - hiredis libraries
#  REDISCAMCLIENT_DEFINITIONS - Compiler switches required for hiredis

SET(REDISCAMCLIENT_DEFINITIONS ${PC_REDISCAMCLIENT_CFLAGS_OTHER})

FIND_PATH(
    REDISCAMCLIENT_INCLUDE_DIR
    NAMES "RedisCameraClient.hpp"
    PATH_SUFFIXES "rediscamclient"
)

FIND_LIBRARY(REDISCAMCLIENT_LIBS NAMES rediscamclient
   HINTS
   ${PC_REDISCAMCLIENT_LIBDIR}
   ${PC_REDISCAMCLIENT_LIBRARY_DIRS}
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(rediscamclient DEFAULT_MSG REDISCAMCLIENT_LIBS REDISCAMCLIENT_INCLUDE_DIR)
