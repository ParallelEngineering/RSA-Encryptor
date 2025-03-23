#ifndef CLI_H
#define CLI_H

#include <string>

namespace cli {
void help();

namespace keyManager {
void create();
void list();
void print(const std::string& name, bool publicKey, bool privateKey);
}  // namespace keyManager
}  // namespace cli

#endif