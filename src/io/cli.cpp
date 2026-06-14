#include "cli.h"

#include <iostream>

#include "../core/key.h"
#include "helper.h"

void cli::help() { std::cout << "Welcome to RSA-Encryptor" << std::endl; }

void cli::keyManager::create() {
    std::string keyName;
    std::cout << "What should be the name of the keys?";
    std::cin >> keyName;

    std::filesystem::path keysFolder = helper::keysPath();

    if (!std::filesystem::exists(keysFolder)) {
        std::filesystem::create_directory(keysFolder);
    }

    if (helper::keyExists(keyName) != NONE) {
        std::cout << "This key already exists" << std::endl;
        return;
    }

    PublicKey pub;
    PrivateKey priv;
    key::createKeyPair(pub, priv);

    ByteArray serializedPub = pub.serialize(pub.n, pub.e);
    ByteArray serializedPriv = priv.serialize(priv.n, priv.d);

    if (helper::writeKey(keyName, serializedPub, true) == 1 &&
        helper::writeKey(keyName, serializedPriv, false) == 1) {
        std::cout << keyName + "-Key created succesfully!\n";
        std::cout << "Path: " << keysFolder << std::endl;
    } else {
        std::cerr << "Error!" << std::endl;
    }
}

void cli::keyManager::list() {}

void cli::keyManager::print(const std::string &name, bool publicKey, bool privateKey) {}
