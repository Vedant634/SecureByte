#include<iostream>
#include "Encryptor.hpp"


void Encryptor::encrypt(std::vector<char>& data , const std::string& key){
        size_t keySize = key.size();
        if(keySize==0){
              throw std::invalid_argument("Key cannot be empty");
        }
        for(size_t i=0; i<data.size();i++){
            data[i] ^= key[i%keySize];
        }
}

void Encryptor::decrypt(std::vector<char>& data , const std::string& key){
    size_t keySize = key.size();
        if(keySize==0){
              throw std::invalid_argument("Key cannot be empty");
        }
        for(size_t i=0; i<data.size();i++){
            data[i] ^= key[i%keySize];
        }
}