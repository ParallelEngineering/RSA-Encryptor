#include <iostream>

class key {

public:
    void createRSAKey();

    std::pair<unsigned long int, unsigned long int> getPrivateKey(std::string& name);
    std::pair<unsigned long int, unsigned long int> getPublicKey(std::string& name);
};