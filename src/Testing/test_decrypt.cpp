#include <iostream>
#include "FileHandler.hpp"
#include "ThreadPool.hpp"
#include "Encryptor.hpp"

const size_t CHUNK_SIZE = 4096;

int main() {
    std::string key = "my_secret";
    std::string inputFile = "test.txt_ENCRYPT";  
    
    int numThreads = 8;

    try {
        FileHandler decryptFileHandler(inputFile, Mode::DECRYPT);
        size_t fileSize = decryptFileHandler.getFileSize();
        ThreadPool threadPool(numThreads);

        for (size_t offset = 0; offset < fileSize; offset += CHUNK_SIZE) {
            size_t chunkSize = std::min(CHUNK_SIZE, fileSize - offset);
            std::vector<char> chunk = decryptFileHandler.readChunk(offset, chunkSize);

            threadPool.executeTask([offset, chunk = std::move(chunk), key, &decryptFileHandler]() mutable {
                Encryptor::decrypt(chunk, key);
                decryptFileHandler.writeChunk(chunk, offset);
            });
        }
       
        decryptFileHandler.renameDecryptedFile();

        std::cout << "Decryption completed! Decrypted file created: " << inputFile << "_DECRYPT\n";
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
