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

#ifndef DYABL_MERSENNETWISTER_H__
#define DYABL_MERSENNETWISTER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void mt_seed_32(uint32_t seed);
uint32_t mt19937_32(void);

void mt_seed_64(uint64_t seed);
uint64_t l_mt19937_64(void);

#ifdef __cplusplus
}
#endif

#endif /* ----- #ifndef MERSENNETWISTER_H__  ----- */
