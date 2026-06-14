#ifndef UTILITY_H
#define UTILITY_H

#include "base256.h"

namespace core::utility {
operations::Base256 modPow(operations::Base256 base, operations::Base256 exponent,
                           const operations::Base256& modulus);
} // namespace core::utility


#endif