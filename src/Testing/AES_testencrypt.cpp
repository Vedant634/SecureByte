#include <iostream>
#include "FileHandler.hpp"
#include "ThreadPool.hpp"
#include "AES_Encryptor.hpp"

const size_t CHUNK_SIZE = 4096;

int main() {
    byte key[KEY_SIZE], iv[BLOCK_SIZE];
    std::string inputFile = "test.txt";
    int numThreads = 8;
    AES_Encryptor::generate_key_iv(key, iv);

    try {
        FileHandler encryptFileHandler(inputFile, Mode::ENCRYPT);
        size_t fileSize = encryptFileHandler.getFileSize();
        ThreadPool threadPool(numThreads);

        for (size_t offset = 0; offset < fileSize; offset += CHUNK_SIZE) {
            size_t chunkSize = std::min(CHUNK_SIZE, fileSize - offset);
            std::vector<char> plainText = encryptFileHandler.readChunk(offset, chunkSize);
            auto cipherText = std::make_shared<std::vector<char>>();

            threadPool.executeTask([offset, plainText, key, iv, cipherText, &encryptFileHandler]() mutable {
                AES_Encryptor::encrypt(key, iv, plainText, *cipherText);
                encryptFileHandler.writeChunk(*cipherText, offset);
            });
        }
        // threadPool.waitForCompletion();
        std::cout << "Encryption completed! Encrypted file created: " << inputFile << "_ENCRYPT\n";
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    std::cout<<"Helo"<<std::endl;
    
   
    std::string encryptedFile = "test.txt_ENCRYPT";
    try {
        FileHandler decryptFileHandler(encryptedFile, Mode::DECRYPT);
        size_t fileSize = decryptFileHandler.getFileSize();
        ThreadPool threadPool(numThreads);

        for (size_t offset = 0; offset < fileSize; offset += CHUNK_SIZE) {
            size_t chunkSize = std::min(CHUNK_SIZE, fileSize - offset);
            std::vector<char> cipherText = decryptFileHandler.readChunk(offset, chunkSize);
            auto recoveredText = std::make_shared<std::vector<char>>();

            threadPool.executeTask([offset, cipherText, key, iv, recoveredText, &decryptFileHandler]() mutable {
                AES_Encryptor::decrypt(key, iv, cipherText, *recoveredText);
                decryptFileHandler.writeChunk(*recoveredText, offset);
            });
        }
        // threadPool.waitForCompletion();
        decryptFileHandler.renameDecryptedFile();
        std::cout << "Decryption completed! Decrypted file created: " << inputFile << "_DECRYPT\n";
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
