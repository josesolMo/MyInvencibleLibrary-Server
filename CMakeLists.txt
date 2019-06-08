cmake_minimum_required(VERSION 3.13)
project(MyInvincibleLibrary-Server)

set(CMAKE_CXX_STANDARD 14)

add_executable(MyInvincibleLibrary-Server main.cpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSON-C json-c)
link_libraries(${JSON-C_LIBRARIES})
target_link_libraries(MyInvincibleLibrary-Server ${JSON-C_LIBRARIES})