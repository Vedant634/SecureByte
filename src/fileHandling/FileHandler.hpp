#ifndef FILE_HANDLER_HPP    
#define FILE_HANDLER_HPP

#include <fstream>
#include <string>
#include<vector>
#include<mutex>

enum class Mode { ENCRYPT , DECRYPT};

class FileHandler{
    public:
        FileHandler(const std::string& fileName,Mode mode);
        ~FileHandler();

        std::vector<char> readChunk(size_t offset,size_t chunkSize);
        bool writeChunk(const std::vector<char>& data, size_t offset);
        size_t getFileSize() ;
        void renameDecryptedFile();


    private :
    std::string fileName;
    std::string outputFileName;
    std::fstream fileStream;
    std::fstream outputStream;
    std::mutex fileMutex;
    Mode mode;

};

#endif