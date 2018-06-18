# - Find EV library
# Once done this will define
#
#  EV_FOUND - This defines if we found EV
#  EV_INCLUDE_DIR - EV include directory
#  EV_LIBS - EV libraries
#  EV_DEFINITIONS - Compiler switches required for EV


# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
#FIND_PACKAGE(PkgConfig)
#PKG_SEARCH_MODULE(PC_LIBEV libEV)

SET(EV_DEFINITIONS ${PC_EV_CFLAGS_OTHER})

FIND_PATH(
    EV_INCLUDE_DIR
    NAMES "ev.h"
    PATH_SUFFIXES ""
)

FIND_LIBRARY(EV_LIBS NAMES ev
   HINTS
   ${PC_EV_LIBDIR}
   ${PC_EV_LIBRARY_DIRS}
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(EV DEFAULT_MSG EV_LIBS EV_INCLUDE_DIR)
