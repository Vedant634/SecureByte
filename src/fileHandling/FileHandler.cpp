#include<iostream>
#include "FileHandler.hpp"

FileHandler::FileHandler(const std::string& fileName) : fileName(fileName){
    fileStream.open(fileName,std::ios::in | std::ios::out | std::ios::binary);

    if(!fileStream.is_open()){
        throw std::runtime_error("Error : could not open file " + fileName);
    }
}

FileHandler::~FileHandler(){
    if(fileStream.is_open())
    fileStream.close();
}

std::vector<char>FileHandler::readChunk(size_t offset,size_t chunkSize){
    std::lock_guard<std::mutex> fileLock(fileMutex);

    std::vector<char> buffer(chunkSize);
    fileStream.seekg(offset,std::ios::beg);

    if(!fileStream.read(buffer.data(),chunkSize))
      buffer.resize(fileStream.gcount());
    

    
    return buffer;
}

bool FileHandler::writeChunk(const std::vector<char>& data, size_t offset){
    std::lock_guard<std::mutex> fileLock(fileMutex);
    if(!fileStream.is_open()){
       throw std::runtime_error("Error no file opened.");
    }
    fileStream.seekp(offset,std::ios::beg);

    fileStream.write(data.data(),data.size());

    fileStream.flush();

    return true;
}

size_t FileHandler::getFileSize(){
    std::lock_guard<std::mutex> lock(fileMutex);

    if (!fileStream.is_open()) {
        throw std::runtime_error("Error: No file opened.");
    }

    fileStream.seekg(0,std::ios::end);

    size_t fileSize = fileStream.tellg();

    fileStream.seekg(0,std::ios::beg);

    return fileSize;
}