#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

#include "utility.h"
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

          std::cout << arguments.publicKey << std::endl;

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

     // std::cout << util.checkForPrime(54557) << std::endl;
     // std::cout << util.checkForPrime(29) << std::endl;
     //std::vector<uint8_t> num1 = {255, 255, 255}; // 16777215 in dezimal
     //std::vector<uint8_t> num2 = {0x01};          // Add one

     std::vector<uint8_t> num1 = convertToVector(16777215); // 16777215 in dezimal
     std::vector<uint8_t> num2 = convertToVector(1);          // Add one

     std::vector<uint8_t> result = add(num1, num2);

     // This gives out the result as a hex number
     for (auto it = result.rbegin(); it != result.rend(); ++it) {
          printf("%02X", *it);
     }
     std::cout << std::endl;
     return 0;
}
