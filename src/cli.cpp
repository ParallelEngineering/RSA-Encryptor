#include <iostream>

#include "cli.h"
#include "keys.h"

void cli::help() {
     std::cout << "Welcome to RSA-Encryptor" << std::endl;
}

void cli::key::create() {
     keys::createRSAKey();
}

void cli::key::list() {
}

void cli::key::print(std::string name, bool publicKey, bool privateKey) {
}
