cmake_minimum_required(VERSION 2.8.3)
project(local_controller)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++14 -pthread)

find_package(catkin REQUIRED COMPONENTS
  msg
  roscpp
  std_msgs
)


###################################
## catkin specific configuration ##
###################################

catkin_package(
   CATKIN_DEPENDS msg roscpp std_msgs
)

###########
## Build ##
###########

include_directories(
  ${catkin_INCLUDE_DIRS}
)


## Declare a C++ executable
 add_executable(local_controller src/LocalController.cpp)


## Add cmake target dependencies of the executable
 add_dependencies(local_controller msg_generate_messages_cpp)

## Specify libraries to link a library or executable target against
 target_link_libraries(local_controller
   ${catkin_LIBRARIES}
 )


