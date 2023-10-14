cmake_minimum_required(VERSION 3.0.0)
project(wtfIsDbus VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

find_package(PkgConfig REQUIRED)
pkg_check_modules(DBUS REQUIRED dbus-1)

include_directories(${DBUS_INCLUDE_DIRS})

add_executable(wtfIsDbus main.cpp)

target_link_libraries(wtfIsDbus ${DBUS_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
