#include "Array.h"
#include "Sort.h"

extern int bogoSort(void *t, size_t nb_elmt, size_t size,
                    int (*compar)(const void *, const void *)) {
  int compteur = 0;
  while (!isSorted(t, nb_elmt, size, compar)) {
    if (Shuffle(t, nb_elmt, size)) {
      compteur++;
    } else {
      return -1;
    }
  }

  return compteur;
}
