cmake_minimum_required(VERSION 3.20)
project(RSA-Encryptor CXX)

# Enforce C++20 and disable compiler-specific extensions
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Prevent CMake from re-downloading Catch2 every time CLion reloads
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "Don't update FetchContent on every config" FORCE)

# include(CTest) automatically sets BUILD_TESTING to ON by default
include(CTest)

# Catch2 via FetchContent
include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.11.0
)
FetchContent_MakeAvailable(Catch2)

# Find Catch 2 CMakeList
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)

# Core library
add_library(RSA-Core
        utility.cpp
        key.cpp
        encryption.cpp
        vec/operations.cpp
)
target_include_directories(RSA-Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# RSA-Encryptor Main
add_executable(RSA-Encryptor main.cpp cli.cpp)
target_link_libraries(RSA-Encryptor PRIVATE RSA-Core)

# Tests
if(BUILD_TESTING)
    add_executable(RSA-Encryptor-Tests
            tests/test_base256.cpp
    )
    target_include_directories(RSA-Encryptor-Tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(RSA-Encryptor-Tests
            PRIVATE
            RSA-Core
            Catch2::Catch2WithMain
    )

    catch_discover_tests(RSA-Encryptor-Tests
            TEST_PREFIX base256:
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endif()