#include "MersenneTwister.h"
#include <stdint.h>
#include <stdio.h>

static uint32_t w = 32;
static uint32_t n = 624;
static uint32_t m = 397;
// static uint32_t r = 31;
static uint32_t a = 0x9908b0df;
static uint32_t u = 11;
static uint32_t d = 0xFFFFFFFF;
static uint32_t s = 7;
static uint32_t b = 0x9D2C5680;
static uint32_t t = 15;
static uint32_t c = 0xEFC60000;
static uint32_t l = 18;
static uint32_t f = 1812433253;
static uint32_t mti = 625;
static uint32_t lower_mask = 0x7FFFFFFF; // (1 << r) -1
static uint32_t upper_mask = 0x80000000;
static uint32_t mt[624];

static void mt_twist(void) {
  for (uint32_t i = 0; i < n; i++) {
    uint32_t x = ((mt[i] & upper_mask) + (mt[(i + 1) % n] & lower_mask));
    mt[i] = mt[(i + m) % n] ^ x >> 1;
    if (x % 2 != 0) {
      mt[i] = mt[i] ^ a;
    }
  }
  mti = 0;
}

void mt_seed_32(uint32_t seed) {
  mti = n;
  mt[0] = seed;
  for (uint32_t i = 1; i < n; i++) {
    mt[i] = (f * (mt[i - 1] ^ (mt[i - 1] >> (w - 2))) + i);
  }
}

uint32_t mt19937_32(void) {
  if (mti >= n) {
    if (mti > n) {
      mt_seed_32(5489);
    }
    mt_twist();
  }

  uint32_t y = mt[mti];
  y ^= ((y >> u));
  y ^= ((y << s) & b);
  y ^= ((y << t) & c);
  y ^= ((y >> l));

  mti++;

  return d & y;
}
