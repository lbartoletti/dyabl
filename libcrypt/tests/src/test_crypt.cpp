extern "C" {
#include "Crypt.h"
}

#include <gtest/gtest.h>

TEST(CRYPT, init) { ; }

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
