/**
 *
 * \file Array.h
 *
 * \brief Some functions utility
 *
 * \version 0.1
 * \date  17.07.2015 14:26:27
 * \author Loïc BARTOLETTI, l.bartoletti@free.fr
 *
 */

#ifndef DYABL_ARRAY_H__
#define DYABL_ARRAY_H__

#include "Utils.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef int (*compar)(const void *, const void *);

/**
 * @brief Search if a value is in an array (as posix's lsearch)
 *
 * @param[in] val Value searched
 * @param[in] arr The array
 * @param[in] nb_elmt Number of elements into the array
 * @param[in] size Size of element of array
 *
 * @return Pointer to the first occurence found or added if not found
 */
void *LinearSearch(const void *val, const void *arr, size_t *nb_elmt,
                   size_t size, int (*compar)(const void *, const void *));

/**
 * @brief Search if a value is in an array (as posix's lfind)
 *
 * @param[in] val Value searched
 * @param[in] arr The array
 * @param[in] nb_elmt Number of elements into the array
 * @param[in] size Size of element of array
 *
 * @return Index of the first occurence found or NULL if not found
 */
void *LinearFind(const void *val, const void *arr, size_t *nb_elmt, size_t size,
                 int (*compar)(const void *, const void *));

/**
 * @brief Search if a value is in an array using Binary Search (as posix's
 * bsearch)
 *
 * @param[in] val Value searched
 * @param[in] arr The array
 * @param[in] nb_elmt Number of elements into the array
 * @param[in] size Size of element of array
 *
 * @return Pointer on the occurence found or NULL if not found
 */
void *BinarySearch(const void *val, const void *arr, size_t nb_elmt,
                   size_t size, int (*compar)(const void *, const void *));

/**
 * @brief Same as BinarySearch but using recursivity
 *
 * @param[in] val Value searched
 * @param[in] arr The array
 * @param[in] start Start of search
 * @param[in] nb_elmt Number of elements into the array
 * @param[in] size Size of element of array
 *
 * @return Pointer on occurence found or NULL if not found
 */
void *BinarySearch_rec(const void *val, const void *arr, size_t start,
                       size_t nb_elmt, size_t size,
                       int (*compar)(const void *, const void *));

/**
 * @brief Same as BinarySearch_rec but using an interpolation function to cut
 * tab in two instead of middle of the array
 *
 * @param[in] arr The array
 * @param[in] val Value searched
 * @param[in] start Start of search
 * @param[in] end End of search
 *
 * @return Pointer on occurence found or -1 if not found
 */
int *InterpolationSearch_rec(int val, int *arr, int start, int end,
                             int (*compar)(const void *, const void *));

/**
 * @brief Count number of elements with @p val value into the array. If the
 * array is sorted, use countBinary
 *
 * @param[in] val Value searched
 * @param[in] arr The array
 * @param[in] nb_elmt Number of elements into the array
 * @param[in] size Size of element of array
 * @param[in] compar Comparison functions
 *
 * @return Number of value counted into the array ; -1 if not found
 */
size_t CountLinear(const void *val, const void *arr, size_t nb_elmt,
                   size_t size, int (*compar)(const void *, const void *));

/**
 * @brief Count number of elements (using Binary search) with @p val value into
 * the array. If the array is not sorted, use countLinear
 *
 * @param[in] val Value searched
 * @param[in] arr The array
 * @param[in] nb_elmt Number of elements into the array
 * @param[in] size Size of element of array
 * @param[in] compar Comparison functions
 *
 * @return Number of value counted into the array ; -1 if not found
 */
size_t CountBinary(const void *val, const void *arr, size_t nb_elmt,
                   size_t size, int (*compar)(const void *, const void *));

/**
 * @brief Check if the array is sorted
 *
 * @param[in] arr The array
 * @param[in] nb_elmt Number of elements into the array
 * @param[in] size Size of element of array
 * @param[in] compar Comparison functions
 *
 * @return Number of value counted into the array ; -1 if not found
 */
bool isSorted(const void *arr, size_t nb_elmt, size_t size,
              int (*compar)(const void *, const void *));

/**
 * @brief Shuffle an array
 *
 * @param[in] arr The array
 * @param[in] nb_elmt Number of elements into the array
 * @param[in] size Size of element of array
 *
 * @return True in case of success, False else
 */
bool Shuffle(void *arr, size_t nb_elmt, size_t size);

#endif /* ----- #ifndef __ARRAY_H_  ----- */
