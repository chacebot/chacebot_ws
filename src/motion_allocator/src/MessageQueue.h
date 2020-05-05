#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H


#include <chrono>
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>

template <class T> class MessageQueue {
public:
  inline void send(T &msg) 
  {
  std::lock_guard<std::mutex> lck(_mutex);
  _queue.push_back(msg);
  _cond.notify_one();
}
  inline T receive(){
  std::unique_lock<std::mutex> lck(_mutex);
  _cond.wait(lck, [this] { return !_queue.empty(); });
  T msg = std::move(_queue.back());
  _queue.pop_back();

  return msg;
}

private:
  std::mutex _mutex;
  std::condition_variable _cond;
  std::deque<T> _queue;
};


#endif