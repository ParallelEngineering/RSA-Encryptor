#include "encryption.h"

#include "vec/operations.h"

std::vector<std::uint8_t> encryption::encrypt(const std::vector<std::uint8_t>& rawData,
                                              const std::vector<std::uint8_t>& e,
                                              const std::vector<std::uint8_t>& N) {
    std::vector<std::uint8_t> encryptedData;

    for (std::uint8_t byte : rawData) {
        std::vector<std::uint8_t> product = operations::pow({byte}, e);
        std::vector<std::uint8_t> encryptedByte;
        operations::div(product, N, &encryptedByte);

        // TODO
        if (encryptedByte.size() > 1) {
            std::cerr << "Error: Encrypted byte size exceeds 1 byte." << std::endl;
        }

        encryptedData.push_back(encryptedByte[0]);
    }

    return encryptedData;
}