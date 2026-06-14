#include "key.h"
#include <filesystem>
#include <fstream>
#include <string>

std::string key::base64Encode(const std::vector<uint8_t> &data) {
    std::vector<uint8_t> result;
    size_t index = 0;

    while (index < data.size()) {
        uint32_t dataSegment = 0;
        for (int i = 2; i >= 0; i--) {
            if (index >= data.size()) break;
            const uint8_t number = data.at(index++);
            dataSegment += number << (i * 8);
        }

        for (int j = 0; j < 4; j++) {
            dataSegment <<= 6;
            result.push_back(static_cast<uint8_t>((dataSegment & (0b00111111 << 24)) >> 24));
        }
    }

    std::string outString;
    for (auto c : result) {
        outString += base64Chars[c];
    }
    return outString;
}

std::vector<uint8_t> key::base64Decode(std::string data) {
    std::vector<uint8_t> result;

    while (!data.empty()) {
        if (data.length() < 4) break;
        std::string letterSegment = data.substr(0, 4);
        data.erase(0, 4);

        uint32_t dataSegment = 0;
        for (int i = 3; i >= 0; i--) {
            if (static_cast<size_t>(3 - i) >= letterSegment.length()) continue;
            const uint8_t number = getBase64Index(letterSegment.at(3 - i));
            if (number > 0b111111) {
                std::cerr << "Invalid char: " << letterSegment.at(3 - i) << std::endl;
                continue;
            }
            dataSegment += number << (i * 6);
        }

        for (int j = 0; j < 3; j++) {
            dataSegment <<= 8;
            result.push_back((dataSegment & (0xFF << 24)) >> 24);
        }
    }
    return result;
}

uint8_t key::getBase64Index(char letter) {
    for (int i = 0; base64Chars[i] != '\0'; i++) {
        if (base64Chars[i] == letter) {
            return i;
        }
    }
    return 0;
}

// Currently using dummy values
void key::createKeyPair(PublicKey &pub, PrivateKey &priv) {
    pub.n = operations::Base256(937131);
    pub.e = operations::Base256(65537);

    priv.n = operations::Base256(937131);
    priv.d = operations::Base256(129381);
}
