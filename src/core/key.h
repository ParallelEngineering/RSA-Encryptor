// key.h
#ifndef KEY_H
#define KEY_H

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "base256.h"

#define KEY_FOLDER "rsa-keys"

enum { NONE, PUBLIC, PRIVATE, BOTH };

struct PublicKey {
    operations::Base256 n;
    operations::Base256 e;
};

struct PrivateKey {
    operations::Base256 n;
    operations::Base256 d;
};

class key {
private:
    static constexpr char base64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    static std::filesystem::path keysPath();
    static int keyExists(const std::string& name);

    static int writeKey(const std::string &name, const std::vector<uint8_t> &data, bool isPublic);
    static int readKey(const std::string &name, std::vector<uint8_t> &data, bool isPublic);

    // Base64 helper functions
    static std::string base64Encode(const std::vector<uint8_t> &data);
    static std::vector<uint8_t> base64Decode(std::string data);
    static uint8_t getBase64Index(char letter);

    // Serializing
    static std::vector<uint8_t> serializeKey(const operations::Base256 &first, const operations::Base256 &second);
    static bool deserializeKey(const std::vector<uint8_t> &data, operations::Base256 &outFirst, operations::Base256 &outSecond);
    static void createKeyPair(PublicKey &pub, PrivateKey &priv);

public:
    static void createRSAKey();
    static bool getPrivateKey(const std::string &name, PrivateKey &outKey);
    static bool getPublicKey(const std::string &name, PublicKey &outKey);
};

#endif