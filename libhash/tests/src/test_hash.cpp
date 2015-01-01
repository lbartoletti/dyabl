extern "C" {
#include "CHashTab.h"
#include "OHashTab.h"
#include "hashfn.h"
}

#include <gtest/gtest.h>

TEST(HASH, string) {
  // Add
  ASSERT_EQ(448, SH_add((const void *)"test"));
  ASSERT_EQ(588, SH_add((const void *)"QwerTY"));
  ASSERT_EQ(364, SH_add((const void *)"DYABL"));

  // Elf
  ASSERT_EQ(502948, SH_elf((const void *)"test"));
  ASSERT_EQ(93177753, SH_elf((const void *)"QwerTY"));
  ASSERT_EQ(4838764, SH_elf((const void *)"DYABL"));

  // PJW
  ASSERT_EQ(502948, SH_pjw((const void *)"test"));
  ASSERT_EQ(93177753, SH_pjw((const void *)"QwerTY"));
  ASSERT_EQ(4838764, SH_pjw((const void *)"DYABL"));

  // CRC
  ASSERT_EQ(3906068, SH_crc((const void *)"test"));
  ASSERT_EQ(2772648665, SH_crc((const void *)"QwerTY"));
  ASSERT_EQ(74288140, SH_crc((const void *)"DYABL"));

  // Jenkins
  ASSERT_EQ(1064684737, SH_jenkins((const void *)"test"));
  ASSERT_EQ(1748713207, SH_jenkins((const void *)"QwerTY"));
  ASSERT_EQ(1162261297, SH_jenkins((const void *)"DYABL"));
}

TEST(CHASHTAB, init) { ; }

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
