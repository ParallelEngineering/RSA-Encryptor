#ifndef VEC_OPERATIONS_H
#define VEC_OPERATIONS_H

#include <iostream>
#include <vector>
#include <cstdint>

[[nodiscard]] uint64_t getStartBitIndex (const std::vector<std::uint8_t> &a) {
    uint64_t index = 0;
    uint64_t bitsSince1 = 0;

    for (auto number: a) {
        // skip 8 bits if all zero
        if (number == 0) {
            bitsSince1 += 8;
            continue;
        }
        // go over every bit in number 
        for (int i = 0; i < sizeof(uint8_t) * 8; i++) {
            // select the current bit using a bitmask
            if ((number & 0b1 << i) != 0) {
                // increment the index by the bits since the last increment
                index += bitsSince1 + 1;
                bitsSince1 = 0;
            } else {
                bitsSince1++;
            }
        }
    }

    // returns position of the first bit needed for the number
    return index - 1;
}

[[nodiscard]] bool isZero (const std::vector<std::uint8_t> &a) {
    for (auto number: a) {
        if (number != 0) return false;
    }

    return true;
}

[[nodiscard]] bool isEqual (
    const std::vector<std::uint8_t> &a,
    const std::vector<std::uint8_t> &b) noexcept
{
    const uint64_t iterations = a.size() > b.size() ? a.size() : b.size();
    // in case both numbers are the same length these variables could point ot the same vector
    const std::vector<std::uint8_t> shortest = a.size() < b.size() ? a : b;
    const std::vector<std::uint8_t> longest = a.size() > b.size() ? a : b;

    for (uint64_t i = 0; i < iterations; i++) {
        if (i >= shortest.size()) {
            if (longest[i] != 0) return false;
        } else {
            if (a[i] != b[i]) return false;
        }
    }

    return true;
}

[[nodiscard]] bool isBigger (
    const std::vector<std::uint8_t> &a,
    const std::vector<std::uint8_t> &b) noexcept
{
    return true;
}

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

// the return value can only be positive, if it would be negative, 0 is returned
[[nodiscard]] std::vector<std::uint8_t> sub(
    const std::vector<std::uint8_t> &a,
    const std::vector<std::uint8_t> &b) noexcept
{
    // prevent from ending the subtraction before going over the hole subtractor and stop if the result can only be negative
    if (getStartBitIndex(b) > getStartBitIndex(a)) return {0};

    std::vector<std::uint8_t> result;

    // handle an underflow when subtracting
    bool borrow = false;

    for (int i = 0; i < a.size(); i++)
    {
        std::int32_t subtract;
        // check for the end of the subtractor
        if (i >= b.size()) {
            subtract = borrow;
        } else {
            subtract = borrow + b[i];
        }
        borrow = false;

        if (a[i] >= subtract) {
            // if the current number is as least as big as subtract
            uint8_t number = a[i] - subtract;
            result.push_back(number);
        } else {
            // borrow from the next number
            subtract -= 256;
            borrow = true;

            // here subtract can only be 0 or negative
            uint8_t number = a[i] - subtract;
            result.push_back(number);
        }
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

[[nodiscard]] std::vector<std::uint8_t> pow(
    const std::vector<std::uint8_t> &a,
    const uint64_t &pow) noexcept
{
    // copy the value from a into result while keeping a constant
    std::vector<uint8_t> result;
    std::copy(a.begin(), a.end(), std::back_inserter(result));

    // start the loop at 1, because the first number is already assigned to result
    for (int i = 1; i < pow; i++) {
        result = mul(result, a);
    }

    return result;
}

#endif
