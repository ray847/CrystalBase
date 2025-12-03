enable_testing()

add_subdirectory(GoogleTest)

add_executable(
  test
  unrolled_for_loop.test.cpp
)
target_link_libraries(
  test
  CrystalBase
  GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(test)
