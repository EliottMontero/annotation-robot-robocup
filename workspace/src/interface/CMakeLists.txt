cmake_minimum_required(VERSION 3.1)
project(interface)




find_package(catkin REQUIRED COMPONENTS
  hl_monitoring
  )

find_package(Qt5 REQUIRED COMPONENTS MultimediaWidgets)

#Enable C++11
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11")

catkin_package(
  INCLUDE_DIRS .
  )

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${Qt5MultimediaWidgets_INCLUDE_DIRS}
  )

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

add_library(interface mainwindow.cpp mainwindow.ui)
qt5_use_modules(interface MultimediaWidgets)

add_executable(main main.cpp)
target_link_libraries(main interface ${LINKED_LIBRARIES})
