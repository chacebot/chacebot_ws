#ifndef LOCAL_CONTROLLER_H
#define LOCAL_CONTROLLER_H

#include "ros/ros.h"
#include <boost/asio.hpp>
#include <msg/RCLocal.h>
#include <regex>
#include <string>

class LocalController

{
public:
  LocalController(std::string port, ros::NodeHandle nh, ros::Rate &loop_rate);

  void execute(ros::Rate &loop_rate);

  enum class SafetyStatus { is_enabled, is_disabled };

  struct Message {
    float STEERING{};
    float DRIVE{};
    SafetyStatus STATUS = SafetyStatus::is_disabled;
  };

private:
  void readSerial();
  static const float rcConverter(const int &ppm);
  void eventHandler();

  Message message;

  boost::asio::io_service io;
  boost::asio::serial_port serial;

  ros::Publisher publisher;
  msg::RCLocal command_msg;
};

#endif