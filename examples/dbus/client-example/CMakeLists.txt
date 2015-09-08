CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(client-example)

INCLUDE(FindPkgConfig)

PKG_CHECK_MODULES(DEPS
                  dbus-1
                  dpl-efl
                  dpl-dbus-efl
                  REQUIRED)

SET(TARGET_NAME "client-example")

SET(SRCS
    client-example.cpp)

INCLUDE_DIRECTORIES(${DEPS_INCLUDE_DIRS})

ADD_DEFINITIONS("-std=c++0x")
ADD_DEFINITIONS("-DDPL_LOGS_ENABLED")

ADD_EXECUTABLE(${TARGET_NAME} ${SRCS})
TARGET_LINK_LIBRARIES(${TARGET_NAME} ${DEPS_LIBRARIES})

