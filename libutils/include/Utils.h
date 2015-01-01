/**
 *
 * \file Utils.h
 *
 * \brief Some functions utility
 *
 * \version 0.1
 * \date  17.07.2015 14:26:27
 * \author Loïc BARTOLETTI, l.bartoletti@free.fr
 *
 */

#ifndef DYABL_UTILS_H_
#define DYABL_UTILS_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/** Return a non const variable
 * Taken from FreeBSD
 */
#ifndef DECONST
#define DECONST(type, var) ((type)(uintptr_t)(const void *)(var))
#endif

/**
 * @brief generic swap function
 *
 * @return -1 error (malloc) ; 1 success
 */
extern int genswap(void *a, void *b, size_t size);

/* Comparaisons */

int comparInt(const void *a, const void *b);
int comparDouble(const void *a, const void *b);

int comparIntptr(const void *a, const void *b);

typedef struct errordesc_t {
  int code;
  const char *message;
} errordesc;

#endif /* ----- #ifndef __ALGO_UTILS_H_  ----- */
