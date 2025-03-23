#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <cstdint>
#include <iostream>

class encryption {
   private:
   public:
    std::int8_t encrypt(std::uint8_t data, unsigned long int e, unsigned long N);
};

#endif