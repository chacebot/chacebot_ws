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
  std::string result;
  boost::asio::streambuf data;
  int steering, drive, status;

  boost::asio::read_until(serial, data, '\n');
  std::istream is(&data);
  std::getline(is, result);
  std::cout<<"Pattern to match: " << result <<"\n";

  std::string raw_input = "<1500,1500,1500>";

  std::cout <<"RawInput: " << raw_input << "\n";

  //raw_input = result;

static std::regex const inputPattern{"^<(\\d{1,4}),(\\d{1,4}),(\\d{1,4})>$"};

   std::smatch matches;
  
   if (std::regex_search(raw_input, matches, inputPattern))
  { 
    std::cout<< "Matches: " << matches[1] << ", " << matches[2] << ", " << matches[3] <<"\n";
    //auto const rawStatus = std::stoi(matches[3]);
    //message.STEERING = rcConverter(std::stoi(matches[1]));
    //message.DRIVE = rcConverter(std::stoi(matches[2]));
    //message.STATUS = (status > 1900 && status < 2000) ? SafetyStatus::is_enabled : SafetyStatus::is_disabled;
  }
  else
  {
    std::cout <<"Regex does not read anything\n";
  }

  
  std::replace(result.begin(), result.end(), '<', ' ');
  std::replace(result.begin(), result.end(), ',', ' ');
  std::replace(result.begin(), result.end(), '>', ' ');
  std::istringstream inputstream(result);
  inputstream >> steering >> drive >> status;
  message.STEERING = rcConverter(steering);
  message.DRIVE = rcConverter(drive);
  message.STATUS = (status > 1900 && status < 2000) ? SafetyStatus::is_enabled : SafetyStatus::is_disabled;
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
