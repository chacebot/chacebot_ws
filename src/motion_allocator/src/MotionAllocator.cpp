#include "MotionAllocator.h"

#include "ros/ros.h"
#include "std_msgs/Int16.h"
#include <msg/RCLocal.h>

#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>


template <typename T>
T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex> lck(_mutex);
    _cond.wait(lck, [this]{return !_queue.empty();});
    T msg = std::move(_queue.back());
    _queue.pop_back();

    return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    std::lock_guard<std::mutex> lck(_mutex);
    _queue.push_back(msg);
    _cond.notify_one();
}

MotionAllocator::MotionAllocator(ros::NodeHandle nh, ros::Rate& loop_rate)
{
    pub_left= nh.advertise<std_msgs::Int16>("motor/dps/left", 10);
    pub_right= nh.advertise<std_msgs::Int16>("motor/dps/right", 10);
    rc_local = nh.subscribe("local_cmd", 10, &MotionAllocator::localInputCallBack, this);
    t1 = std::thread(&MotionAllocator::execute,this, std::ref(loop_rate));
}

MotionAllocator::~MotionAllocator()
{
    t1.join();
    std::cout<< "Destructor Called \n";
}

void MotionAllocator::localInputCallBack(const msg::RCLocal::ConstPtr &msg)
{
    {std::lock_guard<std::mutex> lck(_mutex);
    (*_input)[STEERING_CHANNEL] = msg->steering;
    (*_input)[DRIVE_CHANNEL] = msg->drive; }
}

const float MotionAllocator::rcConverter(const int &ppm)
{
    if (ppm == 0) return 0;

    else return (ppm - 1500) / 500.0;
}

void MotionAllocator::output()
{
    std::lock_guard<std::mutex> lck2(_mutex);

    int drive = rcConverter((*_input)[DRIVE_CHANNEL]) * SCALING_FACTOR;
    int steering = rcConverter((*_input)[STEERING_CHANNEL]) * SCALING_FACTOR; 

    if (drive < 1 && -drive < 1)
    {
        (*_output)[LEFT_WHEEL] = steering;             
        (*_output)[RIGHT_WHEEL] = -steering;
    }

    else
    {
        (*_output)[LEFT_WHEEL] = drive + (drive + steering);
        (*_output)[RIGHT_WHEEL] = drive + (drive - steering);
    }
    message.send(std::move(*_output));
    return;
}

void MotionAllocator::update()
{
    t2 = std::thread(&MotionAllocator::output, this);

    std::vector<int> package = message.receive();
    std::cout << "Left: " << package[LEFT_WHEEL] << ", Right: " << package[RIGHT_WHEEL] << "\n";

    msg_left.data = std::move(package[LEFT_WHEEL]);
    msg_right.data = std::move(package[RIGHT_WHEEL]);

    t2.join();
}



void MotionAllocator::execute()
{

    while (ros::ok())
    {
        update();

        pub_left.publish(msg_left);
        pub_right.publish(msg_right);

        loop_rate.sleep();
    }
}

int main(int argc, char **argv)

{
    ros::init(argc, argv,"motion_allocator");
    ros::NodeHandle nh;
    ros::Rate loop_rate(50); // should be yaml config

    std::unique_ptr<MotionAllocator> ma = std::make_unique<MotionAllocator>(nh, loop_rate);
    
    while (ros::ok())
    {
        ros::spinOnce();
        loop_rate.sleep();
    }
    
    return 0;

}
