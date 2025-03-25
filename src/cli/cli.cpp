#include "cli.h"

#include <filesystem>
#include <iostream>

#include "../rsa/key.h"

using std::filesystem::path;

void cli::help() { std::cout << "Welcome to RSA-Encryptor" << std::endl; }

void cli::encrypt(std::string fileName) {}

void cli::keyManager::create() { key::createRSAKey(); }

void cli::keyManager::list() {}

void cli::keyManager::print(const std::string& name, bool publicKey, bool privateKey) {}
