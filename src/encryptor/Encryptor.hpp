#ifndef ENCRYPTOR_HPP
#define ENCRYPTOR_HPP

#include<vector>
#include<string>

class Encryptor {
    public:
    static void encrypt (std::vector<char>& data,const std::string& key);
    static void decrypt (std::vector<char>& data,const std::string& key);
};

#endif