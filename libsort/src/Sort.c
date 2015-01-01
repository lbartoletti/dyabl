/**
 *
 * \file Sort.c
 *
 * \brief Generic sort functions
 *
 * \version 0.1
 * \date  10.07.2015 06:30:54
 * \author Loïc BARTOLETTI, l.bartoletti@free.fr
 *
 */

#include "Sort.h"
#include "Utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*compar)(const void *, const void *);

extern int bubbleSort(void *t, size_t nb_elmt, size_t size,
                      int (*compar)(const void *, const void *)) {

  bool tri = true;
  int compteur = 0;

  while (tri) {
    tri = false;
    size_t i = 0;
    while (i < nb_elmt - 1) {
      if (compar((char *)t + i * size, (char *)t + (i + 1) * size) > 0) {
        genswap((char *)t + i * size, (char *)t + (i + 1) * size, size);
        tri = true;
        compteur++;
      }
      i++;
    }
  }

  return compteur;
}

extern int insertionSort(void *t, size_t nb_elmt, size_t size,
                         int (*compar)(const void *, const void *))

{
  size_t i = 0;
  int compteur = 0;

  while (i < nb_elmt - 1) {
    if (compar((char *)t + i * size, (char *)t + (i + 1) * size) >= 0) {
      size_t j = i + 1;
      while ((compar((char *)t + j * size, (char *)t + (j - 1) * size) < 0) &&
             j >= 1) {
        genswap((char *)t + j * size, (char *)t + (j - 1) * size, size);
        j--;
        compteur++;
      }
    }
    i++;
  }

  return compteur;
}

/*
 * QuickSort
 */
static size_t partition(void *t, size_t f, size_t l, size_t size, compar cmp,
                        int *compteur) {

  void *pivot = ((char *)t) + (f * size);
  size_t i = f - 1;
  size_t j = l + 1;

  while (1) {
    do {
      j--;
    } while (cmp(((char *)t) + (j * size), pivot) > 0);
    do {
      i++;
    } while (cmp(((char *)t) + (i * size), pivot) < 0);
    if (i < j) {
      genswap(((char *)t) + (j * size), ((char *)t) + (i * size), size);
      (*compteur)++;
    } else {
      return j;
    }
  }
}

static void qSort_rec(void *tab, size_t f, size_t l, size_t size, compar cmp,
                      int *compteur) {
  size_t q;
  if (f < l) {
    q = partition(tab, f, l, size, cmp, compteur);
    qSort_rec(tab, f, q, size, cmp, compteur);
    qSort_rec(tab, q + 1, l, size, cmp, compteur);
  }
}

extern int quickSort(void *t, size_t nb_elmt, size_t size,
                     int (*compar)(const void *, const void *))

{
  int compteur = 0;
  qSort_rec(t, 0, nb_elmt - 1, size, compar, &compteur);
  return compteur;
}

extern int selectionSort(void *t, size_t nb_elmt, size_t size,
                         int (*compar)(const void *, const void *)) {
  int compteur = 0;
  size_t i;
  size_t min;
  size_t j;
  for (i = 0; i < nb_elmt; i++) {
    min = i;
    for (j = i + 1; j < nb_elmt; j++) {
      if (compar((char *)t + j * size, (char *)t + min * size) < 0) {
        min = j;
      }
    }
    if (min != i) {
      genswap((char *)t + i * size, (char *)t + min * size, size);
      compteur++;
    }
  }
  return compteur;
}

/*
 * Merge sort
 */
static void merge_sort(void *tab, size_t f, size_t mid, size_t l, size_t size,
                       compar cmp, int *compteur) {

  size_t i;
  size_t j;
  size_t k;
  size_t n1;
  size_t n2;
  n1 = mid - f + 1;
  n2 = l - mid;

  void **lTab = malloc(n1 * size);
  void **rTab = malloc(n2 * size);
  if (lTab == NULL || rTab == NULL) {
    free(lTab);
    free(rTab);
    return;
  }
  memcpy(lTab, (char *)tab + f * size, size * n1);
  memcpy(rTab, (char *)tab + (mid + 1) * size, size * n2);
  i = j = 0;
  k = f;

  while (i < n1 && j < n2) {
    if (cmp((char *)lTab + i * size, (char *)rTab + j * size) <= 0) {
      memcpy((char *)tab + k * size, (char *)lTab + i * size, size);
      i++;
      (*compteur)++;
    } else {
      memcpy((char *)tab + k * size, (char *)rTab + j * size, size);
      j++;
      (*compteur)++;
    }
    k++;
  }
  while (i < n1) {
    memcpy((char *)tab + k * size, (char *)lTab + i * size, size);
    i++;
    k++;
    (*compteur)++;
  }

  while (j < n2) {
    memcpy((char *)tab + k * size, (char *)rTab + j * size, size);
    j++;
    k++;
    (*compteur)++;
  }

  free(lTab);
  free(rTab);
}
static void mergeSort_rec(void *tab, size_t f, size_t l, size_t size,
                          compar cmp, int *compteur) {
  if (f < l) {
    size_t mid = f + (l - f) / 2;
    mergeSort_rec(tab, f, mid, size, cmp, compteur);
    mergeSort_rec(tab, mid + 1, l, size, cmp, compteur);
    merge_sort(tab, f, mid, l, size, cmp, compteur);
  }
}
extern int mergeSort(void *t, size_t nb_elmt, size_t size,
                     int (*compar)(const void *, const void *)) {
  int compteur = 0;
  mergeSort_rec(t, 0, nb_elmt - 1, size, compar, &compteur);
  return compteur;
}

/*
 * Stooge Sort
 */
static void stoogeSort_rec(void *tab, size_t f, size_t l, size_t size,
                           compar cmp, int *compteur) {
  if (cmp((char *)tab + f * size, (char *)tab + l * size) > 0) {
    genswap((char *)tab + f * size, (char *)tab + l * size, size);
    (*compteur)++;
  }
  if ((l - f + 1) > 2) {
    size_t t = (l - f + 1) / 3;
    stoogeSort_rec(tab, f, l - t, size, cmp, compteur);
    stoogeSort_rec(tab, f + t, l, size, cmp, compteur);
    stoogeSort_rec(tab, f, l - t, size, cmp, compteur);
  }
}

extern int stoogeSort(void *t, size_t nb_elmt, size_t size,
                      int (*compar)(const void *, const void *)) {
  int compteur = 0;
  stoogeSort_rec(t, 0, nb_elmt - 1, size, compar, &compteur);
  return compteur;
}

/*
 * Gnome Sort
 */
extern int gnomeSort(void *t, size_t nb_elmt, size_t size,
                     int (*compar)(const void *, const void *)) {
  size_t i = 0;
  int compteur = 0;
  while (i < nb_elmt) {
    if (i == 0 ||
        (compar((char *)t + (i - 1) * size, (char *)t + i * size) <= 0)) {
      i++;
    } else {
      genswap((char *)t + i * size, (char *)t + (i - 1) * size, size);
      i--;
      compteur++;
    }
  }
  return compteur;
}
