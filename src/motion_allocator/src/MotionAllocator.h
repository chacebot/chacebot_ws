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
class MessageQueue      
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
void localInputCallBack(const msg::RCLocal::ConstPtr &msg); 
const float rcConverter(const int & ppm);
void output();  
void update(); 
void execute(ros::Rate& loop_rate);


std::unique_ptr<std::vector<int>> _input = std::make_unique<std::vector<int>> (2, 0);
std::unique_ptr<std::vector<int>> _output = std::make_unique<std::vector<int>> (2, 0);

MessageQueue<std::vector<int>> message;

std::mutex _mutex;

std::thread t1;
std::thread t2;

ros::Subscriber rc_local;

ros::Publisher pub_left;
ros::Publisher pub_right;

std_msgs::Int16 msg_left;
std_msgs::Int16 msg_right;

static const int STEERING_CHANNEL = 0;
static const int DRIVE_CHANNEL =  1;

static const int LEFT_WHEEL = 0;
static const int RIGHT_WHEEL = 1;

static const int SCALING_FACTOR = 200;


public:

MotionAllocator(ros::NodeHandle nh, ros::Rate& loop_rate);
~MotionAllocator();

};

#endif
