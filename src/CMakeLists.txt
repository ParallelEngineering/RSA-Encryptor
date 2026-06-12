cmake_minimum_required(VERSION 3.20)
project(RSA-Encryptor CXX)

add_subdirectory(core)
add_subdirectory(cli)

add_executable(RSA-Encryptor main.cpp)

target_link_libraries(RSA-Encryptor PRIVATE RSA-Vec RSA-Core RSA-CLI)
