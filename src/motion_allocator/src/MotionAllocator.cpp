#include "MotionAllocator.h"


#include "ros/ros.h"
#include "std_msgs/Int16.h"
#include <msg/RCLocal.h>


MotionAllocator::MotionAllocator(ros::NodeHandle nh) {
  pub_left = nh.advertise<std_msgs::Int16>("motor/dps/left", 10);
  pub_right = nh.advertise<std_msgs::Int16>("motor/dps/right", 10);
  rc_local =
      nh.subscribe("local_cmd", 10, &MotionAllocator::localInputCallBack, this);
}

void MotionAllocator::localInputCallBack(const msg::RCLocal::ConstPtr &msg) {
  {
    std::lock_guard<std::mutex> lck(_mutex);
    _input.steering = msg->steering;
    _input.drive = msg->drive;
  }
}

void MotionAllocator::eventHandler() {
  std::lock_guard<std::mutex> lck(_mutex);

  int steering = _input.steering * SCALING_FACTOR;
  int drive = _input.drive * SCALING_FACTOR;

  if (drive < 1 && -drive < 1) {
    _output.left_wheel_speed = steering;
    _output.right_wheel_speed = -steering;
  }

  else {
    _output.left_wheel_speed = drive + (drive + steering);
    _output.right_wheel_speed = drive + (drive - steering);
  }
  message.send(_output);
  return;
}

void MotionAllocator::update() {
  auto const & package = message.receive();

  msg_left.data = package.left_wheel_speed;
  msg_right.data = package.right_wheel_speed;

}

void MotionAllocator::execute() {


    eventHandler();
    update();
    pub_left.publish(msg_left);
    pub_right.publish(msg_right);
}

int main(int argc, char **argv)

{
  ros::init(argc, argv, "motion_allocator"); //should be yaml config
  ros::NodeHandle nh;
  ros::Rate loop_rate(50); // should be yaml config

  MotionAllocator ma(nh);

  while (ros::ok()) {
    ros::spinOnce();
    ma.execute();
    loop_rate.sleep();
  }

  return 0;
}
