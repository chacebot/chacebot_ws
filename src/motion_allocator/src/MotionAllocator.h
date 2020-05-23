#ifndef MOTION_ALLOCATOR_H
#define MOTION_ALLOCATOR_H

#include "ros/ros.h"
#include "std_msgs/Int16.h"
#include <msg/RCLocal.h>

class MotionAllocator {

private:
  void localInputCallBack(const msg::RCLocal::ConstPtr &msg);
  const float rcConverter(const int &ppm);
  void eventHandler();
  void update();

  struct Input {
    float steering{0};
    float drive{0};
  };

  struct Output {
    int left_wheel_speed{0};
    int right_wheel_speed{0};
  };

  Input _input;
  Output _output;

  ros::Subscriber rc_local;
  ros::Publisher pub_left;
  ros::Publisher pub_right;
  std_msgs::Int16 msg_left;
  std_msgs::Int16 msg_right;

  static const int SCALING_FACTOR = 200;

public:
  MotionAllocator(ros::NodeHandle nh);
  void execute();
};

#endif
