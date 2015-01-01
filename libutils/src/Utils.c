/**
 *
 * \file Utils.c
 *
 * \brief Some functions utility
 *
 * \version 0.1
 * \date  17.07.2015 14:26:27
 * \author Loïc BARTOLETTI, l.bartoletti@free.fr
 *
 */

#include "Utils.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

extern int genswap(void *const a, void *const b, size_t size) {
  void *const temp = malloc(size);
  if (temp == NULL) {
    return -1;
  }
  memcpy(temp, a, size);
  memcpy(a, b, size);
  memcpy(b, temp, size);
  free(temp);

  return 1;
}

int comparInt(const void *a, const void *b) {
  int left = *(const int *)a;
  int right = *(const int *)b;
  return ((left > right) - (left < right));
}

int comparIntptr(const void *a, const void *b) {

  intptr_t left = (intptr_t)a;
  intptr_t right = (intptr_t)b;
  return ((left > right) - (left < right));
}

int comparDouble(const void *a, const void *b) {
  double left = *(const double *)a;
  double right = *(const double *)b;
  return ((left > right) - (left < right));
}
