/**
 *
 * @file DList.h
 *
 * @brief Double linked list
 *
 * @version 0.1
 * @date  21.06.2015 09:44:23
 * @author Loïc BARTOLETTI, l.bartoletti@free.fr
 *
 */

#ifndef DYABL_DLIST_H__
#define DYABL_DLIST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "Utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define DLIST_SUCCESS 1
#define DLIST_ERROR (-10)
#define DLIST_ERNOLIST (-11)
#define DLIST_ERNOCMP (-12)
#define DLIST_ERMEMORY (-13)
#define DLIST_ERPOSITION (-14)

typedef int (*compar)(const void *, const void *);

/***************************************************************************************/
/* DList structure */
/**
 * dlist_elem
 * @brief Element into a double linked list.
 *
 * dlist_elem contain node and data for an element of double linked list.
 */
typedef struct dlist_elem {
  struct dlist_elem *prev; /* !<node of the previous element */
  void *data;              /* !< data of the element. */
  struct dlist_elem *next; /* !< node of the next element. */
} DList_elem;              /* ----------  end of struct
                            * dlist_elem  ---------- */

/**
 * dlist
 * @brief Double linked list.
 *
 * dlist is the double linked list structure.
 */
typedef struct dlist {
  DList_elem *head;            /* !< head node of the single list. */
  DList_elem *elem;            /* !< current node of the single list. */
  DList_elem *tail;            /* !< tail node of the single list. */
  void (*destroy)(void *data); /* !< function to delete
                                * data element. */
  int (*compar)(const void *, const void *); /* !< function to compar
                                              * elements. */
  long nb_elem;                              /* !< number of elements into the
                                              * double linked list. */
} DList;                                     /* ----------  end of struct dlist
                                              * ---------- */

/***************************************************************************************/
/** DList initialization */
/**
 * @brief Create a double linked list.
 *
 * @return A new empty list, else NULL.
 */
DList *DList_new(void);
/**
 * @brief Create a double linked list and initialize functions (destroy and
 * compar).
 *
 * @return A new empty list, else NULL.
 */
DList *DList_new_init(void (*destroy)(void *data),
                      int (*compar)(const void *, const void *));

/***************************************************************************************/
/* DList add elements */

/**
 * @brief Add a new element on to the end of the list.
 *
 * @param[in] list The list to append an element with data.
 * @param[in] data Data to insert into the list.
 *
 * @return -1 at error ; 1 if ok ; 0 if list doesn't exist.
 */
int DList_append(DList *list, const void *data);

/**
 * @brief  Add a new element on to the start of the list.
 *
 * @param[in] list The list to prepend an element with data.
 * @param[in] data Data to insert into the list.
 *
 * @return -1 at error ; 1 if ok ; 0 if list doesn't exist.
 */
int DList_prepend(DList *list, const void *data);

/**
 * @brief  Add a new element at position.
 *
 * @param[in] list The list to prepend an element with data.
 * @param[in] data Data to insert into the list.
 * @param[in] position The position to insert data (can be a negative value like
 * Python).
 *
 * @return -1 at error ; 1 if ok ; 0 if list doesn't exist.
 */
int DList_insert_position(DList *list, const void *data, long position);

/**
 * @brief Add a new element if value is not already in the list.
 *
 * @param[in] list The list to insert a new unique element.
 * @param[in] data Data to insert into the list.
 *
 * @return -2: no compar function ; -1: error ; 1: Success ; 0: list doesn't
 * exist.
 */
int DList_unique_insert(DList *list, const void *data);

/***************************************************************************************/
/* Dlist remove elements */
/**
 * @brief Remove the tail of the list, get data for working with it and/or free
 * it.
 *
 * @param[in] list The list to remove an element.
 * @param[in] data The deleted data.
 *
 * @return 1 if ok ; -1 at error.
 */
int DList_remove_tail(DList *list, void **data);

/**
 * @brief Remove the head of the list, get data for working with it and/or free
 * it.
 *
 * @param[in] list The list to remove an element.
 * @param[in] data The deleted data.
 *
 * @return 1 if ok ; -1 at error.
 */
int DList_remove_head(DList *list, void **data);

/**
 * @brief Remove the current element from a DList, get data for working with it
 * and/or free it.
 *
 * @param[in] list The list to remove an element
 * @param[in] data The deleted data
 *
 * @return 1 if ok ; -1 at error
 */
int DList_remove_next(DList *list, void **data);

/**
 * @brief Remove an element from a DList. If two elements contain the same data,
 * only the firt is removed. If none of the elements contain the data, the DList
 * is unchanged.
 *
 * @param[in] list The list to remove an element.
 * @param[in] data The data to delete.
 *
 * @return 1 if ok ; -1 if not found ; -2 if list is NULL or compar function is
 * not set
 */
int DList_remove(DList *list, const void *data);

/**
 * @brief Same as DList_remove but, removes all elements contains the data.
 *
 * @param[in] list The list to remove an element.
 * @param[in] data The data to delete.
 *
 * @return 1 if ok ; -1 if not found ; -2 if list is NULL or compar function is
 * not set
 */
int DList_remove_all(DList *list, const void *data);

/***************************************************************************************/
/* DList free and delete double linked list */
/**
 * @brief Free all of the memory used by a DList and delete it
 *
 * @param list The list to free'd
 */
void DList_free(DList *list);
/**
 * @brief Empty and free all of the memory used by a DList
 *
 * @param list The list to empty
 */
void DList_empty(DList *list);

/***************************************************************************************/
/* DList macros */
/** Return the number of elements in a DList */
#define DList_length(list) ((list) ? (list)->nb_elem : 0)
/** Gets the first element in a DList */
#define DList_head(list) ((list)->head)
/** Gets the last element in a DList */
#define DList_tail(list) ((list)->tail)
/** Is the element is the first (head) of the list ? */
#define DList_isFirst(list, elem) ((list)->head == (elem))
/** Is the element is the last (tail) of the list ? */
#define DList_isLast(list, elem) ((list)->tail == (elem))
/** Is the list empty ?*/
#define DList_isEmpty(list) (DList_length((list)) == 0 ? true : false)

/***************************************************************************************/
/* DList utility */
/**
 * @brief  Gets the position of the element containing the given data (starting
 * from 0)
 *
 * @param[in] list The list
 * @param[in] data The data to be searched
 *
 * @return position of the first element containing the given data ; -1 if not
 * found ; -2 if list is NULL or compar function is not set
 */
long DList_index(DList *list, const void *data);

/**
 * @brief Do func for each elements in list
 *
 * @param[in] list The list
 * @param[in] func The function
 */
void DList_foreach(DList *list, void (*func)(void **));

/**
 * @brief Do func for each elements in list
 *
 * @param[in] list The list
 * @param[in] func The function
 */
void DList_reverse_foreach(DList *list, void (*func)(void **));

/**
 * @brief Do func for each (safe) elements in list (reverse order)
 *
 * @param[in] list The list
 * @param[in] func The function
 *
 */
void DList_foreach_safe(DList *list, void (*func)(const void *));
/**
 * @brief Do func for each (safe) elements in list (reverse order)
 *
 * @param[in] list The list
 * @param[in] func The function
 *
 */
void DList_reverse_foreach_safe(DList *list, void (*func)(const void *));

/**
 * @brief Create a reversed single list
 *
 * @param[in] list The list
 *
 * @return The reversed list
 */
DList *DList_reverse(DList *list);

/**
 * @brief Sort (in place) the list
 *
 * @param list The list
 *
 */
void DList_sort(DList *list);

#ifdef __cplusplus
}

#endif

#endif
