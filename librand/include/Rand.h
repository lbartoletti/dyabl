/**
 *
 * \file Rand.h
 *
 * \brief Rand
 *
 * \version 0.1
 * \date  12.08.2016 22:32:50
 * \author Loïc BARTOLETTI, l.bartoletti@free.fr
 *
 */

#ifndef DYABL_RAND_H__
#define DYABL_RAND_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

typedef struct lcg_s {
  unsigned long long seed;
  unsigned long long a;
  unsigned long long c;
  unsigned long long m;
} Lcg_s;

unsigned long long lcg(unsigned long long a, unsigned long long c,
                       unsigned long long m);
unsigned long long lcg_st(Lcg_s *l);
void lcg_seed(unsigned long long s);
unsigned long long get_seed(void);

#ifdef __cplusplus
}
#endif

#endif /* ----- #ifndef Rand_H_  ----- */
