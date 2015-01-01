#include "Array.h"
#include <string.h>

static void *LinearWork(const void *val, const void *arr, size_t *nb_elmt,
                        size_t size, int (*compar)(const void *, const void *),
                        bool add_flag);

void *LinearSearch(const void *val, const void *arr, size_t *nb_elmt,
                   size_t size, int (*compar)(const void *, const void *)) {

  return (LinearWork(val, arr, nb_elmt, size, compar, true));
}

void *LinearFind(const void *val, const void *arr, size_t *nb_elmt, size_t size,
                 int (*compar)(const void *, const void *)) {
  return (LinearWork(val, arr, nb_elmt, size, compar, false));
}

static void *LinearWork(const void *val, const void *arr, size_t *nb_elmt,
                        size_t size, int (*compar)(const void *, const void *),
                        bool add_flag) {
  const char *end_p;
  const char *p;

  end_p = (const char *)arr + *nb_elmt * size;
  for (p = (const char *)arr; p < end_p; p += size) {
    if (compar(val, (const void *)p) == 0) {
      return DECONST(void *, p);
    }
  }
  if (!add_flag) {
    return NULL;
  }

  memcpy(DECONST(void *, end_p), val, size);
  ++*nb_elmt;

  return DECONST(void *, end_p);
}
