#ifndef RSA_ENCRYPTOR_HELPER_H
#define RSA_ENCRYPTOR_HELPER_H

#include <filesystem>
#include <string>
#include <vector>

#include "../core/keyPair.h"

namespace helper {
    std::filesystem::path keysPath();
    int keyExists(const std::string& name);

    int writeKey(const std::string &name, const std::vector<uint8_t> &data, bool isPublic);
    int readKey(const std::string &name, std::vector<uint8_t> &data, bool isPublic);
};



#endif //RSA_ENCRYPTOR_HELPER_H
