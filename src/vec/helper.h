#ifndef RSA_ENCRYPTOR_HELPER_H
#define RSA_ENCRYPTOR_HELPER_H

#include <cstdint>
#include <vector>

[[nodiscard]] inline std::uint64_t getStartBitIndex(const std::vector<std::uint8_t> &a) {
    std::uint64_t index = 0;
    std::uint64_t bitsSince1 = 0;

    for (const std::uint8_t number : a) {
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

[[nodiscard]] inline std::vector<std::uint8_t> convertToVector(
    std::uint64_t number) noexcept {
    std::vector<std::uint8_t> result;

    // Loop until the whole number is zero
    while (number) {
        // We only care for the lsb
        result.push_back(static_cast<std::uint8_t>(number & 0xFF));

        // Shift the number by 8 to the right
        number >>= 8;
    }

    return result;
}

// This function picks one bit from pickNumber and places it at the least significant position
// of number.
[[nodiscard]] inline std::vector<std::uint8_t> addBitFromNumber(
    const std::vector<std::uint8_t> &number, const std::vector<std::uint8_t> &pickNumber,
    const std::uint32_t index) {
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
        // TODO investigate operation evaluation order problem
        mostSignificantBit = currentByte & 0b1000000 == 0b10000000;
    }

    // In case the length of the vector and the actual number length matches, the
    // number increases by one vector element with value 1
    if (mostSignificantBit) result.push_back(0b1);

    return result;
}

[[nodiscard]] inline bool isBigger(const std::vector<std::uint8_t> &a,
                            const std::vector<std::uint8_t> &b) noexcept {
    const std::uint32_t aSize = a.size();
    const std::uint32_t bSize = b.size();
    const std::uint64_t iterations = aSize > bSize ? aSize : bSize;

    // We loop reverse throw all the vectors to catch leading zeros
    for (std::int64_t i = iterations - 1; i >= 0; i--) {
        const std::uint8_t aValue = (i < aSize) ? a[i] : 0;
        const std::uint8_t bValue = (i < bSize) ? b[i] : 0;

        if (aValue > bValue) {
            return true;
        }
    }

    return false;
}

[[nodiscard]] inline bool isEqual(const std::vector<std::uint8_t> &a,
                           const std::vector<std::uint8_t> &b) noexcept {
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

[[nodiscard]] inline bool isZero(const std::vector<std::uint8_t> &a) {
    for (const std::uint8_t number : a) {
        if (number != 0) return false;
    }

    return true;
}

#endif  // RSA_ENCRYPTOR_HELPER_H
