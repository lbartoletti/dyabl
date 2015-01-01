
#ifndef DYABL_DARRAY_H__
#define DYABL_DARRAY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "Utils.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
/**
 * darray
 *
 * Dynamic array
 */
typedef struct darray {
  void **data;    /* !< data of the element in array. */
  size_t size;    /* !< sizeof element into the array */
  size_t nb_elem; /* !< Maximum number of elements into the array. */
  size_t pos;     /* !< max position used into the array. */
  size_t step;    /* !< step to increase the array. */
  int (*compar)(const void *, const void *); /* function to compare elements. */
} DArray;

typedef enum config_error {
  DARRAY_ERR = -1,
  DARRAY_ERR_NOARRAY = -2,
  DARRAY_ERR_NOCOMPAR = -3,
  DARRAY_ERR_MEMORY = -4,
  DARRAY_ERR_NOTRANGE = -5,
  DARRAY_ERR_EXIST = -6,
  DARRAY_SUCCESS = 1
} darray_error_t;

static errordesc darray_errors[] = {
    {DARRAY_ERR, (char *)"Error"},
    {DARRAY_ERR_NOARRAY, (char *)"No array"},
    {DARRAY_ERR_NOCOMPAR, (char *)"No compar function setted"},
    {DARRAY_ERR_MEMORY, (char *)"Cannot allocate memory"},
    {DARRAY_ERR_NOTRANGE, (char *)"Domain not in range"},
    {DARRAY_ERR_EXIST, (char *)"Data exists"},
    {DARRAY_SUCCESS, (char *)"No error"}};
/*!
 \def DEFAULT_STEP Default value to step. Used when step equal 0 in set step
 functions.
 */
#define DEFAULT_STEP 1

/********
 * INIT *
 ********/
/**
 * @brief Create a new dynamic array.
 *
 * @return A new empty array, else NULL.
 *
 * An empty array is define as this:
 * data = NULL;
 * nb_elem = 0;
 * size = size;
 * pos = 0;
 * step = DEFAULT_STEP;
 * compar = NULL;
 */
DArray *DArray_new(size_t size);

/**
 * @brief Create a new sized dynamic array.
 *
 * @return A new empty array, else NULL.
 */
DArray *DArray_sized_new(size_t size, size_t nb_elem);

/**
 * @brief Create a new sized dynamic array with step defined.
 *
 * @return A new empty array, else NULL.
 */
DArray *DArray_sized_step_new(size_t size, size_t nb_elem, size_t step);

/**
 * @brief Create a new dynamic array with full initialisation
 *
 * @return A new empty array, else NULL.
 */
DArray *DArray_new_init(size_t size, size_t nb_elem, size_t step,
                        int (*compar)(const void *, const void *));

/***********
 * SETTERS *
 ***********/
/**
 * @brief Set compare function to DArray.
 */
void DArray_set_compar(DArray *array,
                       int (*compar)(const void *, const void *));

/**
 * @brief Set new step to DArray.
 *
 * If step <= 0, DArray_set_step automatically set step to DEFAULT_STEP
 */
void DArray_set_step(DArray *array, size_t step);

/*********
 * UTILS *
 *********/

#define DArray_isEmpty(array)                                                  \
  ((((array)->pos) == 0) ? true : false) /* !< Return true if array is empty   \
                                          */
#define DArray_isFull(array)                                                   \
  ((((array)->pos + 1) >= ((array)->nb_elem))                                  \
       ? true                                                                  \
       : false) /* !< Return true if array is "full" (not yet resized) */

/********
 * SIZE *
 ********/

#define DArray_size(array)                                                     \
  ((array)->pos) /* !< Return actual size used of the array. */
#define DArray_max_size(array)                                                 \
  ((array)->nb_elem) /* !< Return the maximal size of the array. */

/**
 * @brief Grow array (nb_elem) to new size.
 *
 * @param[in] array The array
 * @param[in] size New size of array
 * @param[in] use_step Do you want that size is the new size, or did we go to
 * multiple of array->step ? example: array->nb_elem = 2 array->step = 2
 *
 * considerer size = 5
 * if use_step is false:
 * array->nb_elem = 5
 * if use_step is true:
 * array->nb_elem = 6
 *
 * considerer size = 4
 * array->nb_elem will be 4 even if use_step is true or false
 * @return DARRAY_ERR_NOARRAY if array is NULL; DARRAY_ERR_MEMORY mem alloc
 * error; DARRAY_ERR if size <= array->nb_elem; DARRAY_SUCCESS in case of
 * success.
 */
int DArray_grow(DArray *array, size_t size, bool use_step);

/**
 * @brief Shrink array (nb_elem) to new size.
 *
 * @param[in] array The array
 * @param[in] size New size of array
 * @param[in] use_step Do you want that size is the new size, or did we go to
 * multiple of array->step ? example: array->nb_elem = 10 array->step = 2
 *
 * considerer size = 9
 * if use_step is false:
 * array->nb_elem = 9
 * if use_step is true:
 * array->nb_elem = 8
 *
 * considerer size = 8
 * array->nb_elem will be 8 even if use_step is true or false
 * @return DARRAY_ERR_NOARRAY if array is NULL; DARRAY_ERR_MEMORY mem alloc
 * error; DARRAY_ERR if size >= array->nb_elem; DARRAY_SUCCESS in case of
 * success.
 */
int DArray_shrink(DArray *array, size_t size, bool use_step);

/**
 * @brief Shrink array size to max used elements.
 *
 * @return true in case of success, else false;
 */
bool DArray_shrink_to_fit(DArray *array);

/*********
 * Clear *
 *********/

/**
 * @brief Flush an array.
 */
void DArray_flush(DArray *array);

/**
 * @brief Flush and free an array (not usable after).
 */
void DArray_free(DArray *array);

/*******
 * Add *
 *******/

/**
 * @brief Add a new element on to the end of the array.
 *
 * @param[in] array The array to append an element with data.
 * @param[in] data Data to insert into the array.
 *
 * @return DARRAY_ERR at error ; DARRAY_SUCCESS if ok ; DARRAY_ERR_NOARRAY if
 * array doesn't exist.
 */
int DArray_append(DArray *array, const void *data);

/**
 * @brief  Add a new element on to the start of the array.
 *
 * @param[in] array The array to prepend an element with data.
 * @param[in] data Data to insert into the array.
 *
 * @return DARRAY_ERR at error ; DARRAY_SUCCESS if ok ; DARRAY_ERR_NOARRAY if
 * array doesn't exist.
 */
int DArray_prepend(DArray *array, const void *data);

/**
 * @brief  Add a new element at position.
 *
 * @param[in] array The array to prepend an element with data.
 * @param[in] data Data to insert into the array.
 * @param[in] position The position to insert data (can be a negative value like
 * Python).
 *
 * @return DARRAY_ERR at error ; DARRAY_SUCCESS if ok ; DARRAY_ERR_NOARRAY if
 * array doesn't exist ; DARRAY_ERR_NOTRANGE if position is not in domain range.
 */
int DArray_insert_position(DArray *array, const void *data, size_t position);

/**
 * @brief Change an element at position
 *
 * @param[in] array The array to prepend an element with data.
 * @param[in] data Data to insert into the array.
 * @param[in] position The position to insert data (can be a negative value like
 * Python).
 *
 * @return DARRAY_ERR at error ; DARRAY_SUCCESS if ok ; DARRAY_ERR_NOARRAY if
 * array doesn't exist.
 */
int DArray_set_position(DArray *array, const void *data, size_t position);

/**
 * @brief Get an element at position
 *
 * @param[in] array The array to prepend an element with data.
 * @param[in] position The position to insert data (can be a negative value like
 * Python).
 *
 * @return Data element or NULL at error
 */
void *DArray_get_position(DArray *array, size_t position);

/**
 * @brief Add a new element if value is not already in the array.
 *
 * @param[in] array The array to insert a new unique element.
 * @param[in] data Data to insert into the array.
 *
 * @return DARRAY_ERR_NOCOMPAR if compar function is not set ; DARRAY_ERR at
 * error ; DARRAY_SUCCESS on success ; DARRAY_ERR_NOARRAY if array doesn't exist
 * ; DARRAY_ERR_EXIST if data is already in array.
 */
int DArray_unique_insert(DArray *array, const void *data);

/**********
 * Remove *
 **********/

/**
 * @brief Remove the tail of the array, get data for working with it and/or free
 * it.
 *
 * @param[in] array The array to remove an element.
 * @param[out] data The deleted data.
 *
 * @return DARRAY_SUCCESS if ok ; DARRAY_ERR at error.
 */
int DArray_remove_tail(DArray *array, void **data);

/**
 * @brief Remove the head of the array, get data for working with it and/or free
 * it.
 *
 * @param[in] array The array to remove an element.
 * @param[out] data The deleted data.
 *
 * @return DARRAY_SUCCESS if ok ; DARRAY_ERR at error.
 */
int DArray_remove_head(DArray *array, void **data);

/**
 * @brief Remove the current element from a DArray, get data for working with it
 * and/or free it.
 *
 * @param[in] array The array to remove an element
 * @param[out] data The deleted data
 *
 * @return DARRAY_SUCCESS if ok ; DARRAY_ERR at error ; DARRAY_ERR_NOTRANGE if
 * position is not in domain range
 */
int DArray_remove_position(DArray *array, void **data, size_t position);

/**
 * @brief Remove an element from a DArray. If two elements contain the same
 * data, only the firt is removed. If none of the elements contain the data, the
 * DArray is unchanged.
 *
 * @param[in] array The array to remove an element.
 * @param[out] data The data to delete.
 *
 * @return DARRAY_SUCCESS if ok ; DARRAY_ERR if not found ; DARRAY_ERR_NOARRAY
 * if array doesn't exist ; DARRAY_ERR_NOCOMPAR if compar function is not set
 */
int DArray_remove(DArray *array, void **data);

#ifdef __cplusplus
}

#endif

#endif
