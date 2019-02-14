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


# Handling generated headers is based on Dirk Thomas answer on topic:
# - https://answers.ros.org/answers/123416/revisions/
catkin_destinations()
file(MAKE_DIRECTORY ${CATKIN_DEVEL_PREFIX}/${CATKIN_PACKAGE_INCLUDE_DESTINATION})

set_source_files_properties(${PROTO_SOURCES} ${PROTO_HEADERS} PROPERTIES GENERATED TRUE)

set(DELEGATE_INCLUDE_DIRS
  include
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

set(SOURCES_DIRECTORIES
  src/interface
  )

  #Include Sources sub sources
foreach (DIRECTORY ${SOURCES_DIRECTORIES})
  include (${DIRECTORY}/sources.cmake)
  set (PREFIXED_SOURCES)
  foreach (SOURCE ${SOURCES})
    set (PREFIXED_SOURCES ${PREFIXED_SOURCES} ${DIRECTORY}/${SOURCE})
  endforeach (SOURCE)
  set (ALL_SOURCES ${ALL_SOURCES} ${PREFIXED_SOURCES})
endforeach (DIRECTORY)

set(LINKED_LIBRARIES ${catkin_LIBRARIES} ${DELEGATE_LIBRARIES})


#Build the kid_size main library
add_library (${PROJECT_NAME} SHARED ${ALL_SOURCES})
qt5_use_modules(interface MultimediaWidgets)

target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES})

option(BUILD_TRAITEMENT_TOOLS "Building TRAITEMENT tools" OFF)

if (BUILD_TRAITEMENT_TOOLS)
  add_executable(main tools/main.cpp)
  target_link_libraries(main ${PROJECT_NAME} ${catkin_LIBRARIES})
  endif()
