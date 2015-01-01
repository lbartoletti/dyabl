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

#include "Rand.h"
#include <stdio.h>
#include <stdlib.h>

static unsigned long long Rseed = 0;

unsigned long long lcg_st(Lcg_s *l) {
  return ((l->seed = l->seed * l->a + l->c) & l->m);
}

void lcg_seed(unsigned long long s) { Rseed = s; }

unsigned long long get_seed(void) { return Rseed; }

/*x % 2n == x & (2n - 1)*/
unsigned long long lcg(unsigned long long a, unsigned long long c,
                       unsigned long long m) {
  return ((Rseed = Rseed * a + c) & m);
}
