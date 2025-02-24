#ifndef FILE_HANDLER_HPP    
#define FILE_HANDLER_HPP

#include <fstream>
#include <string>
#include<vector>
#include<mutex>

class FileHandler{
    public:
        FileHandler(const std::string& fileName);
        ~FileHandler();

        std::vector<char> readChunk(size_t offset,size_t chunkSize);
        bool writeChunk(const std::vector<char>& data, size_t offset);
        size_t getFileSize() ;


    private :
    std::string fileName;
    std::fstream fileStream;
    std::mutex fileMutex;

};

#endif