CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(server-example)

INCLUDE(FindPkgConfig)

PKG_CHECK_MODULES(DEPS
                  glib-2.0
                  gio-2.0
                  dpl-efl
                  dpl-dbus-efl
                  REQUIRED)

SET(TARGET_NAME "server-example")

SET(SRCS
    server-example.cpp)

INCLUDE_DIRECTORIES(${DEPS_INCLUDE_DIRS})

ADD_DEFINITIONS("-std=c++0x")
ADD_DEFINITIONS("-pedantic")
ADD_DEFINITIONS("-Wall")
ADD_DEFINITIONS("-DDPL_LOGS_ENABLED")

ADD_EXECUTABLE(${TARGET_NAME} ${SRCS})
TARGET_LINK_LIBRARIES(${TARGET_NAME} ${DEPS_LIBRARIES})