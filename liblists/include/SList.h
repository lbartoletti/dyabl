/**
 *
 * @file SList.h
 *
 * @brief Simple linked list
 *
 * @version 0.1
 * @date  21.06.2015 09:44:23
 * @author Loïc BARTOLETTI, l.bartoletti@free.fr
 *
 */

#ifndef DYABL_SLIST_H__
#define DYABL_SLIST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "Utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef int (*compar)(const void *, const void *);

typedef enum config_error {
  SLIST_ERR = -1,
  SLIST_ERR_NOLIST = -2,
  SLIST_ERR_NOCOMPAR = -3,
  SLIST_ERR_NODESTROY = -4,
  SLIST_ERR_MEMORY = -5,
  SLIST_SUCCESS = 1
} error_t;

static errordesc slist_errors[] = {
    {SLIST_ERR, "Error"},
    {SLIST_ERR_NOLIST, "No list"},
    {SLIST_ERR_NOCOMPAR, "No compar function setted"},
    {SLIST_ERR_NODESTROY, "No destroy function setted"},
    {SLIST_ERR_MEMORY, "Cannot allocate memory"},
    {SLIST_SUCCESS, "No error"}};

/***************************************************************************************/
/* SList structure */
/**
 * slist_elem
 * @brief Element into a single list.
 *
 * slist_elem contain node and data for an element of single list.
 */
typedef struct slist_elem {
  void *data;              /* !< data of the element. */
  struct slist_elem *next; /* !< node of the element. */
} SList_elem;              /* ----------  end of struct
                            * slist_elem  ---------- */

/**
 * slist
 * @brief Single list.
 *
 * slist is the single list structure.
 */
typedef struct slist {
  SList_elem *head;            /* !< head node of the single list. */
  SList_elem *elem;            /* !< current node of the single list. */
  SList_elem *tail;            /* !< tail node of the single list. */
  void (*destroy)(void *data); /* !< function to delete
                                * data element. */
  int (*compar)(const void *, const void *); /* !< function to compar
                                              * elements. */
  long nb_elem;                              /* !< number of elements into the
                                              * single list. */
} SList;                                     /* ----------  end of struct slist
                                              * ---------- */

/***************************************************************************************/
/** SList initialization */
/**
 * @brief Create a single list.
 *
 * @return A new empty list, else NULL.
 */
SList *SList_new(void);

/**
 * @brief Create a single list and initialize functions (delete and compar).
 *
 * @return A new empty list, else NULL.
 */
SList *SList_new_init(void (*destroy)(void *data),
                      int (*compar)(const void *, const void *));

/**
 * @brief Set the destroy function.
 *
 * @param[in] list The list to set the destroy function.
 *
 * @return SLIST_ERR_NOLIST or SLIST_SUCCESS.
 */
int SList_set_destroy(SList *list, void (*destroy)(void *data));

/**
 * @brief Set the comparison function.
 *
 * @param[in] list The list to set the comparison function.
 *
 * @return SLIST_ERR_NOLIST or SLIST_SUCCESS.
 */
int SList_set_compar(SList *list, int (*compar)(const void *, const void *));

/***************************************************************************************/
/* SList add elements */
/**
 * @brief Add a new element on to the end of the list.
 *
 * @param[in] list The list to append an element with data.
 * @param[in] data Data to insert into the list.
 *
 * @return SLIST_ERR_NOLIST, SLIST_ERR_MEMORY or SLIST_SUCCESS.
 */
int SList_append(SList *list, const void *data);

/**
 * @brief  Add a new element on to the start of the list.
 *
 * @param[in] list The list to prepend an element with data.
 * @param[in] data Data to insert into the list.
 *
 * @return SLIST_ERR_NOLIST, SLIST_ERR_MEMORY or SLIST_SUCCESS.
 */
int SList_prepend(SList *list, const void *data);

/**
 * @brief  Add a new element at position.
 *
 * @param[in] list The list to prepend an element with data.
 * @param[in] data Data to insert into the list.
 * @param[in] position The position to insert data (can be a negative value like
 * Python).
 *
 * @return SLIST_ERR_NOLIST, SLIST_ERR_MEMORY, SLIST_ERR or SLIST_SUCCESS.
 */
int SList_insert_position(SList *list, const void *data, long position);

/**
 * @brief Add a new element if value is not already in the list.
 *
 * @param[in] list The list to insert a new unique element.
 * @param[in] data Data to insert into the list.
 *
 * @return SLIST_ERR_NOLIST, SLIST_ERR_MEMORY, SLIST_ERR, SLIST_SUCCESS or 0 if
 * nothing was inserted.
 */
int SList_unique_insert(SList *list, const void *data);

/***************************************************************************************/
/* Slist remove elements */
/**
 * @brief Remove the tail of the list, get data for working with it and/or free
 * it.
 *
 * @param[in] list The list to remove an element.
 * @param[out] data The deleted data.
 *
 * @return SLIST_ERR_NOLIST, SLIST_ERR or SLIST_SUCCESS.
 */
int SList_remove_tail(SList *list, void **data);

/**
 * @brief Remove the head of the list, get data for working with it and/or free
 * it.
 *
 * @param[in] list The list to remove an element.
 * @param[out] data The deleted data.
 *
 * @return SLIST_ERR_NOLIST, SLIST_ERR or SLIST_SUCCESS.
 */
int SList_remove_head(SList *list, void **data);

/**
 * @brief Remove the current element from a SList, get data for working with it
 * and/or free it.
 *
 * @param[in] list The list to remove an element
 * @param[out] data The deleted data
 *
 * @return SLIST_ERR_NOLIST, SLIST_ERR or SLIST_SUCCESS.
 */
int SList_remove_next(SList *list, void **data);

/**
 * @brief Remove an element from a SList. If two elements contain the same data,
 * only the firt is removed. If none of the elements contain the data, the SList
 * is unchanged.
 *
 * @param[in] list The list to remove an element.
 * @param[out] data The data to delete.
 *
 * @return SLIST_ERR_NOLIST, SLIST_ERR_NOCOMPAR, SLIST_ERR or SLIST_SUCCESS.
 */
int SList_remove(SList *list, const void *data);

/**
 * @brief Same as SList_remove but, removes all elements contains the data.
 *
 * @param[in] list The list to remove an element.
 * @param[in] data The data to delete.
 *
 * @return SLIST_ERR_NOLIST, SLIST_ERR_NOCOMPAR, SLIST_ERR or SLIST_SUCCESS.
 */
int SList_remove_all(SList *list, const void *data);

/***************************************************************************************/
/* SList free and delete single list */
/**
 * @brief Free all of the memory used by a SList and delete it
 *
 * @param list The list to free'd
 *
 * @return SLIST_ERR_NOLIST or SLIST_SUCCESS.
 */
int SList_free(SList *list);

/**
 * @brief Empty and free all of the memory used by a SList
 *
 * @param list The list to empty
 *
 * @return SLIST_ERR_NOLIST or SLIST_SUCCESS.
 */
int SList_empty(SList *list);

/***************************************************************************************/
/* SList macros */
/** Return the number of elements in a SList */
#define SList_length(list) ((list) ? (list)->nb_elem : 0)
/** Gets the first element in a SList */
#define SList_head(list) ((list)->head)
/** Gets the last element in a SList */
#define SList_tail(list) ((list)->tail)
/** Is the element is the first (head) of the list ? */
#define SList_isFirst(list, elem) ((list)->head == (elem))
/** Is the element is the last (tail) of the list ? */
#define SList_isLast(list, elem) ((list)->tail == (elem))
/** Is the list empty ?*/
#define SList_isEmpty(list) (SList_length((list)) == 0 ? true : false)

/***************************************************************************************/
/* SList utility */
/**
 * @brief  Gets the position of the element containing the given data (starting
 * from 0)
 *
 * @param[in] list The list
 * @param[in] data The data to be searched
 *
 * @return position of the first element containing the given data ; SLIST_ERR
 * if not found, SLIST_ERR_NOLIST, SLIST_ERR_NOCOMPAR.
 */
long SList_index(SList *list, const void *data);

/**
 * @brief  Gets the element containing the given data
 *
 * @param[in] list The list
 * @param[in] data The data to be searched
 *
 * @return element containing the given data ; NULL if not
 * found or errors
 */
SList_elem *SList_find(SList *list, const void *data);

/**
 * @brief Do func for each elements in list
 *
 * @param[in] list The list
 * @param[in] func The function
 *
 * @return SLIST_ERR_NOLIST or SLIST_SUCCESS.
 */
int SList_foreach(SList *list, void (*func)(void **));

/**
 * @brief Do func for each (safe) elements in list
 *
 * @param list The list
 * @param func The function
 *
 * @return SLIST_ERR_NOLIST or SLIST_SUCCESS.
 *
 */
int SList_foreach_safe(SList *list, void (*func)(const void *));

/**
 * @brief Create a reversed single list
 *
 * @param list The list
 *
 * @return The reversed list or NULL
 */
SList *SList_reverse(SList *list);

/**
 * @brief Sort (in place) the list
 *
 * @param list The list
 *
 * @return SLIST_ERR_NOLIST, SLIST_ERR_NOCOMPAR OR SLIST_SUCCESS.
 */
int SList_sort(SList *list);

#ifdef __cplusplus
}

#endif

#endif
