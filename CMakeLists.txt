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

find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME}
  src/nano_s.h
  src/nano_s.cc
  src/main.cc
  third-party/HidApi/HidApi.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third-party/HidApi)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} cscrypto)

if(NOT MSVC)
  target_link_libraries(${PROJECT_NAME} udev)
else()
  target_link_libraries(${PROJECT_NAME} setupapi)
endif()
