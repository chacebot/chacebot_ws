#ifndef MOTION_ALLOCATOR_H 
#define MOTION_ALLOCATOR_H

#include "ros/ros.h"
#include "std_msgs/Int16.h"
#include <msg/RCLocal.h>

#include <memory>
#include <chrono>
#include <thread>
#include <mutex>
#include <deque>
#include <condition_variable>
#include <vector>


template <class T>
class MessageQueue      //generic message queue for processing output actuation messages without missing a beat
{
public:

    void send(T &&msg);
    T receive();

private:

    std::mutex _mutex;
    std::condition_variable _cond;
    std::deque<T> _queue;
};


class MotionAllocator {

private:

//messageing pipeline
void localInput(); //infinite while loop that reads raw RC commands in ms units (1000 to 2000) from local_controller node and stores in pointer on heap? or could return if calls synchronously in another loop
void localInputCallBack(const msg::RCLocal::ConstPtr &msg);

// math functions
// Pass unique pointer like in the above? - "in and out again" pass the vector index for steering of the serialInput to obtain usable steering values?
float rcConverter(int & ppm);
void output();  //using _inputSerial data - calls rcConverter for _input steering and drive, then interprets into L + R wheel velocity commands by: launches separate threads for steering and drive and adds to message queue
                // try calling this->steering(_inputSerial[0]) and this->drive(_inputSerial[1]) within the math here

void update(); // calls output() reads latest "recieve()" from message queue and publishes to L and R motor velocityies


//data stores
std::unique_ptr<std::vector<int>> _input = std::make_unique<std::vector<int>> (2, 0);
std::unique_ptr<std::vector<int>> _output = std::make_unique<std::vector<int>> (2, 0);

MessageQueue<std::vector<int>> message;
std::condition_variable _condition;
std::mutex _mutex;

std::thread t1;
std::thread t2;

//ros::NodeHandle nh;

ros::Subscriber rc_local;
ros::Publisher pub_left;
ros::Publisher pub_right;

std_msgs::Int16 msg_left;
std_msgs::Int16 msg_right;




public:

MotionAllocator(ros::NodeHandle& nodeHandle);
// sets up ros stuff - publisher and subscriber, etc.
// initialiizes inputSerial() infinite loop


~MotionAllocator();
// closes threads?
// shuts down any needed ros things?
// does not need to deallocate pointer resources because smart pointers

//where is the infinite while loop? what are the threads?
// inputSerial() needs to be a thread or infinite while loop that continuously stores in pointer
// steering() needs to be a thread
// drive() needs to be a thread
// output() could consume steering() and drive() and be it's own thread

void execute(); //infinite while loop that calls update(), publishes L + R motor commands from latest in message queue (vector or array?), with this no need for while loop in main


};






#endif
