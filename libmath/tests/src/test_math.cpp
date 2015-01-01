extern "C" {
#include "Math.h"
}

#include <gtest/gtest.h>

TEST(MATH, init) { ; }

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
