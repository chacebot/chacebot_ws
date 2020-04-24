#ifndef LOCAL_CONTROLLER_H
#define LOCAL_CONTROLLER_H

#include "ros/ros.h"
#include <msg/RCLocal.h>

#include <boost/asio.hpp>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

class LocalController

{
public:
  LocalController(std::string port, ros::NodeHandle nh, ros::Rate &loop_rate);
  //~LocalController();
  void update(ros::Rate &loop_rate); ////** use promise - future here to process new
                                ///messages before pubishing using a task

private:
  std::string readSerial();
  void output();

  std::mutex _mutex;
  std::thread t1;
  std::thread t2;

  std::unique_ptr<std::vector<uint32_t>> _message =
      std::make_unique<std::vector<uint32_t>>(2, 0);

  boost::asio::io_service io;
  boost::asio::serial_port serial;

  ros::Publisher publisher;
  msg::RCLocal command_msg;

  static const int STEERING_CHANNEL = 0;
  static const int DRIVE_CHANNEL = 1;
  static const int ENABLE_CHANNEL = 1;

  bool SAFETY_STATUS = false;
};

#endif