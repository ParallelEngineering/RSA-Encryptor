#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <limits>
#include <iostream>

#include "vec/operations.h"

using operations::Base256;

static Base256 make(uint64_t v) { return Base256(v); }

// Helper for maximum uint64_t value
const uint64_t MAX_U64 = std::numeric_limits<uint64_t>::max();

TEST_CASE("Base256: constructors, copy, assignment, self-assignment") {
    SECTION("Default is zero") {
        Base256 a;
        REQUIRE(a == make(0));
    }

    SECTION("Construct from uint64_t") {
        Base256 a(42);
        REQUIRE(a == make(42));
        REQUIRE(a != make(41));
    }

    SECTION("Construct from max uint64_t") {
        Base256 a(MAX_U64);
        REQUIRE(a == make(MAX_U64));
        REQUIRE(a > make(MAX_U64 - 1));
    }

    SECTION("Copy constructor copies value") {
        Base256 a(123456789);
        Base256 b(a);
        REQUIRE(b == a);
    }

    SECTION("Assignment copies value") {
        Base256 a(777);
        Base256 b(888);
        b = a;
        REQUIRE(b == a);
    }

    SECTION("Self-assignment is safe") {
        Base256 a(424242);
        Base256& ref = a;
        a = ref; // self-assign
        REQUIRE(a == make(424242));
    }
}

TEST_CASE("Base256: equality and inequality") {
    Base256 a(0), b(0), c(1), d(MAX_U64);
    REQUIRE(a == b);
    REQUIRE(a != c);
    REQUIRE(d == make(MAX_U64));
    REQUIRE(d != make(0));
}

TEST_CASE("Base256: addition & compound addition") {
    SECTION("Simple add") {
        REQUIRE(make(10) + make(20) == make(30));
    }

    SECTION("Add zero is identity") {
        REQUIRE(make(123456) + make(0) == make(123456));
        REQUIRE(make(0) + make(123456) == make(123456));
    }

    SECTION("Carry across one byte") {
        REQUIRE(make(255) + make(1) == make(256));
    }

    SECTION("Cascading carry across multiple bytes") {
        REQUIRE(make(65535) + make(1) == make(65536)); // 0xFFFF + 1 = 0x10000
        REQUIRE(make(16777215) + make(1) == make(16777216)); // 0xFFFFFF + 1
        REQUIRE(make(4294967295) + make(1) == make(4294967296)); // 0xFFFFFFFF + 1
    }

    SECTION("Addition exceeding uint64_t (8 bytes to 9 bytes)") {
        Base256 a(MAX_U64); // 0xFFFFFFFFFFFFFFFF
        Base256 b(2);
        Base256 c = a + b;

        // We can't verify with make() because it exceeds uint64_t.
        // But we know (MAX_U64 + 2) - 2 should be MAX_U64
        REQUIRE(c - b == a);
        REQUIRE(c > a);
    }

    SECTION("Compound += operator") {
        Base256 a(100);
        a += make(50);
        REQUIRE(a == make(150));
        a += make(0);
        REQUIRE(a == make(150));
    }

    SECTION("Commutativity: a + b == b + a (small domain)") {
        for (uint64_t a = 0; a <= 200; ++a) {
            for (uint64_t b = 0; b <= 200; ++b) {
                REQUIRE(make(a) + make(b) == make(b) + make(a));
            }
        }
    }
}

TEST_CASE("Base256: subtraction & compound subtraction") {
    SECTION("Simple sub") {
        REQUIRE(make(30) - make(20) == make(10));
    }

    SECTION("Subtract zero is identity") {
        REQUIRE(make(123456) - make(0) == make(123456));
    }

    SECTION("Subtract to zero") {
        REQUIRE(make(123456) - make(123456) == make(0));
        REQUIRE(make(MAX_U64) - make(MAX_U64) == make(0));
    }

    SECTION("Borrow across one byte") {
        REQUIRE(make(256) - make(1) == make(255));
    }

    SECTION("Cascading borrow across multiple bytes") {
        REQUIRE(make(65536) - make(1) == make(65535));
        REQUIRE(make(16777216) - make(1) == make(16777215));
        REQUIRE(make(4294967296) - make(1) == make(4294967295));
    }

    SECTION("Negative result clamping (Underflow to 0)") {
        // Based on your comment: "if it would be negative, 0 is returned"
        REQUIRE(make(10) - make(20) == make(0));
        REQUIRE(make(0) - make(1) == make(0));
        REQUIRE(make(500) - make(MAX_U64) == make(0));
    }

    SECTION("Compound -= operator") {
        Base256 a(100);
        a -= make(40);
        REQUIRE(a == make(60));
        a -= make(60);
        REQUIRE(a == make(0));
    }
}

TEST_CASE("Base256: multiplication & compound multiplication") {
    SECTION("Simple mul") {
        REQUIRE(make(7) * make(6) == make(42));
    }

    SECTION("Mul by zero and one") {
        REQUIRE(make(123456) * make(0) == make(0));
        REQUIRE(make(0) * make(123456) == make(0));
        REQUIRE(make(123456) * make(1) == make(123456));
        REQUIRE(make(1) * make(123456) == make(123456));
    }

    SECTION("Cross-byte product") {
        REQUIRE(make(256) * make(256) == make(65536));
        REQUIRE(make(65536) * make(256) == make(16777216));
    }

    SECTION("Large multiplication (Exceeding uint64_t)") {
        Base256 a(4294967295); // 0xFFFFFFFF (4 bytes)
        Base256 b(4294967295);
        Base256 c = a * b; // Should be 0xFFFFFFFE00000001 (8 bytes)

        // Verify via division
        REQUIRE(c / a == b);
        REQUIRE(c / b == a);
    }

    SECTION("Compound *= operator") {
        Base256 a(10);
        a *= make(10);
        REQUIRE(a == make(100));
        a *= make(0);
        REQUIRE(a == make(0));
    }

    SECTION("Distributivity: a*(b+c) == a*b + a*c (very small domain)") {
        for (uint64_t a = 0; a <= 10; ++a)
            for (uint64_t b = 0; b <= 10; ++b)
                for (uint64_t c = 0; c <= 10; ++c)
                    REQUIRE(make(a) * (make(b) + make(c)) == (make(a) * make(b)) + (make(a) * make(c)));
    }
}

TEST_CASE("Base256: division & compound division") {
    SECTION("Exact division") {
        REQUIRE(make(42) / make(6) == make(7));
        REQUIRE(make(65536) / make(256) == make(256));
    }

    SECTION("Identity and Zero Numerator") {
        REQUIRE(make(123456) / make(1) == make(123456));
        REQUIRE(make(MAX_U64) / make(1) == make(MAX_U64));
        REQUIRE(make(0) / make(123456) == make(0));
    }

    SECTION("Fractional Division (Remainder dropped)") {
        REQUIRE(make(10) / make(3) == make(3));
        REQUIRE(make(255) / make(2) == make(127));
        REQUIRE(make(1000) / make(333) == make(3));
    }

    SECTION("Dividend smaller than Divisor (Results in 0)") {
        REQUIRE(make(10) / make(20) == make(0));
        REQUIRE(make(255) / make(256) == make(0));
        REQUIRE(make(1) / make(MAX_U64) == make(0));
    }

    SECTION("Compound /= operator") {
        Base256 a(100);
        a /= make(2);
        REQUIRE(a == make(50));
        a /= make(50);
        REQUIRE(a == make(1));
    }
}

TEST_CASE("Base256: mixed expressions & combinations") {
    SECTION("(a + b) * c then divide back by c (c > 0)") {
        for (uint64_t a = 1; a <= 20; ++a)
            for (uint64_t b = 1; b <= 20; ++b)
                for (uint64_t c = 1; c <= 20; ++c) {
                    Base256 additionResult = make(a) + make(b);
                    Base256 expr = additionResult * make(c);
                    Base256 divisionResult = expr / make(c);
                    REQUIRE(divisionResult == additionResult);
                }
    }

    SECTION("Chained arithmetic: ((100 * 256) + 50 - 25) / 5 == 5125") {
        Base256 result = ((make(100) * make(256)) + make(50) - make(25)) / make(5);
        REQUIRE(result == make(5125));
    }
}

TEST_CASE("Base256: comparisons edge cases") {
    Base256 a0(0), a1(1), a2(2), a255(255), a256(256);
    Base256 max64(MAX_U64);

    SECTION("Basic ordering") {
        REQUIRE(a0 < a1);
        REQUIRE(a1 < a2);
        REQUIRE(a255 < a256);

        REQUIRE(a256 > a255);
        REQUIRE(a2 > a1);
        REQUIRE(a1 > a0);
    }

    SECTION("Equality vs ordering on same values") {
        Base256 x(123456);
        Base256 y(123456);
        REQUIRE(x == y);
        REQUIRE_FALSE(x != y);

        // Strictness
        REQUIRE_FALSE(x < x);
        REQUIRE(x <= x);
        REQUIRE_FALSE(x > x);
        REQUIRE(x >= x);

        REQUIRE_FALSE(x < y);
        REQUIRE(x <= y);
        REQUIRE_FALSE(x > y);
        REQUIRE(x >= y);
    }

    SECTION("Cross-byte comparisons") {
        REQUIRE(make(256) > make(255));
        REQUIRE(make(65536) > make(65535));
        REQUIRE(make(65536) >= make(65536));
        REQUIRE_FALSE(make(65536) < make(65536));
        REQUIRE(make(16777216) > make(16777215));
    }

    SECTION("Extremely large comparisons") {
        REQUIRE(max64 > a256);
        REQUIRE(a0 < max64);

        Base256 overflow = max64 + a1; // Exceeds uint64_t
        REQUIRE(overflow > max64);
        REQUIRE(max64 < overflow);
    }
}