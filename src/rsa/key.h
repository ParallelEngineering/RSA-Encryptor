#ifndef KEY_H
#define KEY_H

#include <filesystem>
#include <iostream>
#include <map>
#include <vector>

#define KEY_FOLDER "rsa-keys"

enum { NONE, PUBLIC, PRIVATE, BOTH };

const char base64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

class key {
   private:
    static std::filesystem::path keysPath();
    static int keyExists(std::string name);
    static void generatePublicFromPrivate();

    static int writeKey(const std::string &name, std::vector<uint8_t> *data, bool isPublic);
    static int readKey(const std::string &name, std::vector<uint8_t> *data, bool isPublic);

    static std::string base64Encode(const std::vector<uint8_t> &data);
    static std::vector<uint8_t> base64Decode(std::string data);

    static uint8_t getBase64Index(char letter);

    static int createKey(std::vector<uint8_t> *keyPublic, std::vector<uint8_t> *keyPrivate);

   public:
    static void createRSAKey();

    static std::vector<uint8_t> *getPrivateKey(std::string &name);
    static std::vector<uint8_t> *getPublicKey(std::string &name);
};

#endif