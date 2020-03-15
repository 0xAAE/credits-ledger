cmake_minimum_required(VERSION 3.10)

project(nanos)

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
else()
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
endif()

find_package(Threads REQUIRED)

add_library(${PROJECT_NAME} STATIC
  nano_s.h
  nano_s.cc
  packet.h
  packet.cc
  ../../third-party/HidApi/HidApi.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           ${CMAKE_CURRENT_SOURCE_DIR}/../../third-party/HidApi)

target_link_libraries(${PROJECT_NAME}
                      ${CMAKE_THREAD_LIBS_INIT} cscrypto)

if(NOT MSVC)
  target_link_libraries(${PROJECT_NAME} udev)
else()
  target_link_libraries(${PROJECT_NAME} setupapi)
endif()
