cmake_minimum_required(VERSION 2.8.3)
project(msg)


find_package(catkin REQUIRED std_msgs roscpp rospy message_generation)


################################################
## Declare ROS messages, services and actions ##
################################################


## Generate messages in the 'msg' folder
 add_message_files(
   FILES 
   RCLocal.msg
   DIRECTORY msg
 )

## Generate added messages and services with any dependencies listed here
 generate_messages(
   DEPENDENCIES
   std_msgs  # Or other packages containing msgs
)


catkin_package(
   CATKIN_DEPENDS roscpp rospy std_msgs message_runtime
)



