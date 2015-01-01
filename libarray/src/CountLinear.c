#include "Array.h"

size_t CountLinear(const void *val, const void *arr, size_t nb_elmt,
                   size_t size, int (*compar)(const void *, const void *)) {
  size_t i;
  size_t count = 0;
  for (i = 0; i < nb_elmt; i++) {
    if (compar(val, (const char *)arr + i * size) == 0) {
      count++;
    }
  }

  return count;
}
