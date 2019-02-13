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

# Handling generated headers is based on Dirk Thomas answer on topic:
# - https://answers.ros.org/answers/123416/revisions/
catkin_destinations()
file(MAKE_DIRECTORY ${CATKIN_DEVEL_PREFIX}/${CATKIN_PACKAGE_INCLUDE_DESTINATION})


set_source_files_properties(${PROTO_SOURCES} ${PROTO_HEADERS} PROPERTIES GENERATED TRUE)

set(DELEGATE_INCLUDE_DIRS
  include)

catkin_package(
  INCLUDE_DIRS ${DELEGATE_INCLUDE_DIRS} ${PROTO_HEADERS}
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
  endif()