extern "C" {
#include "MersenneTwister.h"
#include "Rand.h"
#include <stdint.h>
#include <stdlib.h>
Lcg_s bsd_rand = {0, 1103515245, 12345, 2147483647};
Lcg_s minstd0_rand = {1, 16807, 0, 2147483647};
Lcg_s minstd_Rand = {1, 48271, 0, 2147483647};
/* 2^31*/
Lcg_s randu_rand = {1, 65539, 0, 2147483647};
Lcg_s stand_rand = {
    1, 1103515245, 12345,
    2147483647}; /*Suggestion in the ISO/IEC 9899, ANSI C, Glibc */
/* 2^32 */
Lcg_s numericalrecipes_rand = {1, 1664525, 1013904223, 4294967295};
/* 2^48 */
// Lcg_s posix48_rand  = {0, 25214903917, 11, 281474976710655};
}

#include <gtest/gtest.h>
#include <random>

using namespace std;

TEST(bsd_rand, rand) {
  linear_congruential_engine<uint64_t, 1103515245, 12345, 2147483648> bsdrand(
      0);
  for (int i = 0; i < 10; i++)
    ASSERT_EQ(lcg_st(&bsd_rand), bsdrand());
}

TEST(minstd0_rand, rand) {
  linear_congruential_engine<uint64_t, 16807, 0, 2147483648> minstd0rand(1);
  for (int i = 0; i < 10; i++)
    ASSERT_EQ(lcg_st(&minstd0_rand), minstd0rand());
}

TEST(minstd_Rand, rand) {
  linear_congruential_engine<unsigned long long, 48271, 0, 2147483648>
      minstdrand(1);
  for (int i = 0; i < 10; i++)
    ASSERT_EQ(lcg_st(&minstd_Rand), minstdrand());
}

TEST(randu_rand, rand) {
  linear_congruential_engine<unsigned long long, 65539, 0, 2147483648>
      randurand(1);
  for (int i = 0; i < 10; i++)
    ASSERT_EQ(lcg_st(&randu_rand), randurand());
}

TEST(stand_rand, rand) {
  linear_congruential_engine<unsigned long long, 1103515245, 12345, 2147483648>
      standrand(1);
  for (int i = 0; i < 10; i++)
    ASSERT_EQ(lcg_st(&stand_rand), standrand());
}

TEST(numericalrecipes_rand, rand) {
  linear_congruential_engine<unsigned long long, 1664525, 1013904223,
                             4294967296>
      numericalrecipesrand(1);
  for (int i = 0; i < 10; i++)
    ASSERT_EQ(lcg_st(&numericalrecipes_rand), numericalrecipesrand());
}

/* Bug with clang https://llvm.org/bugs/show_bug.cgi?id=27839 */
/*
TEST(posix48_rand, rand)
{
    linear_congruential_engine<uint64_t, 25214903917, 11, 281474976710656>
posix48rand(0); for(int i = 0; i < 10; i++) ASSERT_EQ(lcg_st(&posix48_rand),
posix48rand());

}*/

TEST(MeresnneTwister32, rand) {
  mt19937 prng(5489);
  mt_seed_32(5489);

  for (int i = 0; i < 10; i++)
    ASSERT_EQ(mt19937_32(), prng());
}

TEST(MeresnneTwister64, rand) {
  mt19937_64 prng(5489);
  mt_seed_64(5489);

  for (int i = 0; i < 10; i++)
    ASSERT_EQ(l_mt19937_64(), prng());
}

TEST(lcg, rand) {
  unsigned long long seed = 123456;
  lcg_seed(seed);

  ASSERT_EQ(lcg(0, 0, 0), 0);
  ASSERT_EQ(get_seed(), 0);
  ASSERT_EQ(lcg(0, 123456, 65432), 57856);
  ASSERT_EQ(get_seed(), 123456);
  ASSERT_EQ(lcg(50, 987654, 456321), 279168);
  ASSERT_EQ(get_seed(), 7160454);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
