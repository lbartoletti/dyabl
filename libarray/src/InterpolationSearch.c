#include "Array.h"
#include <stdint.h>

int *InterpolationSearch_rec(int val, int *arr, int start, int end,
                             int (*compar)(const void *, const void *)) {
  if (arr[start] == val) {
    return &arr[start];
  }
  if (start == end) {
    return NULL;
  }
  if (arr[start] == arr[end]) {
    return NULL;
  }

  int index =
      start + ((end - start) / (arr[end] - arr[start])) * (val - arr[start]);

  if (arr[index] == val) {
    return &arr[index];
  }
  if (compar(&arr[index], &val) < 0) {
    return InterpolationSearch_rec(val, arr, index + 1, end, compar);
  }
  return InterpolationSearch_rec(val, arr, start, index - 1, compar);
}
