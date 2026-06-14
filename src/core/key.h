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

struct Key {

    // Serializes two 4 bytes Byte Arrays with Big endian
    std::vector<uint8_t> serialize(const operations::Base256 &first, const operations::Base256 &second) {
        std::vector<uint8_t> serialized;
        const auto &firstBytes = first.getBytes();
        const auto &secondBytes = second.getBytes();

        uint32_t firstSize = firstBytes.size();
        uint32_t secondSize = secondBytes.size();

        serialized.push_back((firstSize >> 24) & 0xFF);
        serialized.push_back((firstSize >> 16) & 0xFF);
        serialized.push_back((firstSize >> 8) & 0xFF);
        serialized.push_back(firstSize & 0xFF);

        serialized.insert(serialized.end(), firstBytes.begin(), firstBytes.end());

        serialized.push_back((secondSize >> 24) & 0xFF);
        serialized.push_back((secondSize >> 16) & 0xFF);
        serialized.push_back((secondSize >> 8) & 0xFF);
        serialized.push_back(secondSize & 0xFF);

        serialized.insert(serialized.end(), secondBytes.begin(), secondBytes.end());

        return serialized;
    }

    // Deserializes two 4 bytes Byte Arrays with Big endian
    bool deserialize(const std::vector<uint8_t> &data, operations::Base256 &outFirst, operations::Base256 &outSecond) {
        if (data.size() < 8) return false;

        size_t index = 0;

        uint32_t firstSize = (data[index] << 24) | (data[index + 1] << 16) | (data[index + 2] << 8) | data[index + 3];
        index += 4;

        if (index + firstSize > data.size()) return false;
        std::vector firstBytes(data.begin() + index, data.begin() + index + firstSize);
        index += firstSize;

        if (index + 4 > data.size()) return false;

        const uint32_t secondSize = (data[index] << 24) | (data[index + 1] << 16) | (data[index + 2] << 8) | data[index + 3];
        index += 4;

        if (index + secondSize > data.size()) return false;
        std::vector secondBytes(data.begin() + index, data.begin() + index + secondSize);

        outFirst = operations::Base256(firstBytes);
        outSecond = operations::Base256(secondBytes);

        return true;
    }

};

struct PublicKey : Key{
    operations::Base256 n;
    operations::Base256 e;
};

struct PrivateKey : Key{
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

    static void createKeyPair(PublicKey &pub, PrivateKey &priv);

public:
    static void createRSAKey();
    static bool getPrivateKey(const std::string &name, PrivateKey &outKey);
    static bool getPublicKey(const std::string &name, PublicKey &outKey);
};

#endif