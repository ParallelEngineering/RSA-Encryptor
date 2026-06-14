// key.h
#ifndef KEY_PAIR_H
#define KEY_PAIR_H

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

#include "base256.h"

#define KEY_FOLDER "rsa-keys"

enum { NONE, PUBLIC, PRIVATE, BOTH };

class keyPair {
    static constexpr char base64Chars[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

public:
    struct PublicKey {
        operations::Base256 n;
        operations::Base256 e;

        [[nodiscard]]
        std::vector<uint8_t> serialize() const {
            return s_serialize(n, e);
        }
    };

    struct PrivateKey {
        operations::Base256 n;
        operations::Base256 d;

        [[nodiscard]]
        std::vector<uint8_t> serialize() const {
            return s_serialize(n, d);
        }
    };

private:
    PublicKey public_key;
    PrivateKey private_key;

public:
    static std::vector<uint8_t> s_serialize(const operations::Base256 &first,
                                            const operations::Base256 &second);
    static bool s_deserialize(const std::vector<uint8_t> &data, operations::Base256 &outFirst,
                              operations::Base256 &outSecond);

    keyPair() {
        // Currently using dummy values
        public_key.n = operations::Base256(937131);
        public_key.e = operations::Base256(65537);

        private_key.n = operations::Base256(937131);
        private_key.d = operations::Base256(129381);
    }

    PublicKey getPublicKey() { return public_key; }
    PrivateKey getPrivateKey() { return private_key; }

    // Base64 helper functions
    static std::string base64Encode(const std::vector<uint8_t> &data);
    static std::vector<uint8_t> base64Decode(std::string data);
    static uint8_t getBase64Index(char letter);
};

#endif