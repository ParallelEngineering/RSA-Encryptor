#include <cmath>

#include "encryption.h"

std::int8_t encrypt(std::uint8_t data, unsigned long int e, unsigned long N) {
    return std::pow(data, e);
}
