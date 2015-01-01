#include "Array.h"

bool isSorted(const void *arr, size_t nb_elmt, size_t size,
              int (*compar)(const void *, const void *)) {
  size_t i = 0;

  if (nb_elmt == 1) {
    return true;
  }

  while (i < nb_elmt - 1) {
    const int cmp = compar((const char *)arr + i * size,
                           (const char *)arr + (i + 1) * size);
    if (cmp > 0) {
      return false;
    }
    i++;
  }

  return true;
}
