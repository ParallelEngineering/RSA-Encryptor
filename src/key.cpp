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

int key::writeKey(const std::string& name, const unsigned long int data, const bool isPublic) {
    std::filesystem::path keysFolder = keysPath();

    std::filesystem::path keyFile = keysFolder / (name + (isPublic? ".pub":""));
    std::ofstream outFile(keyFile);

    if (outFile.is_open()) {
        std::string dataStr = std::to_string(data);
        outFile << "-----BEGIN RSA " << (isPublic? "PUBLIC":"PRIVATE") << " KEY-----\n";
        outFile << base64_encode(dataStr) << "\n";
        outFile << "-----END RSA " << (isPublic? "PUBLIC":"PRIVATE") << " KEY-----\n";
        outFile.close();
    } else {
        std::cerr << "Could not write to file: " << keyFile << std::endl;
        return 0;
    }

    return 1;
}

std::string key::base64_encode(std::string &data) {
    //TODO Add base 64 encode function
    return data;
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

    if (writeKey(keyName, 0, true) == 1 && writeKey(keyName, 0, false) == 1) {
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