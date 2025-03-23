#include <filesystem>
#include <fstream>
#include <string>

#include "key.h"

std::filesystem::path key::keysPath() {
    // Navigate to the root directory of the project
    std::filesystem::path programRootPath = std::filesystem::current_path().parent_path();
    std::filesystem::path keysFolder = programRootPath / KEY_FOLDER;
    return keysFolder;
}

int key::keyExists(std::string name) {
    // TODO fix function not case sensitive
    /* 0: key doesn't exist
     * 1: only publicKey exists
     * 2: only privateKey exists
     * 3: both keys exists */
    int status = NONE;

    std::filesystem::path keysFolder = keysPath();

    // go over every file in the KEY_FOLDER
    for (const auto& entry : std::filesystem::directory_iterator(keysFolder)) {
        // check if the name matches
        if (entry.path().stem().string() == name) {
            // check if it's a public key
            if (entry.path().extension() == ".pub") {
                // break the loop if both files are found
                if (status == NONE) status = PUBLIC; else {status = BOTH; break;};
            }
            // check if it's a private key
            else if (entry.path().has_extension() == false) {
                if (status == NONE) status = PRIVATE; else {status = BOTH; break;};
            }
        }
    }

    return status;
}

int key::writeKey(const std::string& name, std::vector<uint8_t> *data, const bool isPublic) {
    if (data == nullptr) return -1;

    std::filesystem::path keysFolder = keysPath();

    std::filesystem::path keyFile = keysFolder / (name + (isPublic? ".pub":""));
    // load file in memory
    std::ofstream outFile(keyFile);

    // key in base64 format
    std::string base64Data = base64Encode(*data);

    if (outFile.is_open()) {
        outFile << "-----BEGIN RSA " << (isPublic? "PUBLIC":"PRIVATE") << " KEY-----\n";
        for (int i = 0; i < base64Data.size(); i += 64) {
            outFile << base64Data.substr(i, 64) << "\n";
        }
        outFile << "-----END RSA " << (isPublic? "PUBLIC":"PRIVATE") << " KEY-----\n";
        outFile.close();
    } else {
        std::cerr << "Could not write to file: " << keyFile << std::endl;
        return -1;
    }

    return 1;
}

int key::readKey(const std::string &name, std::vector<uint8_t> *data, bool isPublic) {
    std::filesystem::path keysFolder = keysPath();

    std::filesystem::path keyFile = keysFolder / (name + (isPublic? ".pub":""));
    // load file in memory
    std::ifstream inFile(keyFile);

    // check if file is loaded correctly
    if (!inFile.is_open()) {
        std::cerr << "Die Datei konnte nicht geladen werden!" << std::endl;
        return -1;
    }

    std::string currentLine;
    std::string keyString;

    while (getline (inFile, currentLine)) {

        if (currentLine.empty()) continue;
        if (currentLine.at(0) == '-') continue;

        keyString += currentLine;
    }

    // remove any line breaks
    for (int c = keyString.length() - 1; c >= 0; c--) {
        if (keyString.at(c) == '\n') {
            keyString.erase(c);
        }
    }

    *data = base64Decode(keyString);

    return 1;
}

std::string key::base64Encode(const std::vector<uint8_t> &data) {
    // TODO the encoding might not work if the data size is not dividable by 3
    if (data.size() % 3 != 0) std::cerr << "Encoding data size not dividable by 3" << std::endl;

    std::vector<uint8_t> result;
    int index = 0;

    while (index < data.size()) {

        uint32_t dataSegment = 0;

        // take 3 numbers from the vector
        for (int i = 2; i >= 0; i--) {
            // prevent from reading outside the vector
            if (index > data.size() - 1) break;

            uint8_t number = data.at(index++);
            dataSegment += number << i * 8;
        }

        // 4 times, put 6 bits from dataSegment into result vector
        for (int j = 0; j < 4; j++) {
            dataSegment <<= 6;
            result.push_back(static_cast<uint8_t>((dataSegment & 0b00111111 << 24) >> 24));
        }
    }

    std::string outString;

    // convert the numeric value to the corresponding char of the base64 charset
    for (auto c : result) {
        outString += base64Chars[c];
    }

    return outString;
}

std::vector<uint8_t> key::base64Decode(std::string data) {
    // TODO the decoding might not work if the data size is not dividable by 4
    if (data.size() % 4 != 0) std::cerr << "Decoding data size not dividable by 4" << std::endl;
    
    std::vector<uint8_t> result;

    while (!data.empty()) {
        // remove 4 chars from the string
        std::string letterSegment = data.substr(0, 4);
        data.erase(0, 4);

        uint32_t dataSegment = 0;

        // put 4 chars in one binary string
        for (int i = 3; i >= 0; i--) {
            // check if there are remaining letters
            if (i > letterSegment.length() - 1) continue;

            const uint8_t number = getBase64Index(letterSegment.at(3 - i));

            // check for valid char code
            if (number > 0b111111) {
                std::cerr << "trying to Decode not valid char: " << letterSegment.at(i) << std::endl;
                continue;
            }
            dataSegment += number << i * 6;
        }

        // read data from binary string
        for (int j = 0; j < 3; j++) {
            dataSegment <<= 8;
            result.push_back((dataSegment & 0xFF << 24) >> 24);
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

int key::createKey(std::vector<uint8_t> *keyPublic, std::vector<uint8_t> *keyPrivate) {
    *keyPublic = {
        23, 87, 45, 190, 12, 78, 34, 210, 56, 89,
        123, 67, 90, 150, 32, 76, 54, 200, 11, 99,
        101, 145, 67, 189, 43, 88, 29, 176, 58, 92,
        111, 134, 78, 201, 15, 84, 39, 220, 66, 97,
        105, 142, 71, 185, 49, 81, 27, 170, 53, 95,
        41
    };
    *keyPrivate = {
        34, 78, 123, 56, 89, 210, 45, 190, 12, 87,
        67, 150, 32, 76, 54, 200, 11, 99, 101, 145,
        67, 189, 43, 88, 29, 176, 58, 92, 111, 134,
        78, 201, 15, 84, 39, 220, 66, 97, 105, 142,
        71, 185, 49, 81, 27, 170, 53, 95, 102, 147,
        68, 191, 44, 85, 30, 177, 59, 93, 112, 135,
        79, 202, 16, 85, 40, 221, 67, 98, 23, 87,
        91, 65
    };
    return 1;
}

void key::createRSAKey() {
    std::string keyName;

    std::cout << "What should the key be called? ";
    std::cin >> keyName;

    std::filesystem::path keysFolder = keysPath();

    // check if keys folder exists
    if (!std::filesystem::exists(keysFolder)) {
        // Create the "RSA-Keys" directory in the root folder fo the program
        std::filesystem::create_directory(keysFolder);
    }

    // check if a key with this name already exits
    if (keyExists(keyName) != NONE) {
        std::cout << "This key already exits, try a different name!" << std::endl;
        return;
    }

    auto* keyPublic = new std::vector<uint8_t>();
    auto* keyPrivate = new std::vector<uint8_t>();

    createKey(keyPublic, keyPrivate);

    if (writeKey(keyName, keyPublic, true) == 1 && writeKey(keyName, keyPrivate, false) == 1) {
        std::cout << keyName + " key successfully created!\n";
        std::cout << "You can find your newly created key here: " << keysFolder << std::endl;
    } else {
        std::cerr << "Key creation failed!" << std::endl;
    }
}

std::vector<uint8_t> * key::getPrivateKey(std::string &name) {
    std::vector<uint8_t>* data = new std::vector<uint8_t>;
    readKey(name, data, false);
    return data;
}

std::vector<uint8_t> * key::getPublicKey(std::string &name) {
    std::vector<uint8_t>* data = new std::vector<uint8_t>;
    readKey(name, data, true);
    return data;
}
