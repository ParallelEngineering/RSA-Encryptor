#include <iostream>
#include <filesystem>

#define KEY_FOLDER "rsa-keys"

enum {
    NONE,
    PUBLIC,
    PRIVATE,
    BOTH
};

class key {
private:
    static std::filesystem::path keysPath();
    static int keyExists(std::string name);
    static void generatePublicFromPrivate();
    static int writeKey(const std::string &name, unsigned long int data, bool isPublic);
    static std::string base64_encode(std::string &data);

public:
    static void createRSAKey();

    static std::pair<unsigned long int, unsigned long int> getPrivateKey(std::string &name);
    static std::pair<unsigned long int, unsigned long int> getPublicKey(std::string &name);
};