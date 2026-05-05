cmake_minimum_required(VERSION 3.20)
project(RSA-Encryptor CXX)

add_executable(RSA-Encryptor main.cpp cli.cpp)

add_subdirectory(core)
add_subdirectory(vec)

target_link_libraries(RSA-Encryptor PRIVATE RSA-Vec)