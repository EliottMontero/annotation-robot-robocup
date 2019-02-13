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

catkin_package(
  INCLUDE_DIRS .
  )

include_directories( ${catkin_INCLUDE_DIRS} )


set(SOURCES_DIRECTORIES
  src/traitement
  )

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

#Build the kid_size main library
add_library (traitement src/traitement/annotation.cpp src/traitement/position.cpp) 
target_link_libraries(${PROJECT_NAME} ${PROTOBUF_LIBRARIES})



