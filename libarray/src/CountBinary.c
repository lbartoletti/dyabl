#include "Array.h"
#include <assert.h>

#include <stdio.h>
static size_t BinarySearch_count(size_t count, const void *val, const void *arr,
                                 size_t start, size_t nb_elmt, size_t size,
                                 int (*compar)(const void *, const void *)) {

  size_t r;
  const void *p;
  int cmp;

  if (start > nb_elmt) {
    return count;
  }

  r = (start + nb_elmt) / 2;

  p = (const void *)((const char *)arr + r * size);
  cmp = compar(val, p);
  if (cmp == 0) {
    count++;
    size_t left = r - 1;
    size_t right = r + 1;
    do {
      if (compar(val, (const char *)arr + left * size) == 0) {
        count++;
        left--;
      } else {
        break;
      }
    } while (left != 0);

    do {
      if (compar(val, (const char *)arr + right * size) == 0) {
        count++;
        right++;
      } else {
        break;
      }
    } while (right < nb_elmt);
  } else {
    if (cmp > 0) {
      return BinarySearch_count(count, val, arr, start, r - 1, size, compar);
    }
    {
      return BinarySearch_count(count, val, arr, r + 1, nb_elmt, size, compar);
    }
  }

  return count;
}

size_t CountBinary(const void *val, const void *arr, size_t nb_elmt,
                   size_t size, int (*compar)(const void *, const void *)) {
  return BinarySearch_count(0, val, arr, 0, nb_elmt, size, compar);
}
