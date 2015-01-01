/**
 *
 * \file Sort.c
 *
 * \brief Integer sort function
 *
 * \version 0.1
 * \date  10.07.2015 06:30:54
 * \author Loïc BARTOLETTI, l.bartoletti@free.fr
 *
 */

#include "Sort.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

extern int integerSort(int *t, size_t nb_elmt,
                       int (*compar)(const void *, const void *)) {
  if (nb_elmt <= 1) {
    return 0;
  }

  size_t size = sizeof(int);
  int max = t[0];
  size_t i = 1;
  while (i < nb_elmt) {
    if (compar((char *)t + i * size, (char *)t + (i - 1) * size) > 0) {
      max = t[i];
    }
    i++;
  }

  int *ts = malloc((size_t)max * size);
  if (!ts) {
    return -1;
  }

  memset(ts, 0, (size_t)max * size);

  i = 0;
  while (i < nb_elmt) {
    ts[t[i]]++;
    i++;
  }

  i = 0;
  int it_t = 0;
  int compteur = 0;
  int j;
  int nb_int;
  while ((int)i <= max) {
    nb_int = 0;
    j = ts[i];
    while (nb_int < j) {
      t[it_t] = (int)i;
      compteur++;
      nb_int++;
      it_t++;
    }
    i++;
  }

  free(ts);
  return compteur;
}
