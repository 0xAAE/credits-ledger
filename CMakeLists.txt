cmake_minimum_required(VERSION 3.10)

project(credits_ledger)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
else()
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
endif()

add_subdirectory(third-party/cscrypto)
add_subdirectory(src/nanos)

find_package(Qt5Widgets)

if(${Qt5Widgets_FOUND})
  add_subdirectory(qt)
endif()

add_executable(${PROJECT_NAME} src/main.cc)

target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} cscrypto nanos)
