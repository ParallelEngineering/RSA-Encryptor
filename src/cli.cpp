#include <iostream>

#include "cli.h"
#include "key.h"

void cli::help() {
     std::cout << "Welcome to RSA-Encryptor" << std::endl;
}

void cli::create() {
     key keys;
     keys.createRSAKey();
}
