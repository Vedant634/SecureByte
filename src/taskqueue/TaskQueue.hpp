#ifndef TASKQUEUE_HPP
#define TASKQUEUE_HPP
#include<queue>
#include<mutex>
#include<condition_variable>
#include "Task.hpp"

class TaskQueue {
    public:
        void push(Task task);
        Task pop();

    private:
        std::queue<Task> taskQueue;
        std::mutex queueMutex;
        std::condition_variable is_available;

};

#endif