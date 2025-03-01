#include "utility.h"

int utility::gcd(int a, int b) {
    if (a == 0) {
        return b;
    }
    return gcd(b % a, a);
}