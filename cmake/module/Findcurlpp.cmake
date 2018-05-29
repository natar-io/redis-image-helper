# - Find CURLPP library
# Once done this will define
#
#  CURLPP_FOUND - This defines if we found CURLPP
#  CURLPP_INCLUDE_DIR - CURLPP include directory
#  CURLPP_LIBS - CURLPP libraries
#  CURLPP_DEFINITIONS - Compiler switches required for CURLPP


# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
#FIND_PACKAGE(PkgConfig)
#PKG_SEARCH_MODULE(PC_LIBCURLPP libCURLPP)

SET(CURLPP_DEFINITIONS ${PC_CURLPP_CFLAGS_OTHER})

FIND_PATH(
    CURLPP_INCLUDE_DIR
    NAMES "cURLpp.hpp"
    PATH_SUFFIXES "curlpp"
)

FIND_LIBRARY(CURLPP_LIBS NAMES curlpp
   HINTS
   ${PC_CURLPP_LIBDIR}
   ${PC_CURLPP_LIBRARY_DIRS}
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CURLPP DEFAULT_MSG CURLPP_LIBS CURLPP_INCLUDE_DIR)
