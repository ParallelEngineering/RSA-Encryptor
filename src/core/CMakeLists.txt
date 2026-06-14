add_library(RSA-Core
        keyPair.cpp
        encrypt.cpp
        decrypt.h
        decrypt.cpp
)

target_link_libraries(RSA-Core PUBLIC Base256)