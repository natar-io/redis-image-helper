# - Find hiredis library
# Once done this will define
#
#  HIREDIS_FOUND - This defines if we found hiredis
#  HIREDIS_INCLUDE_DIR - hiredis include directory
#  HIREDIS_LIBS - hiredis libraries
#  HIREDIS_DEFINITIONS - Compiler switches required for hiredis


# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
#FIND_PACKAGE(PkgConfig)
#PKG_SEARCH_MODULE(PC_LIBHIREDIS libhiredis)

SET(HIREDIS_DEFINITIONS ${PC_HIREDIS_CFLAGS_OTHER})

FIND_PATH(
    HIREDIS_INCLUDE_DIR
    NAMES "hiredis.h"
    PATH_SUFFIXES "hiredis"
)

FIND_LIBRARY(HIREDIS_LIBS NAMES hiredis
   HINTS
   ${PC_HIREDIS_LIBDIR}
   ${PC_HIREDIS_LIBRARY_DIRS}
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(hiredis DEFAULT_MSG HIREDIS_LIBS HIREDIS_INCLUDE_DIR)
