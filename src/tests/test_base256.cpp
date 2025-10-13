// Catch2 v3: use the macros-only header to keep compile times down
#include <catch2/catch_test_macros.hpp>
#include <cstdint>

// Adjust include path to match your project structure:
#include "vec/operations.h"

using operations::Base256;

static Base256 make(uint64_t v) { return Base256(v); }

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
    Base256 a(0), b(0), c(1);
    REQUIRE(a == b);
    REQUIRE(a != c);
}

TEST_CASE("Base256: addition") {
    SECTION("Simple add") {
        REQUIRE(make(10) + make(20) == make(30));
    }

    SECTION("Carry across one byte") {
        REQUIRE(make(255) + make(1) == make(256));
    }

    SECTION("Carry across multiple bytes") {
        REQUIRE(make(65535) + make(1) == make(65536)); // 0xFFFF + 1 = 0x1'0000
    }

    SECTION("Add zero is identity") {
        REQUIRE(make(123456) + make(0) == make(123456));
        REQUIRE(make(0) + make(123456) == make(123456));
    }

    SECTION("Commutativity: a + b == b + a (small domain)") {
        for (uint64_t a = 0; a <= 200; ++a) {
            for (uint64_t b = 0; b <= 200; ++b) {
                REQUIRE(make(a) + make(b) == make(b) + make(a));
            }
        }
    }

    SECTION("Associativity: (a + b) + c == a + (b + c) (very small domain)") {
        for (uint64_t a = 0; a <= 20; ++a)
            for (uint64_t b = 0; b <= 20; ++b)
                for (uint64_t c = 0; c <= 20; ++c)
                    REQUIRE((make(a) + make(b)) + make(c) == make(a) + (make(b) + make(c)));
    }
}

TEST_CASE("Base256: subtraction") {
    SECTION("Simple sub") {
        REQUIRE(make(30) - make(20) == make(10));
    }

    SECTION("Borrow across one byte") {
        REQUIRE(make(256) - make(1) == make(255));
    }

    SECTION("Borrow across multiple bytes") {
        REQUIRE(make(65536) - make(1) == make(65535));
    }

    SECTION("Subtract to zero") {
        REQUIRE(make(123456) - make(123456) == make(0));
    }

    SECTION("Subtract zero is identity") {
        REQUIRE(make(123456) - make(0) == make(123456));
    }

    // NOTE: If your implementation allows negative results, define behavior.
    // Many big-int libs clamp or assume a >= b. Here we skip a<b cases.
}

TEST_CASE("Base256: multiplication") {
    SECTION("Simple mul") {
        REQUIRE(make(7) * make(6) == make(42));
    }

    SECTION("Mul by zero and one") {
        REQUIRE(make(123456) * make(0) == make(0));
        REQUIRE(make(123456) * make(1) == make(123456));
    }

    SECTION("Cross-byte product") {
        REQUIRE(make(256) * make(256) == make(65536));
    }

    SECTION("Commutativity: a * b == b * a (small domain)") {
        for (uint64_t a = 0; a <= 50; ++a) {
            for (uint64_t b = 0; b <= 50; ++b) {
                REQUIRE(make(a) * make(b) == make(b) * make(a));
            }
        }
    }

    SECTION("Distributivity: a*(b+c) == a*b + a*c (very small domain)") {
        for (uint64_t a = 0; a <= 10; ++a)
            for (uint64_t b = 0; b <= 10; ++b)
                for (uint64_t c = 0; c <= 10; ++c)
                    REQUIRE(make(a) * (make(b) + make(c)) == (make(a) * make(b)) + (make(a) * make(c)));
    }
}

TEST_CASE("Base256: division") {
    SECTION("Exact division") {
        REQUIRE(make(42) / make(6) == make(7));
        REQUIRE(make(65536) / make(256) == make(256));
    }

    SECTION("Identity") {
        REQUIRE(make(123456) / make(1) == make(123456));
    }

    // NOTE: Division by zero behavior is unspecified in your header (noexcept).
    // If you define it later (throw, assert, or set to zero), add tests accordingly.
}

TEST_CASE("Base256: mixed expressions") {
    SECTION("(a + b) * c then divide back by c (c > 0)") {
        for (uint64_t a = 1; a <= 50; ++a)
            for (uint64_t b = 1; b <= 50; ++b)
                for (uint64_t c = 1; c <= 50; ++c) {
                    Base256 expr = (make(a) + make(b)) * make(c);
                    REQUIRE(expr / make(c) == make(a) + make(b));
                }
    }
}

TEST_CASE("Base256: comparisons") {
    Base256 a0(0), a1(1), a2(2), a255(255), a256(256);

    SECTION("Basic ordering") {
        REQUIRE(a0 < a1);
        REQUIRE(a1 < a2);
        REQUIRE(a255 < a256);

        REQUIRE(a256 > a255);
        REQUIRE(a2 > a1);
        REQUIRE(a1 > a0);
    }

    SECTION("Equality vs ordering") {
        Base256 x(123456);
        Base256 y(123456);
        REQUIRE(x == y);
        REQUIRE_FALSE(x != y);

        // Strictness: x < x must be false, x <= x must be true
        REQUIRE_FALSE(x < x);       // <-- this will FAIL with your current operator<
        REQUIRE(x <= x);
        REQUIRE_FALSE(x > x);
        REQUIRE(x >= x);
    }

    SECTION("Cross-byte comparisons") {
        REQUIRE(make(256) > make(255));
        REQUIRE(make(65536) > make(65535));
        REQUIRE(make(65536) >= make(65536));
        REQUIRE_FALSE(make(65536) < make(65536));
    }
}
