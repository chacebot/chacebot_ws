cmake_minimum_required(VERSION 2.8.3)
project(gopigo3_node)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  rospy
  sensor_msgs
  std_msgs
  std_srvs
  nav_msgs
  message_generation
)

add_message_files(
  FILES
  MotorStatus.msg
  MotorStatusLR.msg
)

add_service_files(
  FILES
  SPI.srv
)

generate_messages(
  DEPENDENCIES
  std_msgs
)


###################################
## catkin specific configuration ##
###################################

catkin_package(
  CATKIN_DEPENDS message_runtime
)

###########
## Build ##
###########

## Specify additional locations of header files
include_directories(
  ${catkin_INCLUDE_DIRS}
)


#############
## Install ##
#############

install(PROGRAMS src/gopigo3_driver.py
    DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(PROGRAMS src/gopigo3_drivedistance_sensor.py
    DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(PROGRAMS src/gopigo3_imu.py
    DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(PROGRAMS src/ultra_sonic.py
    DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY launch
    DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

