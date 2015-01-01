extern "C" {
#include "Utils.h"
#include <stdint.h>
#include <stdlib.h>
}

#include <gtest/gtest.h>

using namespace std;
TEST(utils, swap) {
  int a = 10, b = 20;
  int r;
  r = genswap(&a, &b, sizeof(int));
  ASSERT_EQ(r, 1);
  ASSERT_EQ(a, 20);
  ASSERT_EQ(b, 10);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
