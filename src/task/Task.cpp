#include<iostream>
#include "Task.hpp"
#include <sstream>

Task::Task(std::vector<char> &data,size_t offset,Action action,std::string key)
 :data(data),offset(offset),action(action),key(key){}

std::string Task::to_string() const{
        std::ostringstream oss;
        oss << "Offset: "<< offset
            << ", Action: " << (action==Action::ENCRYPT?"ENCRYPT":"DECRYPT")
            << ", Key: " << key 
            << ", Data Size: " << data.size() << "bytes";
        
        return oss.str();
      
}

