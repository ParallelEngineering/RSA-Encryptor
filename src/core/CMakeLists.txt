add_library(RSA-Core
        utility.cpp
        key.cpp
        encryption.cpp
)
target_include_directories(RSA-Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(RSA-Encryptor PRIVATE RSA-Core)
