/**
 *
 * @file OHashTab.h
 *
 * @brief Open Hash Tab
 *
 * @version 0.1
 * @date  24.01.2017
 * @author Loïc BARTOLETTI, l.bartoletti@free.fr
 *
 */

#ifndef DYABL_OPEN_HASH_TABLE_H__
#define DYABL_OPEN_HASH_TABLE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
typedef int (*compar_fn_t)(const void *, const void *);
typedef void (*destroy_fn_t)(void *);

#ifdef __cplusplus
}
#endif

#endif
