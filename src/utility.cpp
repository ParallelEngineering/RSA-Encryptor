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

bool utility::checkForPrime(int number)
{
    int counter = 0;
    if (number <= 1) {
        return false;
    }
    else {

        // Check how many numbers divide n in
        // Range 2 to sqrt(n)
        for (int i = 2; i * i <= number; i++) {
            if (number % i == 0) {
                counter++;
            }
        }

        // If counter is greater than 0 then n is prime
        if (counter > 0) {
            return false;
        }
        else {
            return true;
        }
    }
}
