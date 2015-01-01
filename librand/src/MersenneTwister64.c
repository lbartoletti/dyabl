#include "MersenneTwister.h"
#include <stdint.h>
#include <stdio.h>

static uint64_t w = 64;
static uint64_t n = 312;
static uint64_t m = 156;
// static uint64_t r = 31;
static uint64_t a = 0xB5026F5AA96619E9;
static uint64_t u = 29;
static uint64_t d = 0x5555555555555555;
static uint64_t s = 17;
static uint64_t b = 0x71D67FFFEDA60000;
static uint64_t t = 37;
static uint64_t c = 0xFFF7EEE000000000;
static uint64_t l = 43;
static uint64_t f = 6364136223846793005;
static uint64_t mti = 311;
static uint64_t lower_mask = 0x7FFFFFFF; // (1<<r)-1
static uint64_t upper_mask = 0xFFFFFFFF80000000;
static uint64_t mt[312];

static void mt_twist(void) {
  for (uint64_t i = 0; i < n; i++) {
    uint64_t x = ((mt[i] & upper_mask) + (mt[(i + 1) % n] & lower_mask));
    mt[i] = mt[(i + m) % n] ^ x >> 1;
    if (x % 2 != 0) {
      mt[i] = mt[i] ^ a;
    }
  }
  mti = 0;
}

void mt_seed_64(uint64_t seed) {
  mt[0] = seed;
  for (mti = 1; mti < n; mti++) {
    mt[mti] = (f * (mt[mti - 1] ^ (mt[mti - 1] >> (w - 2))) + mti);
  }
}

uint64_t l_mt19937_64(void) {
  if (mti >= n) {
    if (mti > n) {
      mt_seed_64(5489);
    }
    mt_twist();
  }

  uint64_t y = mt[mti];
  y ^= ((y >> u) & d);
  y ^= ((y << s) & b);
  y ^= ((y << t) & c);
  y ^= ((y >> l));

  mti++;

  return y;
}
