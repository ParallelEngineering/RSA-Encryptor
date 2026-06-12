#ifndef CLI_H
#define CLI_H

#include <string>

namespace cli {
void help();
void runBase256Poc(); // Declared PoC function

namespace keyManager {
void create();
void list();
void print(const std::string &name, bool publicKey = true, bool privateKey = false);
}
}

#endif // CLI_H