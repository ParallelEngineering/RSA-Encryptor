#ifndef VEC_OPERATIONS_H
#define VEC_OPERATIONS_H

#include <iostream>
#include <vector>
#include <cstdint>

[[nodiscard]] std::vector<std::uint8_t> add(
    const std::vector<std::uint8_t> &a,
    const std::vector<std::uint8_t> &b) noexcept
{
    // Time complexity O(iterations)
    // Initialize the result vector to store the sum
    std::vector<std::uint8_t> result;

    // Get the max iterations based on the largest vector
    const int iterations = std::max(a.size(), b.size());

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

        // Calculate the carry which is the overflow beyond 255
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
        result.push_back(static_cast<std::uint8_t>(sum));
    }

    // If we got a carry we append this as well
    if (carry)
    {
        result.push_back(static_cast<std::uint8_t>(carry));
    }

    return result;
}

[[nodiscard]] std::vector<std::uint8_t> convertToVector(std::uint64_t number) noexcept {

    std::vector<std::uint8_t> result;

    // Loop until the whole number is zero
    while (number)
    {
        // We only care for the lsb
        result.push_back(static_cast<std::uint8_t>(number & 0xFF));

        // Shift the number by 8 to the right
        number >>= 8;
    }

    return result;
}

[[nodiscard]] std::vector<std::uint8_t> mul(
    const std::vector<std::uint8_t> &a,
    const std::vector<std::uint8_t> &b) noexcept
{
    // Time complexity O(aSize * bSize)
    const std::uint64_t aSize = a.size();
    const std::uint64_t bSize = b.size();

    // Initialize the result vector with zeros, with the size of aSize + bSize
    std::vector<std::uint8_t> result(aSize + bSize, 0);

    for (std::uint64_t i = 0; i < aSize; i++)
    {
        // Set up the carry value for each iteration
        std::uint16_t carry = 0;

        for (uint64_t x = 0; x < bSize; x++)
        {
            // Calculate the product by adding up the previous result, the carry, and the new product
            std::uint16_t product = result[i + x] + carry + (a[i] * b[x]);

            // Calculate the carry which is the overflow beyond 255
            carry = product >> 8;

            // Store the least significant byte (lsb) of the product in the result
            result[i + x] = static_cast<std::uint8_t>(product & 0xFF);
        }

        // If there is a carry, append it to the result
        // But with the offset of bSize because of the previous inner loop
        result[i + bSize] += static_cast<std::uint8_t>(carry);
    }

    return result;
}

[[nodiscard]] std::vector<std::uint8_t> div(
    const std::vector<std::uint8_t> &a,
    const std::vector<std::uint8_t> &b) noexcept
{
    std::vector<std::uint8_t> result;
    return result;
}

#endif
