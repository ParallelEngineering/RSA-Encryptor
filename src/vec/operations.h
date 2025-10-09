#ifndef VEC_OPERATIONS_H
#define VEC_OPERATIONS_H

#include <cstdint>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <vector>

#include "helper.h"

namespace operations {
class Base256 {
   public:

    explicit Base256(const std::uint64_t initalValue) {
        data = convertToVector(initalValue);
    }

    Base256(const Base256& base256) {
        data = base256.data;
    }

    void add(const std::vector<std::uint8_t> &b) noexcept;
    void sub(const std::vector<std::uint8_t> &b) noexcept;
    std::vector<std::uint8_t> sub(const std::vector<std::uint8_t> &a,
                                                const std::vector<std::uint8_t> &b) noexcept;
    void mul(const std::vector<std::uint8_t> &b) noexcept;
    void div(const std::vector<std::uint8_t> &divisor,
             std::vector<std::uint8_t> *remaining = nullptr) noexcept;

    std::vector<std::uint8_t> pow(const std::vector<std::uint8_t> &a,
                                                const std::uint64_t &pow) noexcept;

    Base256& operator=(const Base256& other) {
        // We protect us from self assignment
        if (this != &other) {
            data = other.data;
        }
        return *this;
    }

    friend Base256 operator+(const Base256 &rhs, const Base256 &lhs) {
        Base256 result(rhs);
        result += lhs;
        return result;
    }

    Base256 &operator+=(const Base256 &rhs) {
        add(rhs.data);
        return *this;
    }

    friend Base256 operator-(const Base256 &rhs, const Base256 &lhs) {
        Base256 result(rhs);
        result -= lhs;
        return result;
    }

    Base256 &operator-=(const Base256 &rhs) {
        sub(rhs.data);
        return *this;
    }

    friend Base256 operator*(const Base256 &rhs, const Base256 &lhs) {
        Base256 result(rhs);
        result *= lhs;
        return result;
    }

    Base256 &operator*=(const Base256 &rhs) {
        mul(rhs.data);
        return *this;
    }

    friend Base256 operator/(const Base256 &rhs, const Base256 &lhs) {
        Base256 result(rhs);
        result /= lhs;
        return result;
    }

    Base256 &operator/=(const Base256 &rhs) {
        div(rhs.data);
        return *this;
    }

    [[nodiscard]] friend bool operator==(const Base256& lhs, const Base256& rhs) {
        return isEqual(lhs.data, rhs.data);
    }

    [[nodiscard]] friend bool operator!=(const Base256& lhs, const Base256& rhs) {
        return !isEqual(lhs.data, rhs.data);
    }

   private:
    std::vector<std::uint8_t> data;
};

}  // namespace operations

#endif
