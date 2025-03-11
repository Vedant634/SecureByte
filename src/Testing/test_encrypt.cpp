#include <iostream>
#include "FileHandler.hpp"
#include "ThreadPool.hpp"
#include "Encryptor.hpp"

const size_t CHUNK_SIZE = 4096;

int main(){
    std::string key = "my_secret";
    std::string inputFile = "test.txt";
    int numThreads = 8;

    try{
        FileHandler encryptFileHandler(inputFile, Mode::ENCRYPT);
        size_t fileSize = encryptFileHandler.getFileSize();
        ThreadPool threadPool(numThreads);

        for(size_t offset =0 ; offset < fileSize ; offset+= CHUNK_SIZE ){
            size_t chunkSize = std::min(CHUNK_SIZE , fileSize - offset);    
            std::vector<char> chunk = encryptFileHandler.readChunk(offset , chunkSize);

            threadPool.executeTask([offset,chunk = std::move(chunk),key,&encryptFileHandler]()mutable{
                Encryptor::encrypt(chunk,key);
                encryptFileHandler.writeChunk(chunk,offset);
            });
        }
         std::cout << "Encryption completed! Encrypted file created: " << inputFile << "_ENCRYPT\n";
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
       return 0;
}