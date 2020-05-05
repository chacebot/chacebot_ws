#include "LocalController.h"

LocalController::LocalController(std::string port, ros::NodeHandle nh,
                                 ros::Rate &loop_rate)
    : io(), serial(io, port)

{
  publisher = nh.advertise<msg::RCLocal>("local_cmd", 10);
}

std::string
LocalController::readSerial()
{ 
  using namespace boost;
  char c;
  std::string result;
  asio::streambuf data;
  int steering, drive, status;

  boost::asio::read_until(serial, data, '\n');
  std::istream is(&data);
  std::getline(is, result);
  std::replace(result.begin(), result.end(), '{', ' ');
  std::replace(result.begin(), result.end(), ',', ' ');
  std::replace(result.begin(), result.end(), '}', ' ');
  std::istringstream inputstream(result);
  inputstream >> steering >> drive >> status;
  message.STEERING = rcConverter(steering);
  message.DRIVE = rcConverter(drive);
  message.STATUS = (status > 1900 && status < 2000) ? SafetyStatus::enabled : SafetyStatus::disabled;
  return result;
}

const float LocalController::rcConverter(const int &ppm)
{
  return (ppm == 0) ? 0 : (ppm - 1500) / 500.0;
}

void LocalController::eventHandler()
{ 
  readSerial();
  command_msg.steering = message.STATUS == SafetyStatus::enabled ? message.STEERING : 0;
  command_msg.drive = message.STATUS == SafetyStatus::enabled ? message.DRIVE : 0;
  command_msg.status = message.STATUS == SafetyStatus::enabled ? true : false;
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
