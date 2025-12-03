#include <gtest/gtest.h>

#include "CrystalBase/unrolled_for_loop.h"

TEST(UnrolledForLoop, UnrolledForLoop) {
  int i = 0;
  auto op = [&i]() {
    i++;
  };
  crystal::UnrolledForLoop<decltype(op), 5>(op);
  EXPECT_EQ(i, 5);
}
