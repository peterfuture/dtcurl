# CMake version
cmake_minimum_required (VERSION 2.8)

# project
project (DTCURL)

# Library dependence
FIND_PACKAGE(CURL  REQUIRED)
INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})

# target - lib
INCLUDE_DIRECTORIES(src)
AUX_SOURCE_DIRECTORY(src SRC_CURL)
#ADD_LIBRARY(dtcurl_shared SHARED ${SRC_CURL})
ADD_LIBRARY(dtcurl_static STATIC ${SRC_CURL})
TARGET_LINK_LIBRARIES(dtcurl_static curl)
TARGET_LINK_LIBRARIES(dtcurl_static pthread)

# target - test
SET(SRC_TEST test/curl_test.c )
ADD_EXECUTABLE(test test/curl_test.c)
TARGET_LINK_LIBRARIES(test dtcurl_static)
