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
    static void writeKey(std::string name, unsigned long int data);

public:
    static void createRSAKey();

    static std::pair<unsigned long int, unsigned long int> getPrivateKey(std::string& name);
    static std::pair<unsigned long int, unsigned long int> getPublicKey(std::string& name);
};