#include "Array.h"
#include <stdint.h>

void *BinarySearch(const void *val, const void *arr, size_t nb_elmt,
                   size_t size, int (*compar)(const void *, const void *)) {
  size_t lo;
  size_t up;

  lo = 0;
  up = nb_elmt;
  while (lo <= up) {
    size_t mid = lo + (up - lo) / 2;
    const void *p = (const void *)((const char *)arr + mid * size);
    int cmp = compar(val, p);

    if (cmp < 0) {
      up = mid - 1;
    } else if (cmp > 0) {
      lo = mid + 1;
    } else {
      return DECONST(void *, p);
    }
  }
  return NULL;
}

void *BinarySearch_rec(const void *val, const void *arr, size_t start,
                       size_t nb_elmt, size_t size,
                       int (*compar)(const void *, const void *)) {

  size_t r;
  const void *p;
  int cmp;

  if (start > nb_elmt) {
    return NULL;
  }

  r = start + (nb_elmt - start) / 2;

  p = (const void *)((const char *)arr + r * size);
  cmp = compar(val, p);
  if (cmp == 0) {
    return DECONST(void *, p);
  }
  if (cmp < 0) {
    return BinarySearch_rec(val, arr, start, r - 1, size, compar);
  }
  return BinarySearch_rec(val, arr, r + 1, nb_elmt, size, compar);
}
