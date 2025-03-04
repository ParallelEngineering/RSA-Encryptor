#include <filesystem>
#include "keys.h"


void keys::createRSAKey() {
    std::string keyName;

    std::cout << "Enter the name of the key: ";
    std::cin >> keyName;

    // Navigate to the root directory of the project
    std::filesystem::path programRootPath = std::filesystem::current_path().parent_path();
    std::filesystem::path keysFolder = programRootPath / KEY_FOLDER;

    // Check if keys folder exists
    if (!std::filesystem::exists(keysFolder)) {
        // Create the "RSA-Keys" directory in the root folder fo the program
        std::filesystem::create_directory(keysFolder);
    }

    std::cout << "You can find your newly created key here: " << keysFolder << std::endl;
}

std::pair<unsigned long int, unsigned long int> getPrivateKey(std::string& name) {
    return {647'090'566'899, 234'099'456'876'004};
}

std::pair<unsigned long int, unsigned long int> getPublicKey(std::string& name) {
    return {143'548'453'234, 234'099'456'876'004};
}