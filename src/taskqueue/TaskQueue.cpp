#include<iostream>
#include "TaskQueue.hpp"

void TaskQueue::push(Task task){
    std::lock_guard<std::mutex> taskLock(queueMutex);
    taskQueue.push(task);
    is_available.notify_one();
}

Task TaskQueue::pop(){
    std::unique_lock<std::mutex> taskLock(queueMutex);
    
    is_available.wait(taskLock,[this](){ return !taskQueue.empty();});

    Task task = taskQueue.front();
    taskQueue.pop();

    return task;
}