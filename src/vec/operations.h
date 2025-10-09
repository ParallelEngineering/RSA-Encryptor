#ifndef VEC_OPERATIONS_H
#define VEC_OPERATIONS_H

#include <cstdint>
#include <stdexcept>
#include <vector>

namespace operations {
class Base256 {
   public:
    [[nodiscard]] std::uint64_t getStartBitIndex(const std::vector<std::uint8_t> &a) {
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

    // This function picks one bit from pickNumber and places it at the least significant position
    // of number.
    [[nodiscard]] std::vector<std::uint8_t> addBitFromNumber(
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
            mostSignificantBit = currentByte & 0b1000000 == 0b10000000;
        }

        // In case the length of the vector and the actual number length matches, the
        // number increases by one vector element with value 1
        if (mostSignificantBit) result.push_back(0b1);

        return result;
    }

    [[nodiscard]] bool isZero(const std::vector<std::uint8_t> &a) {
        for (std::uint8_t number : a) {
            if (number != 0) return false;
        }

        return true;
    }

    [[nodiscard]] bool isEqual(const std::vector<std::uint8_t> &a,
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

    [[nodiscard]] bool isEqual(Base256 &a, Base256 &b) {
        return isEqual(a.data, b.data);
    }

    [[nodiscard]] bool isBigger(const std::vector<std::uint8_t> &a,
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

    explicit Base256(std::uint64_t initalValue) {
        data = convertToVector(initalValue);
    }

    void add(const std::vector<std::uint8_t> &b) noexcept {
        // Time complexity O(iterations)
        // Initialize the result vector to store the sum
        std::vector<std::uint8_t> result;

        // Get the max iterations based on the largest vector
        const int iterations = std::max(data.size(), b.size());

        // Carry to handle overflow between bytes
        std::uint16_t carry = 0;

        for (int i = 0; i < iterations; i++) {
            // Set up a holder for the sum
            std::uint16_t sum = carry;

            // Only add to sum if we actually have a value in a
            if (i < data.size()) {
                sum += data[i];
            }

            // Only add to sum if we actually have a value in b
            if (i < b.size()) {
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
        if (carry) {
            result.push_back(static_cast<std::uint8_t>(carry));
        }

        data = std::move(result);
    }

    // The return value can only be positive, if it would be negative, 0 is returned
    void sub(const std::vector<std::uint8_t> &b) noexcept {
        // Prevent from ending the subtraction before going over the hole subtractor and stop if the
        // result can only be negative
        if (getStartBitIndex(b) > getStartBitIndex(data)) throw std::invalid_argument("Invalid base256 index");

        std::vector<std::uint8_t> result = sub(data, b);

        data = std::move(result);
    }

    [[nodiscard]] std::vector<std::uint8_t> convertToVector(std::uint64_t number) noexcept;

    // The return value can only be positive, if it would be negative, 0 is returned
    [[nodiscard]] std::vector<std::uint8_t> sub(const std::vector<std::uint8_t> &a,
                                                const std::vector<std::uint8_t> &b) noexcept;

    void mul(const std::vector<std::uint8_t> &b) noexcept;
    void div(const std::vector<std::uint8_t> &divisor,
        std::vector<std::uint8_t> *remaining = nullptr) noexcept;

    [[nodiscard]] std::vector<std::uint8_t> pow(const std::vector<std::uint8_t> &a,
                                                const std::uint64_t &pow) noexcept;

    Base256& operator+=(const Base256& rhs) {
        add(rhs.data);
        return *this;
    }

    Base256& operator-=(const Base256& rhs) {
        sub(rhs.data);
        return *this;
    }

    Base256& operator*=(const Base256& rhs) {
        mul(rhs.data);
        return *this;
    }

    Base256& operator/=(const Base256& rhs) {
        div(rhs.data);
        return *this;
    }

private:
    std::vector<std::uint8_t> data;
};

}  // namespace operations

#endif
