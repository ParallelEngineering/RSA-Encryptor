#include "utility.h"


namespace core::utility {
// TODO move this to base 256 library
operations::Base256 modPow(operations::Base256 base, operations::Base256 exponent,
                           const operations::Base256& modulus) {
    operations::Base256 result(1);
    base %= modulus;

    operations::Base256 zero(0);
    operations::Base256 one(1);
    operations::Base256 two(2);

    while (exponent > zero) {
        if (exponent % two == one) {
            result = (result * base) % modulus;
        }
        base = (base * base) % modulus;
        exponent /= two;
    }
    return result;
}
} // namespace core::utility
