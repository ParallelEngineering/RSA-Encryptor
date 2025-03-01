#include <iostream>

#include "cli.h"

int main(int argc, char* argv[]) {

     // check if there are additional arguments
     if (argc > 1) {
          std::string feature = argv[1];
          if (feature == "create") {
               cli::create();
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
