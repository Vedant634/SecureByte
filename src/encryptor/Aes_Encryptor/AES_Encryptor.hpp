#ifndef AES_Encryptor_HPP
#define AES_Encryptor_HPP

#include <openssl/evp.h>
#include <openssl/rand.h>
#include<vector>
#include<string>

typedef unsigned char byte;
#define KEY_SIZE 32
#define BLOCK_SIZE 16

class AES_Encryptor{
    public:
    static void generate_key_iv(byte key[KEY_SIZE], byte iv[BLOCK_SIZE]);
    static void encrypt(const byte key[KEY_SIZE], const byte iv[BLOCK_SIZE], const std::vector<char>& plaintext, std::vector<char>& ciphertext); 
    static void decrypt(const byte key[KEY_SIZE], const byte iv[BLOCK_SIZE], const std::vector<char>& ciphertext, std::vector<char>& recovered_text);
};

#endif