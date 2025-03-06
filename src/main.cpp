#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

#include "utility.h"
#include "vec/operations.h"

int main() {
     utility util;

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
