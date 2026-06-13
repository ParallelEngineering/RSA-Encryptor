#include "key.h"
#include <filesystem>
#include <fstream>
#include <string>

std::filesystem::path key::keysPath() {
    std::filesystem::path programRootPath = std::filesystem::current_path().parent_path();
    std::filesystem::path keysFolder = programRootPath / KEY_FOLDER;
    return keysFolder;
}

int key::keyExists(const std::string& name) {
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

// Serializes two 4 bytes Byte Arrays with Big endian
std::vector<uint8_t> key::serializeKey(const operations::Base256 &first, const operations::Base256 &second) {
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
bool key::deserializeKey(const std::vector<uint8_t> &data, operations::Base256 &outFirst, operations::Base256 &outSecond) {
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

int key::writeKey(const std::string &name, const std::vector<uint8_t> &data, const bool isPublic) {
    std::filesystem::path keysFolder = keysPath();
    std::filesystem::path keyFile = keysFolder / (name + (isPublic ? ".pub" : ""));
    std::ofstream outFile(keyFile);

    std::string base64Data = base64Encode(data);

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

int key::readKey(const std::string &name, std::vector<uint8_t> &data, bool isPublic) {
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

    data = base64Decode(keyString);
    return 1;
}

std::string key::base64Encode(const std::vector<uint8_t> &data) {
    std::vector<uint8_t> result;
    size_t index = 0;

    while (index < data.size()) {
        uint32_t dataSegment = 0;
        for (int i = 2; i >= 0; i--) {
            if (index >= data.size()) break;
            const uint8_t number = data.at(index++);
            dataSegment += number << (i * 8);
        }

        for (int j = 0; j < 4; j++) {
            dataSegment <<= 6;
            result.push_back(static_cast<uint8_t>((dataSegment & (0b00111111 << 24)) >> 24));
        }
    }

    std::string outString;
    for (auto c : result) {
        outString += base64Chars[c];
    }
    return outString;
}

std::vector<uint8_t> key::base64Decode(std::string data) {
    std::vector<uint8_t> result;

    while (!data.empty()) {
        if (data.length() < 4) break;
        std::string letterSegment = data.substr(0, 4);
        data.erase(0, 4);

        uint32_t dataSegment = 0;
        for (int i = 3; i >= 0; i--) {
            if (static_cast<size_t>(3 - i) >= letterSegment.length()) continue;
            const uint8_t number = getBase64Index(letterSegment.at(3 - i));
            if (number > 0b111111) {
                std::cerr << "Invalid char: " << letterSegment.at(3 - i) << std::endl;
                continue;
            }
            dataSegment += number << (i * 6);
        }

        for (int j = 0; j < 3; j++) {
            dataSegment <<= 8;
            result.push_back((dataSegment & (0xFF << 24)) >> 24);
        }
    }
    return result;
}

uint8_t key::getBase64Index(char letter) {
    for (int i = 0; base64Chars[i] != '\0'; i++) {
        if (base64Chars[i] == letter) {
            return i;
        }
    }
    return 0;
}

// Currently using dummy values
void key::createKeyPair(PublicKey &pub, PrivateKey &priv) {
    pub.n = operations::Base256(937131);
    pub.e = operations::Base256(65537);

    priv.n = operations::Base256(937131);
    priv.d = operations::Base256(129381);
}

void key::createRSAKey() {
    std::string keyName;
    std::cout << "What should be the name of the keys?";
    std::cin >> keyName;

    std::filesystem::path keysFolder = keysPath();

    if (!std::filesystem::exists(keysFolder)) {
        std::filesystem::create_directory(keysFolder);
    }

    if (keyExists(keyName) != NONE) {
        std::cout << "This key already exists" << std::endl;
        return;
    }

    PublicKey pub;
    PrivateKey priv;
    createKeyPair(pub, priv);

    std::vector<uint8_t> serializedPub = serializeKey(pub.n, pub.e);
    std::vector<uint8_t> serializedPriv = serializeKey(priv.n, priv.d);

    if (writeKey(keyName, serializedPub, true) == 1 && writeKey(keyName, serializedPriv, false) == 1) {
        std::cout << keyName + "-Key created succesfully!\n";
        std::cout << "Path: " << keysFolder << std::endl;
    } else {
        std::cerr << "Error!" << std::endl;
    }
}

bool key::getPrivateKey(const std::string &name, PrivateKey &outKey) {
    std::vector<uint8_t> rawData;
    if (readKey(name, rawData, false) != 1) {
        return false;
    }
    return deserializeKey(rawData, outKey.n, outKey.d);
}

bool key::getPublicKey(const std::string &name, PublicKey &outKey) {
    std::vector<uint8_t> rawData;
    if (readKey(name, rawData, true) != 1) {
        return false;
    }
    return deserializeKey(rawData, outKey.n, outKey.e);
}