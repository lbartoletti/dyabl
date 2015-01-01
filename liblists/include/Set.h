/**
 *
 * \file Set.h
 *
 * \brief Set using SList
 *
 * \version 0.1
 * \date  02.04.2016 15:00:35
 * \author Loïc BARTOLETTI, l.bartoletti@free.fr
 *
 */

#ifndef DYABL_SET_H__
#define DYABL_SET_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "SList.h"

/** @brief Define Set as a Single Linked List */
typedef SList Set;
Set *Set_new(void (*destroy)(void *data),
             int (*compar)(const void *, const void *));
void Set_destroy(Set *set);
int Set_insert(Set *set, const void *data);
int Set_remove(Set *set, const void *data);
Set *Set_union(Set *set1, Set *set2);
Set *Set_intersection(Set *set1, Set *set2);
Set *Set_difference(Set *set1, Set *set2);
Set *Set_symetric_difference(Set *set1, Set *set2);
bool Set_isMember(Set *set, const void *data);
bool Set_isSubset(Set *set1, Set *set2);
bool Set_isSuperset(Set *set1, Set *set2);
bool Set_isDisjoint(Set *set1, Set *set2);
bool Set_isEqual(Set *set1, Set *set2);
bool Set_isEmpty(Set *set);
long Set_size(Set *set);

#ifdef __cplusplus
}

#endif
#endif /* ----- #ifndef __SET_H__  ----- */
