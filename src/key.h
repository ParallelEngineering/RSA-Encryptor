#include <iostream>

class key {

public:
    void createRSAKey();
    unsigned long int getPrivateKey(std::string& name);
    unsigned long int getPublicKey(std::string& name);
};