#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <cstring>

#include "utility.h"
#include "cli.h"
#include "key.h"
#include "vec/operations.h"

int main(int argc, char* argv[]) {

     struct {
          bool privateKey = false;
          bool publicKey = false;
     } arguments;

     // Check if there are additional arguments
     if (argc > 1) {

          // Check for additional arguments
          for (int i = 1; argc > i; i++) {
               // Check for a public key
               if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--public")) {
                    arguments.publicKey = true;
               }
               
               // Check for a private key
               if (!strcmp(argv[i], "-P") || !strcmp(argv[i], "--private")) {
                    arguments.privateKey = true;
               }
          }

          std::string feature = argv[1];

          if (feature == "key" && argv[2] != nullptr) {
               std::string subFeature = argv[2];

               if (subFeature == "create") {
                    cli::keyManager::create();
               } else if (subFeature == "list") {
                    cli::keyManager::list();
               } else if (subFeature == "print") {
                    cli::keyManager::print(argv[3], arguments.publicKey, arguments.privateKey);
               }

          } else if (feature == "help") {
               cli::help();
          } else {
               cli::help();
          }
     } else {
          cli::help();
     }

     return 0;
}
