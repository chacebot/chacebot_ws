#include "LocalController.h"

#include "ros/ros.h"
#include "std_msgs/Int16.h"
#include <msg/RCLocal.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

using std::stof;
using std::string;
using std::to_string;

LocalController::LocalController(std::string port, ros::NodeHandle nh,
                                 ros::Rate &loop_rate)
    : io(), serial(io, port)

{
  publisher = nh.advertise<msg::RCLocal>("local_cmd", 10);
}

std::string
LocalController::readSerial() { // have this be it's own infinite loop thread?
  using namespace boost;
  char c;
  std::string result;
  for (;;) {
    asio::read(serial, asio::buffer(&c, 1));
    switch (c) {
    case '\r':
      break;
    case '\n':
      return result;
    default:
      result += c;
    }
  }
}

void LocalController::output() {
  std::string output[2] = {"1500",
                           "1500"};     // use this or global pointer vector?
  std::string raw_input = readSerial(); // mutex here?
  std::string status;
  std::replace(raw_input.begin(), raw_input.end(), '{',
               ' '); // should all this be done in readSerial?
  std::replace(raw_input.begin(), raw_input.end(), ',', ' ');
  std::replace(raw_input.begin(), raw_input.end(), '}', ' ');
  std::istringstream inputstream(raw_input);

  inputstream >> output[STEERING_CHANNEL] >> output[DRIVE_CHANNEL] >> status;
  std::cout << output[STEERING_CHANNEL] << " ," << output[DRIVE_CHANNEL]
            << "\n";
  SAFETY_STATUS = (stoi(status) > 1500 && stoi(status) < 2100) ? true : false;
  std::cout << "Status: " << SAFETY_STATUS << "\n";
  command_msg.status = SAFETY_STATUS;

  if (SAFETY_STATUS == true) {
    command_msg.steering = stoi(output[0]);
    command_msg.drive = stoi(output[1]);
  }

  else {
    command_msg.steering = 1500;
    command_msg.drive = 1500;
  }
}

void LocalController::update(ros::Rate &loop_rate) {

  while (ros::ok()) {
    try {
      output(); // use future - promise here?
    } catch (...) {
      std::cout << "Exception handled with serial read\n";
    };
    publisher.publish(command_msg);
    loop_rate.sleep();
  }
}

int main(int argc, char **argv) {
  std::string device = "/dev/ttyACM0";
  ros::init(argc, argv, "local_controller");
  ros::NodeHandle nh;
  ros::Rate loop_rate(50);

  std::unique_ptr<LocalController> lc =
      std::make_unique<LocalController>(device, nh, loop_rate);
  lc->update(loop_rate);

  return 0;
}