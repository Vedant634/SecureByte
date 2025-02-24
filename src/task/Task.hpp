#ifndef TASK_HPP
#define TASK_HPP
#include<vector>
#include<string>

enum class Action {
    ENCRYPT,
    DECRYPT
};

struct Task{
    std::vector<char> data;
    size_t offset;
    Action action;
    std::string key;

    Task(std::vector<char> &data,size_t offset,Action action,std::string key);
    std::string to_string()const;
};

#endif