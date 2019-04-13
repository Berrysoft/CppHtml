cmake_minimum_required(VERSION 3.1)
project(CppHtml)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include_directories(include)

file(GLOB HTML_HEADERS "include/html/*.hpp")
file(GLOB LIB_SOURCE "src/html/*.cpp")
add_library(cpphtml STATIC ${LIB_SOURCE})

install(FILES ${HTML_HEADERS} DESTINATION include/html)

find_path(BSTEST_INCLUDE_DIR NAMES bstest.hpp)
file(GLOB TEST_SOURCE "test/*.cpp")
add_executable(test ${TEST_SOURCE})
target_include_directories(test PRIVATE ${BSTEST_INCLUDE_DIR})
target_link_libraries(test cpphtml)
