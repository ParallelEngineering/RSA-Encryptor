#include <iostream>

#include "utility.h"

int main() {
     utility util;

     std::cout << util.checkForPrime(54557) << std::endl;
     std::cout << util.checkForPrime(29) << std::endl;
     return 0;
}
