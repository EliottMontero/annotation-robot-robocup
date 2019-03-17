cmake_minimum_required(VERSION 3.1)
project(traitement)


find_package(catkin REQUIRED COMPONENTS
  hl_communication
  hl_monitoring
  )

  
include(FindProtobuf)
find_package(Protobuf REQUIRED)


#Enable C++11
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11")


set(DELEGATE_INCLUDE_DIRS
  include)

catkin_package(
   INCLUDE_DIRS ${DELEGATE_INCLUDE_DIRS}
   LIBRARIES ${PROJECT_NAME}
)

include_directories(${DELEGATE_INCLUDE_DIRS} ${catkin_INCLUDE_DIRS}  )


set(SOURCES_DIRECTORIES
  src/traitement
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



#Build the kid_size main library
add_library (${PROJECT_NAME} SHARED ${ALL_SOURCES})

target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES})

option(BUILD_TRAITEMENT_TOOLS "Building TRAITEMENT tools" OFF)

if (BUILD_TRAITEMENT_TOOLS)
  add_executable(test_traitement tools/test_traitement.cpp)
  target_link_libraries(test_traitement ${PROJECT_NAME} ${catkin_LIBRARIES})

  add_executable(test_pos tools/test.cpp)
  target_link_libraries(test_pos ${PROJECT_NAME} ${catkin_LIBRARIES})
  
endif()
