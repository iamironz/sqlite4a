cmake_minimum_required(VERSION 3.5)
project(sqlite3x)

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fvisibility=hidden")

file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/src/*.h)
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cc)

if (NOT CMAKE_SYSTEM_NAME STREQUAL Android)
    if (DEFINED ANDROID_TOOLCHAIN)
        include_directories(${ANDROID_TOOLCHAIN}/sysroot/usr/include)
    endif ()
endif ()

add_subdirectory(sqlite3)
include_directories(${sqlite3_SOURCE_DIR})
add_library(sqlite3x SHARED ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} sqlite3 log)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}")