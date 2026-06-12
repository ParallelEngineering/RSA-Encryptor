add_library(RSA-Core
        utility.cpp
        key.cpp
        encryption.cpp
)

target_link_libraries(RSA-Core PUBLIC Base256)