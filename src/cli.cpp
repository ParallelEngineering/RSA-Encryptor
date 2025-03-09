#include <iostream>

#include "cli.h"
#include "key.h"

void cli::help() {
     std::cout << "Welcome to RSA-Encryptor" << std::endl;
}

void cli::keyManager::create() {
     key::createRSAKey();
}

void cli::keyManager::list() {
}

void cli::keyManager::print(const std::string& name, bool publicKey, bool privateKey) {
}
