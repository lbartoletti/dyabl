/**
 *
 * \file Sort.h
 *
 * \brief Generic sort functions
 *
 * \version 0.1
 * \date  10.07.2015 06:30:20
 * \author Loïc BARTOLETTI, l.bartoletti@free.fr
 *
 */

#ifndef DYABL_SORT_H__
#define DYABL_SORT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

extern int bubbleSort(void *t, size_t nb_elmt, size_t size,
                      int (*compar)(const void *, const void *));
extern int insertionSort(void *t, size_t nb_elmt, size_t size,
                         int (*compar)(const void *, const void *));
extern int quickSort(void *t, size_t nb_elmt, size_t size,
                     int (*compar)(const void *, const void *));
extern int selectionSort(void *t, size_t nb_elmt, size_t size,
                         int (*compar)(const void *, const void *));
extern int mergeSort(void *t, size_t nb_elmt, size_t size,
                     int (*compar)(const void *, const void *));
extern int stoogeSort(void *t, size_t nb_elmt, size_t size,
                      int (*compar)(const void *, const void *));
extern int gnomeSort(void *t, size_t nb_elmt, size_t size,
                     int (*compar)(const void *, const void *));
extern int integerSort(int *t, size_t nb_elmt,
                       int (*compar)(const void *, const void *));
extern int bogoSort(void *t, size_t nb_elmt, size_t size,
                    int (*compar)(const void *, const void *));

extern int shellSortSh(void *t, size_t nb_elmt, size_t size,
                       int (*compar)(const void *, const void *));
extern int shellSortGo(void *t, size_t nb_elmt, size_t size,
                       int (*compar)(const void *, const void *));
extern int shellSortKn(void *t, size_t nb_elmt, size_t size,
                       int (*compar)(const void *, const void *));
extern int shellSortSe(void *t, size_t nb_elmt, size_t size,
                       int (*compar)(const void *, const void *));
extern int shellSortPr(void *t, size_t nb_elmt, size_t size,
                       int (*compar)(const void *, const void *));
extern int shellSortIS(void *t, size_t nb_elmt, size_t size,
                       int (*compar)(const void *, const void *));

extern int shellSortSh_B(void *t, size_t l, size_t nb_elmt, size_t size,
                         int (*compar)(const void *, const void *));
extern int shellSortGo_B(void *t, size_t l, size_t nb_elmt, size_t size,
                         int (*compar)(const void *, const void *));
extern int shellSortKn_B(void *t, size_t l, size_t nb_elmt, size_t size,
                         int (*compar)(const void *, const void *));
extern int shellSortSe_B(void *t, size_t l, size_t nb_elmt, size_t size,
                         int (*compar)(const void *, const void *));
extern int shellSortPr_B(void *t, size_t l, size_t nb_elmt, size_t size,
                         int (*compar)(const void *, const void *));
extern int shellSortIS_B(void *t, size_t l, size_t nb_elmt, size_t size,
                         int (*compar)(const void *, const void *));

#ifdef __cplusplus
}
#endif

#endif /* ----- #ifndef __Sort_H_  ----- */
