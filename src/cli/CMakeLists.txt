add_library(RSA-CLI
    cli.cpp
)

target_link_libraries(RSA-CLI
        PRIVATE
        RSA-Core
)