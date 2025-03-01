#include <filesystem>
#include "key.h"


void key::createRSAKey() {
    std::string keyName;

    std::cout << "What should the key be called?" << std::endl;
    std::cin >> keyName;
}

std::pair<unsigned long int, unsigned long int> getPrivateKey(std::string& name) {
    return {647'090'566'899, 234'099'456'876'004};
}

std::pair<unsigned long int, unsigned long int> getPublicKey(std::string& name) {
    return {143'548'453'234, 234'099'456'876'004};
}