#include "hashfn.h"

uint32_t SH_add(const void *s) {
  uint32_t hash = 0;
  const char *str = (const char *)s;
  while (*str) {
    hash += (uint32_t)*str++;
  }

  return hash;
}

uint32_t SH_pjw(const void *s) {
  uint32_t hash = 0;
  uint32_t tmp;
  const char *str = (const char *)s;
  while (*str) {
    hash = (hash << 4) + (uint32_t)*str++;
    if ((tmp = (hash & 0xf0000000))) {
      hash ^= (tmp >> 24);
      hash ^= tmp;
    }
  }

  return hash;
}

uint32_t SH_crc(const void *s) {
  uint32_t hash = 0;
  uint32_t high;
  const char *str = (const char *)s;
  while (*str) {
    high = hash & 0xf8000000;
    hash <<= 5;
    hash ^= (high >> 27);
    hash ^= (uint32_t)*str++;
  }

  return hash;
}

uint32_t SH_elf(const void *s) {

  uint32_t hash = 0;
  uint32_t high;
  const char *str = (const char *)s;
  while (*str) {
    hash = (hash << 4) + (uint32_t)*str++;
    if ((high = hash & 0xF0000000)) {
      hash ^= high >> 24;
    }
    hash &= ~high;
  }

  return hash;
}

uint32_t SH_jenkins(const void *s) {
  uint32_t hash = 0;
  const char *str = (const char *)s;

  while (*str) {
    hash += (uint32_t)*str++;
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}
