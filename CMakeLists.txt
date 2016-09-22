# CMake version
cmake_minimum_required (VERSION 2.8)

# project
project (dtcurl)

LIST(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

# Library dependence
if(BUILD_FOR_ANDROID)
    #setup curl android env
else()
    FIND_PACKAGE(CURL  REQUIRED)
    INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
endif()
# Target - libdtcurl.so
INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(src)
AUX_SOURCE_DIRECTORY(src SRC_CURL)
ADD_LIBRARY(dtcurl SHARED ${SRC_CURL})
TARGET_LINK_LIBRARIES(dtcurl curl)

if(BUILD_FOR_ANDROID)
    MESSAGE("Android pthread prebuild to libc")
else()
    TARGET_LINK_LIBRARIES(dtcurl pthread)
endif()

# Target - test
INCLUDE_DIRECTORIES(include)
SET(SRC_TEST test/curl_test.c)
ADD_EXECUTABLE(test test/curl_test.c)
TARGET_LINK_LIBRARIES(test dtcurl)


if(BUILD_FOR_ANDROID)
    MESSAGE("Android Can Not Install")
else()
    INSTALL(TARGETS dtcurl
        #RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )
INSTALL(DIRECTORY include/ DESTINATION /usr/local/include/${PROJECT_NAME})
endif()
