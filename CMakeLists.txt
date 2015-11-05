# CMake version
cmake_minimum_required (VERSION 2.8)

# project
project (dtcurl)

# Library dependence
FIND_PACKAGE(CURL  REQUIRED)
INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})

# Target - libdtcurl.so
INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(src)
AUX_SOURCE_DIRECTORY(src SRC_CURL)
ADD_LIBRARY(dtcurl SHARED ${SRC_CURL})
TARGET_LINK_LIBRARIES(dtcurl curl)
TARGET_LINK_LIBRARIES(dtcurl pthread)

# Target - test
INCLUDE_DIRECTORIES(include)
SET(SRC_TEST test/curl_test.c)
ADD_EXECUTABLE(test test/curl_test.c)
TARGET_LINK_LIBRARIES(test dtcurl)


INSTALL(TARGETS dtcurl
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    )

install(DIRECTORY include/ DESTINATION /usr/local/include/${PROJECT_NAME})
