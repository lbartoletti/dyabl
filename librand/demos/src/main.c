#include <MersenneTwister.h>
#include <Rand.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  Lcg_s bsd_rand = {0, 1103515245, 12345, 2147483647};
  Lcg_s minstd_rand0 = {1, 16807, 0, 2147483647};
  Lcg_s minstd_rand = {1, 48271, 0, 2147483647};
  Lcg_s posix48_rand = {0, 25214903917, 11, 281474976710655};

  printf("BSD RAND\n");
  printf("=====\n");
  for (int i = 0; i < 10; i++) {
    printf("%llu\n", lcg_st(&bsd_rand));
  }

  printf("POSIX48\n");
  printf("=====\n");
  for (int i = 0; i < 10; i++) {
    printf("%llu\n", lcg_st(&posix48_rand));
  }

  printf("MINSTD0\n");
  printf("=====\n");
  for (int i = 0; i < 10; i++) {
    printf("%llu\n", lcg_st(&minstd_rand0));
  }

  printf("MINSTD\n");
  printf("=====\n");
  for (int i = 0; i < 10; i++) {
    printf("%llu\n", lcg_st(&minstd_rand));
  }

  printf("MersenneTwister 32\n");
  printf("======\n");
  mt_seed_32(5489);
  for (int i = 0; i < 10; i++)
    printf("%u\n", mt19937_32());

  printf("MersenneTwister 64\n");
  printf("======\n");
  mt_seed_64(5489);
  for (int i = 0; i < 10; i++)
    printf("%llu\n", l_mt19937_64());

  return 0;
}
