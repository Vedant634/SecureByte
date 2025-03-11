#include<iostream>
#include "AES_Encryptor.hpp"


void AES_Encryptor::generate_key_iv(byte key[KEY_SIZE], byte iv[BLOCK_SIZE]){
    int rc = RAND_bytes(key,KEY_SIZE);
    if(rc!=1){
        throw std::runtime_error("Failed to generate random key");
    }
    rc = RAND_bytes(iv,BLOCK_SIZE);
    if(rc != 1){
        throw std::runtime_error("Failed to generate random IV");
    }
}

void AES_Encryptor::encrypt(const byte key[KEY_SIZE] ,const byte iv[BLOCK_SIZE],const std::vector<char>& plainText, std::vector<char>& ciphertext){
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw std::runtime_error("Failed to create cipher context");

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Encryption initialization failed");
    }

    int ciphertext_len = plainText.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc());
    ciphertext.resize(ciphertext_len);

    int len;
     if (EVP_EncryptUpdate(ctx, reinterpret_cast<byte*>(ciphertext.data()), &len, 
                         reinterpret_cast<const byte*>(plainText.data()), plainText.size()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Encryption failed");
    }
    EVP_CIPHER_CTX_set_padding(ctx, 1);
    int total_len = len;

    if (EVP_EncryptFinal_ex(ctx, reinterpret_cast<byte*>(ciphertext.data()) + len, &len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Final encryption step failed");
    }

    total_len += len;
    ciphertext.resize(total_len);

    EVP_CIPHER_CTX_free(ctx);
}

void AES_Encryptor::decrypt(const byte key[KEY_SIZE], const byte iv[BLOCK_SIZE],  
    const std::vector<char>& ciphertext, std::vector<char>& recovered_text) {
EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
if (!ctx) throw std::runtime_error("Failed to create cipher context");

if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1) {
EVP_CIPHER_CTX_free(ctx);
throw std::runtime_error("Decryption initialization failed");
}

EVP_CIPHER_CTX_set_padding(ctx, 1);  // Ensure PKCS7 padding is enabled

recovered_text.resize(ciphertext.size());  // Allocate enough space

int len = 0, total_len = 0;

if (EVP_DecryptUpdate(ctx, reinterpret_cast<byte*>(recovered_text.data()), &len, 
  reinterpret_cast<const byte*>(ciphertext.data()), ciphertext.size()) != 1) {
EVP_CIPHER_CTX_free(ctx);
throw std::runtime_error("Decryption failed");
}
total_len += len;

int padding_len = 0;
if (EVP_DecryptFinal_ex(ctx, reinterpret_cast<byte*>(recovered_text.data()) + total_len, &padding_len) != 1) {
EVP_CIPHER_CTX_free(ctx);
throw std::runtime_error("Final decryption step failed (possible padding issue)");
}

total_len += padding_len;
recovered_text.resize(total_len);  

EVP_CIPHER_CTX_free(ctx);
}
