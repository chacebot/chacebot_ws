#include "LocalController.h"
#include <iostream>

LocalController::LocalController(std::string port, ros::NodeHandle nh,
                                 ros::Rate &loop_rate)
    : io(), serial(io, port)

{
  publisher = nh.advertise<msg::RCLocal>("local_cmd", 10);
}

void LocalController::readSerial()
{
  std::string raw_input;
  boost::asio::streambuf data;
  boost::asio::read_until(serial, data, '\n');
  std::istream is(&data);
  std::getline(is, raw_input);

  static std::regex const inputPattern{"<(\\d{1,4}),(\\d{1,4}),(\\d{1,4})>\r"};
  std::smatch matches;

  if (std::regex_search(raw_input, matches, inputPattern))
  {
    auto const rawStatus = std::stoi(matches[3]);
    message.STEERING = rcConverter(std::stoi(matches[1]));
    message.DRIVE = rcConverter(std::stoi(matches[2]));
    message.STATUS = (rawStatus > 1100 && rawStatus < 1200) ? SafetyStatus::is_enabled : SafetyStatus::is_disabled;
  }
  else
  {
    std::cout << "Regex does not read anything\n";
    message.STATUS = SafetyStatus::is_disabled;
  }
}

const float LocalController::rcConverter(const int &ppm)
{
  return (ppm == 0) ? 0 : (ppm - 1500) / 500.0;
}

void LocalController::eventHandler()
{
  readSerial();
  command_msg.steering = message.STATUS == SafetyStatus::is_enabled ? message.STEERING : 0;
  command_msg.drive = message.STATUS == SafetyStatus::is_enabled ? message.DRIVE : 0;
  command_msg.status = message.STATUS == SafetyStatus::is_enabled ? true : false;
}

void LocalController::execute(ros::Rate &loop_rate)
{

  while (ros::ok())
  {
    try
    {
      eventHandler();
    }
    catch (...)
    {
      std::cout << "Exception handled with serial read\n";
    };
    publisher.publish(command_msg);
    loop_rate.sleep();
  }
}

int main(int argc, char **argv)
{
  std::string device = "/dev/ttyACM0";       // should be yaml config
  ros::init(argc, argv, "local_controller"); // should be yaml config
  ros::NodeHandle nh;
  ros::Rate loop_rate(50); // should be yaml config

  try
  {
    LocalController lc(device, nh, loop_rate);
    lc.execute(loop_rate);
  }

  catch (...)
  {
    std::cout << "Serial Port unavailible for RC Control\n";
  }

  return 0;
}
