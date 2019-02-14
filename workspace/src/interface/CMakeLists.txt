cmake_minimum_required(VERSION 3.1)
project(interface)



find_package(OpenCV REQUIRED)

find_package(catkin REQUIRED COMPONENTS
  hl_monitoring
  hl_communication
  traitement
  )

find_package(Qt5 REQUIRED COMPONENTS MultimediaWidgets)

#Enable C++11
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11")

set(DELEGATE_INCLUDE_DIRS
  ${OpenCV_DIR}
)

set(DELEGATE_LIBRARIES
  ${OpenCV_LIBS}
  )

catkin_package(
  INCLUDE_DIRS ${DELEGATE_INCLUDE_DIRS}
  LIBRARIES ${PROJECT_NAME} ${DELEGATE_LIBRARIES}
  )

include_directories(
  ${DELEGATE_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
  ${Qt5MultimediaWidgets_INCLUDE_DIRS}
  )

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(LINKED_LIBRARIES ${catkin_LIBRARIES} ${DELEGATE_LIBRARIES})
add_library(interface mainwindow.cpp mainwindow.ui)
qt5_use_modules(interface MultimediaWidgets)

add_executable(main main.cpp)
target_link_libraries(main interface ${LINKED_LIBRARIES})
