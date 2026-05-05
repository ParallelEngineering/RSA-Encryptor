
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


if (BUILD_TESTING)
    add_executable(RSA-Encryptor-Tests
            ../tests/test_base256.cpp
    )
    target_include_directories(RSA-Encryptor-Tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(RSA-Encryptor-Tests
            PRIVATE
            RSA-Core
            RSA-Vec
            Catch2::Catch2WithMain
    )

    catch_discover_tests(RSA-Encryptor-Tests
            TEST_PREFIX base256:
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endif ()
