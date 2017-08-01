#ifndef SHAREDQUEUE_HPP
#define SHAREDQUEUE_HPP

#include "Message.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>

template <class T>
class SharedQueue
{
public:
    SharedQueue();
    ~SharedQueue();
    
    T& front();
    void pop_front();
    
    void push_back(const T& item);
    void push_back(T&& item);
    
    int size();
    bool empty();
    
private:
    std::deque<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_;
};

template <class T>
SharedQueue<T>::SharedQueue(){}

template <class T>
SharedQueue<T>::~SharedQueue(){}

template <class T>
T& SharedQueue<T>::front()
{
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
        cond_.wait(mlock);
    }
    return queue_.front();
}

template <class T>
void SharedQueue<T>::pop_front()
{
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
        cond_.wait(mlock);
    }
    queue_.pop_front();
}

template <class T>
void SharedQueue<T>::push_back(const T& item)
{
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push_back(item);
    mlock.unlock();     // unlock before notificiation to minimize mutex con
    cond_.notify_one(); // notify one waiting thread
    
}

template <class T>
void SharedQueue<T>::push_back(T&& item)
{
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push_back(std::move(item));
    mlock.unlock();     // unlock before notificiation to minimize mutex con
    cond_.notify_one(); // notify one waiting thread
    
}

template <class T>
int SharedQueue<T>::size()
{
    std::unique_lock<std::mutex> mlock(mutex_);
    int size = queue_.size();
    mlock.unlock();
    return size;
}

template <class T>
bool SharedQueue<T>::empty()
{
    std::unique_lock<std::mutex> mlock(mutex_);
    bool isEmpty = queue_.empty();
    mlock.unlock();
    return isEmpty;
}

#endif /* ZONE_HPP */
