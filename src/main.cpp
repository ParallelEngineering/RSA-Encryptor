#include <iostream>

#include "cli.h"

int main(int argc, char* argv[]) {

     struct {
          bool privateKey = false;
          bool publicKey = false;
     } arguments;

     // check if there are additional arguments
     if (argc > 1) {

          // check for additional arguments
          for (int i = 1; i >= argc; i++) {
               if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--public") == 0) {
                    arguments.publicKey = true;
               } else if (strcmp(argv[i], "-P") == 0 || strcmp(argv[i], "--private") == 0) {
                    arguments.privateKey = true;
               }
          }

          std::string feature = argv[1];

          if (feature == "key" && argv[2] != nullptr) {
               std::string subFeature = argv[2];

               if (subFeature == "create") {
                    cli::key::create();
               } else if (subFeature == "list") {
                    cli::key::list();
               } else if (subFeature == "print") {
                    cli::key::print(argv[3], arguments.publicKey, arguments.privateKey);
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
