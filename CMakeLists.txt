cmake_minimum_required(VERSION 3.15)

# Project Options
project(CrystalBase CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# External Libraries
#add_subdirectory(lib/spdlog)

# Internal Library
add_library(CrystalBase)
target_sources(CrystalBase
  PRIVATE
    src/base.cpp
    src/concepts.cpp
    src/containers.cpp
    src/stable_vector.cpp
    src/strict_index.cpp
    src/unrolled_for_loop.cpp
)
target_include_directories(CrystalBase
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(CrystalBase
  PRIVATE
)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  message(STATUS "Building CrystalBase Standalone (with tests/main)")
  # Main
  add_executable(main main.cpp)
  target_include_directories(main
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
  target_link_libraries(main
  PRIVATE
    CrystalBase
  )

  # Testing
  add_subdirectory(test)
endif()

