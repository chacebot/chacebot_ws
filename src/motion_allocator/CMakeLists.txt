cmake_minimum_required(VERSION 2.8.3)
project(motion_allocator)

## Compile as C++11, supported in ROS Kinetic and newer
 add_compile_options(-std=c++14 -pthread)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  msg
)


###################################
## catkin specific configuration ##
###################################
catkin_package(
   CATKIN_DEPENDS roscpp rospy msg std_msgs
)

###########
## Build ##
###########

include_directories(
  ${catkin_INCLUDE_DIRS}
)


## Declare a C++ executable
add_executable(motion_allocator src/MotionAllocator.cpp)

## Add cmake target dependencies of the executable
add_dependencies(motion_allocator msg_generate_messages_cpp)

## Specify libraries to link a library or executable target against
 target_link_libraries(motion_allocator
   ${catkin_LIBRARIES}
 )
