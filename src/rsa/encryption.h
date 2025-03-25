#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <cstdint>
#include <iostream>

class encryption {
   public:
    static std::vector<std::uint8_t> encrypt(const std::vector<std::uint8_t>& data,
                                             const std::vector<std::uint8_t>& e,
                                             const std::vector<std::uint8_t>& N);
};

#endif