#include <iostream>
#include <filesystem>
#include <map>
#include <vector>

#define KEY_FOLDER "rsa-keys"

enum {
    NONE,
    PUBLIC,
    PRIVATE,
    BOTH
};

const char base64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

class key {
private:
    static std::filesystem::path keysPath();
    static int keyExists(std::string name);
    static void generatePublicFromPrivate();
    static int writeKey(const std::string &name, std::vector<uint8_t> data, bool isPublic);
    static std::vector<uint8_t> base64Decode(std::string data);

public:
    static void createRSAKey();

    // TODO
    static std::string base64Encode(std::vector<uint8_t> data);

    static std::pair<unsigned long int, unsigned long int> getPrivateKey(std::string &name);
    static std::pair<unsigned long int, unsigned long int> getPublicKey(std::string &name);
};