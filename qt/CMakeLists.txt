cmake_minimum_required(VERSION 2.8.11)

project(credits_ledger_qt)

if(WIN32)
  set(GUI_TYPE WIN32)
endif(WIN32)

#set(HEADERS
#qt5_wrap_cpp(MOC_FILES ${HEADERS})

add_executable(${PROJECT_NAME} ${GUI_TYPE}
  main.cc
  #  ${HEADERS}
  #  ${MOC_FILES}
)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets cscrypto)
