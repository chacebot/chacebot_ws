#include "LocalController.h"


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
  asio::streambuf data;
  boost::asio::read_until(serial, data, '\n');
  std::istream is(&data);
  std::getline(is, result);
  return result;
}

void LocalController::eventHandler() { // should this extraction be in read?
  std::string raw_input = readSerial(); 
  std::string status;
  std::replace(raw_input.begin(), raw_input.end(), '{', ' ');
  std::replace(raw_input.begin(), raw_input.end(), ',', ' ');
  std::replace(raw_input.begin(), raw_input.end(), '}', ' ');
  std::istringstream inputstream(raw_input);

inputstream >> message.STEERING >> message.DRIVE >> status;
message.STATUS = (stoi(status) > 1900 && stoi(status) < 2000) ? SafetyStatus::enabled : SafetyStatus::disabled;
command_msg.steering = message.STATUS == SafetyStatus::enabled ? message.STEERING : 1500;
command_msg.drive = message.STATUS == SafetyStatus::enabled ? message.DRIVE : 1500;
command_msg.status = message.STATUS == SafetyStatus::enabled ? true : false;

}

void LocalController::execute(ros::Rate &loop_rate) {

  while (ros::ok()) {
    try {
      eventHandler();
    } catch (...) {
      std::cout << "Exception handled with serial read\n";
    };
    publisher.publish(command_msg);
    loop_rate.sleep();
  }
}

int main(int argc, char **argv) {
  std::string device = "/dev/ttyACM0";       // should be yaml config
  ros::init(argc, argv, "local_controller"); // should be yaml config
  ros::NodeHandle nh;
  ros::Rate loop_rate(50); // should be yaml config

  try {
    LocalController lc(device, nh, loop_rate);
    lc.execute(loop_rate);
  }

  catch (...) {
    std::cout << "Serial Port unavailible for RC Control\n";
  }

  return 0;
}
