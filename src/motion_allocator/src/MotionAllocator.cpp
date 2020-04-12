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
    // The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 

    std::unique_lock<std::mutex> lck(_mutex);
    _cond.wait(lck, [this]{return !_queue.empty();});

    T msg = std::move(_queue.back());
    _queue.pop_back();

    return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
    std::lock_guard<std::mutex> lck(_mutex);
    _queue.push_back(msg);
    _cond.notify_one();
}

MotionAllocator::MotionAllocator(ros::NodeHandle& nodeHandle)
{
    nh = nodeHandle;
    rc_local = nodeHandle.subscribe("local_cmd", 10, &MotionAllocator::localInputCallBack, this);
    pub_left= nodeHandle.advertise<std_msgs::Int16>("motor/dps/left", 10);
    pub_right= nodeHandle.advertise<std_msgs::Int16>("motor/dps/right", 10);
    //t1 = std::thread(&MotionAllocator::localInput, this);
}

MotionAllocator::~MotionAllocator()
{
    //t1.join();
    std::cout<< "Destructor Called \n";
}

void MotionAllocator::localInputCallBack(const msg::RCLocal::ConstPtr &msg)
{
    //{std::lock_guard<std::mutex> lck(_mutex);
    (*_input)[0] = msg->steering;
    (*_input)[1] = msg->drive; 

    //std::cout<< "Local input call back steering: " << (*_input)[0] << "\n";
    //std::cout<< "Local input call back Drive: " << (*_input)[1] << "\n";
}
/*
void MotionAllocator::localInput()
{
    while (true)
    {
        for(int i = 1000; i < 2000; i++)
        {
            {std::lock_guard<std::mutex> lck(_mutex);
            (*_input)[0] = 1500;
            (*_input)[1] = i;}
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            
        }

        for(int j = 2000; j > 1000; j--)
        {
            { std::lock_guard<std::mutex> lck(_mutex);
            (*_input)[0] = 1800;
            (*_input)[1] = j; }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            
        }
    }
}
*/

float MotionAllocator::rcConverter(int &ppm)
{

    //std::cout<< "PPM input: " << ppm << "\n";
    if (ppm == 0) return 0;

    else return (ppm - 1500) / 500.0;
}

void MotionAllocator::output()
{
    //std::lock_guard<std::mutex> lck2(_mutex);
    int drive = rcConverter((*_input)[1]) * 20;
    int steering = rcConverter((*_input)[0]) * 20; // implement yaml with config parameter for this scaling factor (20)

    //std::cout<<"Drive: " << (*_input)[1] << "\n";
    //std::cout<<"Steering: " << (*_input)[0] << "\n";

    if (drive < 1 && -drive < 1)
    {
        (*_output)[0] = steering;
        (*_output)[1] = -steering;
    }

    else
    {
        (*_output)[0] = drive + (drive + steering);
        (*_output)[1] = drive + (drive - steering);
    }

    //std::cout<< "Output left: " << (*_output)[0] << "\n";
    //std::cout<< "Output right: " << (*_output)[1] << "\n";
    message.send(std::move(*_output));
    return;
}

void MotionAllocator::update()
{
    t2 = std::thread(&MotionAllocator::output, this);
    //output();

    std::vector<int> package = message.receive();
    std::cout << "Left: " << package[0] << ", Right: " << package[1] << "\n";
    msg_left.data = package[0];
    msg_right.data = package[1];

    t2.join();
}



void MotionAllocator::execute()
{

        //std::cout<< "in execute\n";

        update();

        //thread here for left
        pub_left.publish(msg_left);

        // thread here for right
        pub_right.publish(msg_right);

        //join above threads

        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int main(int argc, char **argv)

{
    ros::init(argc, argv,"motion_allocator");
    ros::NodeHandle nh;

    std::unique_ptr<MotionAllocator> ma = std::make_unique<MotionAllocator>(nh);

    ros::Rate loop_rate(10);

    while (ros::ok())
    {
        ma->execute();
        ros::spinOnce();
        loop_rate.sleep();

    }

    return 0;

}