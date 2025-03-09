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

int key::writeKey(const std::string& name, std::vector<uint8_t> data, const bool isPublic) {
    std::filesystem::path keysFolder = keysPath();

    std::filesystem::path keyFile = keysFolder / (name + (isPublic? ".pub":""));
    // load file in memory
    std::ofstream outFile(keyFile);

    // key in base64 format
    std::string base64Data = base64Encode(data);

    if (outFile.is_open()) {
        outFile << "-----BEGIN RSA " << (isPublic? "PUBLIC":"PRIVATE") << " KEY-----\n";
        for (int i = 0; i < base64Data.size(); i += 64) {
            outFile << base64Data.substr(i, 64) << "\n";
        }
        outFile << "-----END RSA " << (isPublic? "PUBLIC":"PRIVATE") << " KEY-----\n";
        outFile.close();
    } else {
        std::cerr << "Could not write to file: " << keyFile << std::endl;
        return 0;
    }

    return 1;
}

std::string key::base64Encode(std::vector<uint8_t> data) {

    const unsigned int charCount = data.size() * 1.5;
    char* result = new char[charCount];
    unsigned int resultIndex = charCount - 1;

    while (!data.empty()) {

        uint32_t dataSegment = 0;

        // pop 3 numbers from the data vector and store it as one block in dataSegment
        for (int i = 0; i < 3; i++) {
            if (data.size() < 1) break;
            uint8_t number = data.back();
            data.pop_back();
            dataSegment += number << i * 8;
        }

        // 4 times, put 6 bits from dataSegment into result array
        for (int j = 0; j < 4; j++) {
            if (resultIndex < 1) break;
            result[resultIndex--] = static_cast<uint8_t>(dataSegment & 0b00111111);
            dataSegment >>= 6;
        }
    }

    std::string outString;

    // convert the numeric value to the corresponding char of the base64 charset
    for (char& c : std::string(result, charCount)) {
        outString += base64Chars[c];
    }

    // free memory space
    delete[] result;

    return outString;
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

    if (writeKey(keyName, {0}, true) == 1 && writeKey(keyName, {0}, false) == 1) {
        std::cout << keyName + " key successfully created!\n";
        std::cout << "You can find your newly created key here: " << keysFolder << std::endl;
    } else {
        std::cerr << "Key creation failed!" << std::endl;
    }
}

std::pair<unsigned long int, unsigned long int> getPrivateKey(std::string& name) {
    return {647'090'566'899, 234'099'456'876'004};
}

std::pair<unsigned long int, unsigned long int> getPublicKey(std::string& name) {
    return {143'548'453'234, 234'099'456'876'004};
}