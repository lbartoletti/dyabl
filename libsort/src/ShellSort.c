/**
 *
 * \file Sort.c
 *
 * \brief Shell sort functions
 *
 * \version 0.1
 * \date  10.07.2015 06:30:54
 * \author Loïc BARTOLETTI, l.bartoletti@free.fr
 *
 */

/* All codes are adapted from Robert Sedgewick's codes */
/* presented at Fourth Annual European Symposium on Algorithms */
/* Barcelona, September, 1996 */
/* http://www.cs.princeton.edu/~rs/shell/ */
/* http://www.cs.princeton.edu/~rs/shell/driver.c */

#include "Sort.h"
#include <stdlib.h>
#include <string.h>
/*
 * Shell's original
 */
extern int shellSortSh_B(void *t, size_t l, size_t nb_elmt, size_t size,
                         int (*compar)(const void *, const void *)) {
  int compteur = 0;
  size_t i;
  size_t j;
  size_t h;
  void *v = malloc(size);
  if (v == NULL) {
    return -1;
  }

  for (h = 1; h <= (nb_elmt - l) / 4; h = 2 * h) {
  }
  for (; h > 0; h /= 2) {
    for (i = l + h; i < nb_elmt; i++) {
      j = i;
      memcpy(v, (char *)t + i * size, size);

      while (j >= l + h && compar(v, (char *)t + (j - h) * size) < 0) {
        memcpy((char *)t + j * size, (char *)t + (j - h) * size, size);
        j -= h;
        compteur++;
      }
      memcpy((char *)t + j * size, v, size);
      compteur++;
    }
  }
  free(v);
  return compteur;
}

extern int shellSortSh(void *t, size_t nb_elmt, size_t size,
                       int (*compar)(const void *, const void *)) {
  return shellSortSh_B(t, 0, nb_elmt, size, compar);
}

/*
 * Knuth
 */
extern int shellSortKn_B(void *t, size_t l, size_t nb_elmt, size_t size,
                         int (*compar)(const void *, const void *)) {
  int compteur = 0;
  size_t i;
  size_t j;
  size_t h;
  void *v = malloc(size);
  if (v == NULL) {
    return -1;
  }

  for (h = 1; h <= (nb_elmt - l) / 9; h = 3 * h + 1) {
  }
  for (; h > 0; h /= 3) {
    for (i = l + h; i < nb_elmt; i++) {
      j = i;
      memcpy(v, (char *)t + i * size, size);

      while (j >= l + h && compar(v, (char *)t + (j - h) * size) < 0) {
        memcpy((char *)t + j * size, (char *)t + (j - h) * size, size);
        j -= h;
      }
      memcpy((char *)t + j * size, v, size);
    }
  }
  free(v);
  return compteur;
}

extern int shellSortKn(void *t, size_t nb_elmt, size_t size,
                       int (*compar)(const void *, const void *)) {
  return shellSortKn_B(t, 0, nb_elmt, size, compar);
}

/*
 * Gonnet
 */
extern int shellSortGo_B(void *t, size_t l, size_t nb_elmt, size_t size,
                         int (*compar)(const void *, const void *)) {
  int compteur = 0;
  size_t i;
  size_t j;
  size_t h;
  void *v = malloc(size);
  if (v == NULL) {
    return -1;
  }

  for (h = nb_elmt - l + 1; h > 0; h = ((h > 1) && (h < 5)) ? 1 : 5 * h / 11) {
    for (i = l + h; i < nb_elmt; i++) {
      j = i;
      memcpy(v, (char *)t + i * size, size);

      while (j >= l + h && compar(v, (char *)t + (j - h) * size) < 0) {
        memcpy((char *)t + j * size, (char *)t + (j - h) * size, size);
        j -= h;
      }
      memcpy((char *)t + j * size, v, size);
    }
  }
  free(v);
  return compteur;
}

extern int shellSortGo(void *t, size_t nb_elmt, size_t size,
                       int (*compar)(const void *, const void *)) {
  return shellSortGo_B(t, 0, nb_elmt, size, compar);
}

/*
 * Sedgewick
 */
extern int shellSortSe_B(void *t, size_t l, size_t nb_elmt, size_t size,
                         int (*compar)(const void *, const void *)) {
  int compteur = 0;
  size_t i;
  size_t j;
  size_t h;
  size_t k;
  void *v = malloc(size);
  if (v == NULL) {
    return -1;
  }

  for (k = 1; 4 * k * k < (nb_elmt - l); k += k) {
  }

  for (h = (nb_elmt - l) / 4; k > 0; k /= 2, h = k * k - (3 * k) / 2 + 1) {
    for (i = l + h; i < nb_elmt; i++) {
      memcpy(v, (char *)t + i * size, size);
      j = i;
      while (j >= h && compar(v, (char *)t + (j - h) * size) < 0) {
        memcpy((char *)t + j * size, (char *)t + (j - h) * size, size);
        j -= h;
      }
      memcpy((char *)t + j * size, v, size);
    }
  }
  free(v);
  return compteur;
}

extern int shellSortSe(void *t, size_t nb_elmt, size_t size,
                       int (*compar)(const void *, const void *)) {
  return shellSortSe_B(t, 0, nb_elmt, size, compar);
}

/*
 * Pratt
 */
extern int shellSortPr_B(void *t, size_t l, size_t nb_elmt, size_t size,
                         int (*compar)(const void *, const void *)) {
  int compteur = 0;
  size_t incs[28] = {262144, 229376, 200704, 175616, 153664, 134456, 117649,
                     32768,  28672,  25088,  21952,  19208,  16807,  4096,
                     3584,   3136,   2744,   2401,   512,    448,    392,
                     343,    64,     56,     49,     8,      7,      1};
  size_t i;
  size_t j;
  size_t k;
  size_t h;
  void *v = malloc(size);
  if (v == NULL) {
    return -1;
  }

  for (k = 0; k < 28; k++) {
    for (h = incs[k], i = l + h; i < nb_elmt; i++) {
      memcpy(v, (char *)t + i * size, size);
      j = i;
      while (j >= h && compar(v, (char *)t + (j - h) * size) < 0) {
        memcpy((char *)t + j * size, (char *)t + (j - h) * size, size);
        j -= h;
      }
      memcpy((char *)t + j * size, v, size);
    }
  }
  free(v);
  return compteur;
}

extern int shellSortPr(void *t, size_t nb_elmt, size_t size,
                       int (*compar)(const void *, const void *)) {
  return shellSortPr_B(t, 0, nb_elmt, size, compar);
}

/*
 * Incerpi-Sedgewick
 */
extern int shellSortIS_B(void *t, size_t l, size_t nb_elmt, size_t size,
                         int (*compar)(const void *, const void *)) {
  int compteur = 0;
  size_t incs[16] = {1391376, 463792, 198768, 86961, 33936, 13776, 4592, 1968,
                     861,     336,    112,    48,    21,    7,     3,    1};
  size_t i;
  size_t j;
  size_t k;
  size_t h;
  void *v = malloc(size);
  if (v == NULL) {
    return -1;
  }

  for (k = 0; k < 16; k++) {
    for (h = incs[k], i = l + h; i < nb_elmt; i++) {
      memcpy(v, (char *)t + i * size, size);
      j = i;
      while (j >= h && compar(v, (char *)t + (j - h) * size) < 0) {
        memcpy((char *)t + j * size, (char *)t + (j - h) * size, size);
        j -= h;
      }
      memcpy((char *)t + j * size, v, size);
    }
  }
  free(v);
  return compteur;
}

extern int shellSortIS(void *t, size_t nb_elmt, size_t size,
                       int (*compar)(const void *, const void *)) {
  return shellSortIS_B(t, 0, nb_elmt, size, compar);
}
