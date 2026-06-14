#include "helper.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

std::filesystem::path helper::keysPath() {
    std::filesystem::path programRootPath = std::filesystem::current_path().parent_path();
    std::filesystem::path keysFolder = programRootPath / KEY_FOLDER;
    return keysFolder;
}

int helper::keyExists(const std::string& name) {
    int status = NONE;
    std::filesystem::path keysFolder = keysPath();

    if (!std::filesystem::exists(keysFolder)) return NONE;

    for (const auto &entry : std::filesystem::directory_iterator(keysFolder)) {
        if (entry.path().stem().string() == name) {
            if (entry.path().extension() == ".pub") {
                if (status == NONE) status = PUBLIC;
                else { status = BOTH; break; }
            } else if (!entry.path().has_extension()) {
                if (status == NONE) status = PRIVATE;
                else { status = BOTH; break; }
            }
        }
    }
    return status;
}

int helper::writeKey(const std::string &name, const std::vector<uint8_t> &data, const bool isPublic) {
    std::filesystem::path keysFolder = keysPath();
    std::filesystem::path keyFile = keysFolder / (name + (isPublic ? ".pub" : ""));
    std::ofstream outFile(keyFile);

    std::string base64Data = key::base64Encode(data);

    if (outFile.is_open()) {
        outFile << "-----BEGIN RSA " << (isPublic ? "PUBLIC" : "PRIVATE") << " KEY-----\n";
        for (size_t i = 0; i < base64Data.size(); i += 64) {
            outFile << base64Data.substr(i, 64) << "\n";
        }
        outFile << "-----END RSA " << (isPublic ? "PUBLIC" : "PRIVATE") << " KEY-----\n";
        outFile.close();
        return 1;
    } else {
        std::cerr << "Could not write to file: " << keyFile << std::endl;
        return -1;
    }
}

int helper::readKey(const std::string &name, std::vector<uint8_t> &data, bool isPublic) {
    std::filesystem::path keysFolder = keysPath();
    std::filesystem::path keyFile = keysFolder / (name + (isPublic ? ".pub" : ""));
    std::ifstream inFile(keyFile);

    if (!inFile.is_open()) {
        std::cerr << "Could not load file: " << keyFile << std::endl;
        return -1;
    }

    std::string currentLine;
    std::string keyString;

    while (getline(inFile, currentLine)) {
        if (currentLine.empty()) continue;
        if (currentLine.at(0) == '-') continue;
        keyString += currentLine;
    }

    // Remove linebreaks
    for (int c = static_cast<int>(keyString.length()) - 1; c >= 0; c--) {
        if (keyString.at(c) == '\n' || keyString.at(c) == '\r') {
            keyString.erase(c, 1);
        }
    }

    data = key::base64Decode(keyString);
    return 1;
}

bool helper::getPrivateKey(const std::string &name, PrivateKey &outKey) {
    std::vector<uint8_t> rawData;
    if (readKey(name, rawData, false) != 1) {
        return false;
    }
    return outKey.deserialize(rawData, outKey.n, outKey.d);
}

bool helper::getPublicKey(const std::string &name, PublicKey &outKey) {
    std::vector<uint8_t> rawData;
    if (readKey(name, rawData, true) != 1) {
        return false;
    }
    return outKey.deserialize(rawData, outKey.n, outKey.e);
}