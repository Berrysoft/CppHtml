cmake_minimum_required(VERSION 3.1)
project(CppHtml CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB HTML_HEADERS "include/html/*.hpp")
file(GLOB LIB_SOURCE "src/*.cpp")
add_library(cpphtml ${LIB_SOURCE})
target_include_directories(cpphtml
  PUBLIC
    $<INSTALL_INTERFACE:include>    
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(cpphtml PUBLIC "CPPHTML_STATIC_DEFINE")
endif()

install(FILES ${HTML_HEADERS} DESTINATION include/html)
install(TARGETS cpphtml RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

if(BUILD_TESTS)
  #find_path(BSTEST_INCLUDE_DIR NAMES bstest.hpp)
  file(GLOB TEST_SOURCE "test/*.cpp")
  add_executable(test ${TEST_SOURCE})
  #target_include_directories(test PRIVATE ${BSTEST_INCLUDE_DIR})
  target_link_libraries(test cpphtml)
endif()
