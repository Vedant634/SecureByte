#include<iostream>
#include "ThreadPool.hpp"

ThreadPool::ThreadPool(int numThreads):numThreads(numThreads),stop(false){
    for(int i=0; i < numThreads; i++){
        threads.emplace_back(&ThreadPool::workerFunction,this);
           
    }
}

ThreadPool::~ThreadPool(){
    {
        std::lock_guard<std::mutex> poolLock(poolMutex);
        stop = true;
    }
    poolCv.notify_all();
    for(auto &th: threads){
        if(th.joinable())
            th.join();
    }
}

void ThreadPool::executeTask(std::function<void()> task){
    {
        std::lock_guard<std::mutex> poolLock(poolMutex);
        taskQueue.push(std::move(task));
    }
    poolCv.notify_one();
}

void ThreadPool::workerFunction(){
     while(true){
                std::function<void()>task;
                {
                    std::unique_lock<std::mutex> poolLock(poolMutex);
                    poolCv.wait(poolLock,[this]{
                        return stop || !taskQueue.empty();
                    });
                    if(stop && taskQueue.empty()){
                        return ;
                    }
                    task  = std::move(taskQueue.front());
                    taskQueue.pop();
                }
                task();
            }
        
        
}

void ThreadPool::waitForCompletion() {
    std::unique_lock<std::mutex> lock(poolMutex);
    poolCv.wait(lock, [this] { return taskQueue.empty(); });
}

