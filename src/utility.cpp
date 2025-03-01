#include "utility.h"

int utility::gcd(int a, int b) {
    if (a == 0) {
        return b;
    }
    return gcd(b % a, a);
}

int utility::phi(int a, int b) {
    return ((a - 1) * (b - 1));
}