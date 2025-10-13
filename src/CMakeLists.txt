cmake_minimum_required(VERSION 3.20)
project(RSA-Encryptor CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing early
include(CTest)          # defines BUILD_TESTING (ON by default) and enable_testing()

# --- Catch2 via FetchContent ---
include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.6.0
)
FetchContent_MakeAvailable(Catch2)

# Tell CMake where to find Catch2's CMake helpers (Catch.cmake)
# (Required when Catch2 is brought in via FetchContent and not installed system-wide)
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)  # provides catch_discover_tests()

# --- Your core library (shared between app and tests) ---
add_library(rsa_core
        utility.cpp
        key.cpp
        encryption.cpp
        vec/operations.cpp
)
target_include_directories(rsa_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# --- Main app ---
add_executable(RSA-Encryptor main.cpp cli.cpp)
target_link_libraries(RSA-Encryptor PRIVATE rsa_core)

# --- Tests ---
if(BUILD_TESTING)
    add_executable(RSA-Encryptor-Tests
            tests/test_base256.cpp
    )
    target_include_directories(RSA-Encryptor-Tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(RSA-Encryptor-Tests
            PRIVATE
            rsa_core
            Catch2::Catch2WithMain
    )

    # For single-config generators (Make/Ninja), this “just works”.
    # For multi-config (VS/Xcode), use: ctest -C Debug
    catch_discover_tests(RSA-Encryptor-Tests
            TEST_PREFIX base256:
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endif()