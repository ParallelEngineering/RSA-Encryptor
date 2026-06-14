add_library(RSA-CLI
        cli.cpp
        helper.cpp
)

target_link_libraries(RSA-CLI
        PRIVATE
        RSA-Core
)

target_link_libraries(RSA-Core PUBLIC Base256)