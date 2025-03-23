#ifndef VEC_OPERATIONS_H
#define VEC_OPERATIONS_H

#include <iostream>
#include <vector>
#include <cstdint>

namespace operations {
    [[nodiscard]] std::uint64_t getStartBitIndex (const std::vector<std::uint8_t> &a) {
        std::uint64_t index = 0;
        std::uint64_t bitsSince1 = 0;

        for (std::uint8_t number: a) {
            // Skip 8 bits if all zero
            if (number == 0) {
                bitsSince1 += 8;
                continue;
            }
            // Go over every bit in number
            for (std::uint64_t i = 0; i < sizeof(std::uint8_t) * 8; i++) {
                // Select the current bit using a bitmask
                if ((number & 0b1 << i) != 0) {
                    // Increment the index by the bits since the last increment
                    index += bitsSince1 + 1;
                    bitsSince1 = 0;
                } else {
                    bitsSince1++;
                }
            }
        }

        // Returns position of the first bit needed for the number
        return index - 1;
    }

    // This function picks one bit from pickNumber and places it at the least significant position of number.
    [[nodiscard]] std::vector<std::uint8_t> addBitFromNumber (const std::vector<std::uint8_t> &number, const std::vector<std::uint8_t> &pickNumber, std::uint32_t index) {
        std::vector<std::uint8_t> result;

        // Check if the index is valid
        if (index > getStartBitIndex(pickNumber)) {
            return {0};
        }

        // Check if the bit at the given index is set
        bool mostSignificantBit = pickNumber[index / 8] & 0b1 << index % 8;

        for (std::uint8_t currentByte : number) {
            result.push_back((currentByte << 1) + mostSignificantBit);

            // Check if the most significant bit of the current byte is set
            mostSignificantBit = currentByte & 0b1000000 == 0b10000000;
        }

        // In case the length of the vector and the actual number length matches, the numberincreases by one vector element with value 1
        if (mostSignificantBit) result.push_back(0b1);

        return result;
    }

    [[nodiscard]] bool isZero (const std::vector<std::uint8_t> &a) {
        for (std::uint8_t number: a) {
            if (number != 0) return false;
        }

        return true;
    }

    [[nodiscard]] bool isEqual (
        const std::vector<std::uint8_t> &a,
        const std::vector<std::uint8_t> &b) noexcept
    {
        const std::uint32_t aSize = a.size();
        const std::uint32_t bSize = b.size();
        const std::uint64_t iterations = aSize > bSize ? aSize : bSize;

        // In case both numbers are the same length these variables could point ot the same vector
        const std::uint32_t shortest = aSize < bSize ? aSize : bSize;
        const std::vector<std::uint8_t> longest = aSize > bSize ? a : b;

        for (std::uint64_t i = 0; i < iterations; i++) {
            if (i >= shortest) {
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
        const std::uint32_t aSize = a.size();
        const std::uint32_t bSize = b.size();
        const std::uint64_t iterations = aSize > bSize ? aSize : bSize;

        // We loop reverse throw all the vectors to catch leading zeros
        for (std::int64_t i = iterations - 1; i >= 0; i--) {

            const std::uint8_t aValue = (i < aSize) ? a[i] : 0;
            const std::uint8_t bValue = (i < bSize) ? b[i] : 0;

            if (aValue > bValue)
            {
                return true;
            }
        }

        return false;
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

    // The return value can only be positive, if it would be negative, 0 is returned
    [[nodiscard]] std::vector<std::uint8_t> sub(
        const std::vector<std::uint8_t> &a,
        const std::vector<std::uint8_t> &b) noexcept
    {
        // Prevent from ending the subtraction before going over the hole subtractor and stop if the result can only be negative
        if (getStartBitIndex(b) > getStartBitIndex(a)) return {0};

        std::vector<std::uint8_t> result;

        // Handle an underflow when subtracting
        bool borrow = false;

        for (int i = 0; i < a.size(); i++)
        {
            std::int32_t subtract;
            // Check for the end of the subtractor
            if (i >= b.size()) {
                subtract = borrow;
            } else {
                subtract = borrow + b[i];
            }
            borrow = false;

            if (a[i] >= subtract) {
                // If the current number is as least as big as subtract
                std::uint8_t number = a[i] - subtract;
                result.push_back(number);
            } else {
                // Borrow from the next number
                subtract -= 256;
                borrow = true;

                // Here subtract can only be 0 or negative
                std::uint8_t number = a[i] - subtract;
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
        const std::vector<std::uint8_t> dividend,
        const std::vector<std::uint8_t> &divisor,
        std::vector<std::uint8_t> *remaining = nullptr) noexcept
{
        std::vector<std::uint8_t> quotient;
        std::uint8_t quotientBuffer = 0;
        std::uint16_t quotientBitIndex = 0;

        // The index of the last bit in the dividendMask inside dividend
        std::int64_t dividendIndex = getStartBitIndex(dividend);
        // This copy's the most significant bit of the dividend
        std::vector<std::uint8_t> dividendMask = addBitFromNumber({0}, dividend, dividendIndex--);

        while (dividendIndex >= -1) {

            if (quotientBitIndex > 7) {
                // The quotient is stored from the most significant byte to the least significant
                // byte, in contrast to all the other vector based numbers.
                // Which is later reversed, at the end of the function.
                quotient.push_back(quotientBuffer);
                quotientBuffer = 0;
                quotientBitIndex = 0;
            }

            if (isEqual(dividendMask, divisor) || isBigger(dividendMask, divisor)) {

                // Stop the loop if the dividend is smaller than the divisor, because fractional digits are not supported
                if (dividendIndex < 0) {
                    quotientBuffer <<= 1;

                    // If remaining pointer is passed, set the remaining value
                    if (remaining != nullptr) *remaining = dividendMask;
                    break;
                }

                // Shift the dividend and set the new bit as high
                quotientBuffer <<= 1;
                quotientBuffer++;
                quotientBitIndex++;

                dividendMask = sub(dividendMask, divisor);
                dividendMask = addBitFromNumber(dividendMask, dividend, dividendIndex--);
            } else {
                // Stop the loop if the dividend is smaller than the divisor, because fractional digits are not supported
                if (dividendIndex < 0) {
                    quotientBuffer <<= 1;

                    // If remaining pointer is passed, set the remaining value
                    if (remaining != nullptr) *remaining = dividendMask;
                    break;
                }

                // if the divisor is bigger than the dividend, we need to shift the dividend and set the new bit as low
                quotientBuffer <<= 1;
                quotientBitIndex++;

                // Stop the loop if the dividend is smaller than the divisor, because fractional digits are not supported
                if (dividendIndex < 0) break;

                // Because dividendMask is smaller than divisor, we add the next bit from the dividend
                dividendMask = addBitFromNumber(dividendMask, dividend, dividendIndex--);
            }
        }

        if (quotientBuffer != 0) quotient.push_back(quotientBuffer);

        // Reverse the quotient
        std::reverse(quotient.begin(), quotient.end());

        return quotient;
    }

    [[nodiscard]] std::vector<std::uint8_t> pow(
        const std::vector<std::uint8_t> &a,
        const std::uint64_t &pow) noexcept
    {
        // Copy the value from a into result while keeping a constant
        std::vector<std::uint8_t> result;
        std::copy(a.begin(), a.end(), std::back_inserter(result));

        // Start the loop at 1, because the first number is already assigned to result
        for (std::uint32_t i = 1; i < pow; i++) {
            result = mul(result, a);
        }

        return result;
    }
}

#endif
