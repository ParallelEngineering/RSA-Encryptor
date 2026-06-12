if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/Base256/CMakeLists.txt")
  message(FATAL_ERROR "Base256 submodule not initialized. Run: git submodule update --init --recursive")
endif()
add_subdirectory(Base256)