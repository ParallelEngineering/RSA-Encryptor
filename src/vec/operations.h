#include <iostream>
#include <vector>
#include <cstdint>

std::vector<uint8_t> add(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b) {
    // Initialize the result vector to store the sum
    std::vector<uint8_t> result;

    // Get the max iterations based on the largest vector
    int iterations = std::max(a.size(), b.size());

    // Carry to handle overflow between bytes
    std::uint16_t carry = 0;

    for (int i = 0; i < iterations; i++)
    {
        // Set up a holder for the sum
        std::uint16_t sum = carry;

        // Only add to sum if we actually have a value in a
        if (i < a.size())
        {
            sum += a[i];
        }
        
        // Only add to sum if we actually have a value in b
        if (i < b.size())
        {
            sum += b[i];
        }

        // Calculate the carry for the next byte (overflow beyond 255)
        carry = sum >> 8;

        // Clear out everything except the lsb
        // This is done by masking the sum with 255
        // For example
        // 0000000101111111 (=383)
        // 0000000011111111 (=255)
        // ----------------
        // 0000000100000000
        sum &= 0xFF;

        // Append the least significant byte of the sum to the result
        result.push_back(static_cast<uint8_t>(sum));
    }

    // If we got a carry we append this as well
    if (carry)
    {
        result.push_back(static_cast<uint8_t>(carry));
    }

    return result;
}

std::vector<uint8_t> mul(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b) {

}