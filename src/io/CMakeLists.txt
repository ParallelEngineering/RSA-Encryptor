add_library(RSA-IO
        cli.cpp
        helper.cpp
)

target_link_libraries(RSA-IO
        PRIVATE
        RSA-Core
)

target_link_libraries(RSA-Core PUBLIC Base256)