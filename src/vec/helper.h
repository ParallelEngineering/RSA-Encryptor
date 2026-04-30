#ifndef RSA_ENCRYPTOR_HELPER_H
#define RSA_ENCRYPTOR_HELPER_H

#include <cstdint>
#include <vector>

[[nodiscard]] inline std::int64_t getStartBitIndex(const std::vector<std::uint8_t> &a) {
    std::int64_t index = 0;
    std::int64_t bitsSince1 = 0;

    for (const std::uint8_t number: a) {
        if (number == 0) {
            bitsSince1 += 8;
            continue;
        }
        for (std::uint64_t i = 0; i < sizeof(std::uint8_t) * 8; i++) {
            if ((number & (0b1 << i)) != 0) {
                index += bitsSince1 + 1;
                bitsSince1 = 0;
            } else {
                bitsSince1++;
            }
        }
    }

    // Returning signed int64_t stops negative values from underflowing
    return index - 1;
}

[[nodiscard]] inline std::vector<std::uint8_t> convertToVector(
    std::uint64_t number) noexcept {
    std::vector<std::uint8_t> result;

    // Securely ensure 0 results in at least [0], never[]
    if (number == 0) return {0};

    while (number) {
        result.push_back(static_cast<std::uint8_t>(number & 0xFF));
        number >>= 8;
    }

    return result;
}

[[nodiscard]] inline std::vector<std::uint8_t> addBitFromNumber(
    const std::vector<std::uint8_t> &number, const std::vector<std::uint8_t> &pickNumber,
    const std::int64_t index) {
    std::vector<std::uint8_t> result;

    // Validated boundary access
    if (index < 0 || index > getStartBitIndex(pickNumber)) {
        return {0};
    }

    bool mostSignificantBit = (pickNumber[index / 8] & (0b1 << (index % 8))) != 0;

    for (std::uint8_t currentByte: number) {
        // FIX: The mask for MSB is 0x80 (128). We evaluate this BEFORE currentByte gets overwritten/shifted.
        bool nextMSB = (currentByte & 0x80) != 0;
        result.push_back(static_cast<std::uint8_t>((currentByte << 1) | mostSignificantBit));
        mostSignificantBit = nextMSB;
    }

    if (mostSignificantBit) result.push_back(0b1);

    return result;
}

[[nodiscard]] inline bool isBigger(const std::vector<std::uint8_t> &a,
                                   const std::vector<std::uint8_t> &b) noexcept {
    const std::uint32_t aSize = a.size();
    const std::uint32_t bSize = b.size();
    const std::uint64_t iterations = aSize > bSize ? aSize : bSize;

    for (std::int64_t i = iterations - 1; i >= 0; i--) {
        const std::uint8_t aValue = (i < aSize) ? a[i] : 0;
        const std::uint8_t bValue = (i < bSize) ? b[i] : 0;

        if (aValue > bValue) {
            return true;
        } else if (aValue < bValue) {
            return false; // FIX: Ensure false is explicitly returned if the checking bit drops behind.
        }
    }

    return false;
}

[[nodiscard]] inline bool isEqual(const std::vector<std::uint8_t> &a,
                                  const std::vector<std::uint8_t> &b) noexcept {
    const std::uint32_t aSize = a.size();
    const std::uint32_t bSize = b.size();
    const std::uint64_t iterations = aSize > bSize ? aSize : bSize;

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
    for (const std::uint8_t number: a) {
        if (number != 0) return false;
    }
    return true;
}

#endif  // RSA_ENCRYPTOR_HELPER_H
