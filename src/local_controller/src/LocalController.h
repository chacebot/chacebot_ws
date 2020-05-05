#ifndef LOCAL_CONTROLLER_H
#define LOCAL_CONTROLLER_H

#include "ros/ros.h"
#include <msg/RCLocal.h>
#include <boost/asio.hpp>



class LocalController

{
public:
  LocalController(std::string port, ros::NodeHandle nh, ros::Rate &loop_rate);
  //~LocalController();
  void
  execute(ros::Rate &loop_rate); 

  enum class SafetyStatus { enabled, disabled };

  struct Message {
    float STEERING {};
    float DRIVE {};
    SafetyStatus STATUS = SafetyStatus::disabled;
  };

private:
  std::string readSerial();
  static const float rcConverter(const int &ppm);
  void eventHandler();


  Message message;

  boost::asio::io_service io;
  boost::asio::serial_port serial;

  ros::Publisher publisher;
  msg::RCLocal command_msg;

};

#endif