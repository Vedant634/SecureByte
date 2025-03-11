#include <iostream>
#include<filesystem>
#include "FileHandler.hpp"



FileHandler::FileHandler(const std::string &fileName,Mode mode) : fileName(fileName),mode(mode)
{   
    if(mode == Mode::ENCRYPT) {
        fileStream.open(fileName, std::ios::in | std::ios::out | std::ios::binary);

        if (!fileStream.is_open()){
        throw std::runtime_error("Error : could not open file " + fileName);
        }

        std::string outputFileName = fileName + "_ENCRYPT";
        outputStream.open(outputFileName, std::ios::out | std::ios::binary | std::ios::trunc);

        if (!outputStream.is_open()){
        throw std::runtime_error("Error: Could not create output file " + outputFileName);
        }

    }

    else if(mode == Mode::DECRYPT){
        fileStream.open(fileName, std::ios::in | std::ios::out | std::ios::binary);

        if (!fileStream.is_open()){
        throw std::runtime_error("Error : could not open file " + fileName);
        }
    }
    

    
}

FileHandler::~FileHandler()
{
    if (fileStream.is_open())
        fileStream.close();

    if (outputStream.is_open())
    {
        outputStream.close();
    }
}

std::vector<char> FileHandler::readChunk(size_t offset, size_t chunkSize)
{
    std::lock_guard<std::mutex> fileLock(fileMutex);

    std::vector<char> buffer(chunkSize);
    fileStream.seekg(offset, std::ios::beg);

    if (!fileStream.read(buffer.data(), chunkSize))
        buffer.resize(fileStream.gcount());

    return buffer;
}

bool FileHandler::writeChunk(const std::vector<char> &data, size_t offset)
{
    std::lock_guard<std::mutex> fileLock(fileMutex);

    if (mode == Mode::ENCRYPT){
        outputStream.seekp(offset, std::ios::beg);
        outputStream.write(data.data(), data.size());

        if (!outputStream.good()) {
            throw std::runtime_error("Error: Failed to write to file");
        }
        outputStream.flush();
    }
    else if (mode == Mode::DECRYPT) {
      
        fileStream.seekp(offset, std::ios::beg);
        fileStream.write(data.data(), data.size());

        if (!fileStream.good()) {
            throw std::runtime_error("Error: Failed to write to decrypted file");
        }

        fileStream.flush();
    }
    

    return true;
}

size_t FileHandler::getFileSize()
{
    std::lock_guard<std::mutex> lock(fileMutex);
    

    if (!fileStream.is_open())
    {
        throw std::runtime_error("Error: No file opened.");
    }

    fileStream.seekg(0, std::ios::end);

    size_t fileSize = fileStream.tellg();

    fileStream.seekg(0, std::ios::beg);

    return fileSize;
}

void FileHandler::renameDecryptedFile() {
    if (mode == Mode::DECRYPT) {
        std::string decryptedFileName = fileName;
        size_t pos = decryptedFileName.rfind("_ENCRYPT"); // Find last occurrence

        if (pos != std::string::npos) {
            decryptedFileName.replace(pos, 8, "_DECRYPT"); // Replace exactly 8 chars

            try {
                std::filesystem::rename(fileName, decryptedFileName);
                std::cout << "File renamed to " << decryptedFileName << std::endl;
            } catch (const std::filesystem::filesystem_error &e) {
                std::cerr << "Error renaming file: " << e.what() << std::endl;
            }
        } else {
            std::cerr << "Error: Filename does not contain '_ENCRYPT'\n";
        }
    }
}
