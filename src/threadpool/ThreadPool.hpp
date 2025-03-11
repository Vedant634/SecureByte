#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include<vector>
#include<queue>
#include<thread>
#include<mutex>
#include <functional>
#include<condition_variable>


class ThreadPool{
    private:
        int numThreads;
        std::vector<std::thread> threads;
        std::queue<std::function<void()>> taskQueue;
        std::mutex poolMutex;
        std::condition_variable poolCv;
        bool stop;
        void workerFunction();
    
    public:
        explicit ThreadPool(int numThreads);
        void executeTask(std::function<void()> task);
        void waitForCompletion();
        ~ThreadPool();
};

#endif