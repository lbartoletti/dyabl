/**
 *
 * \file Set.c
 *
 * \brief Set using SList
 *
 * \version 0.1
 * \date  02.04.2016 15:00:35
 * \author Loïc BARTOLETTI, l.bartoletti@free.fr
 *
 */

#include "Set.h"

Set *Set_new(void (*destroy)(void *),
             int (*compar)(const void *, const void *)) {
  if (compar) {
    return SList_new_init(destroy, compar);
  }
  return NULL;
}

void Set_destroy(Set *set) { SList_free(set); }

int Set_insert(Set *set, const void *data) {
  return SList_unique_insert(set, data);
}

int Set_remove(Set *set, const void *data) { return SList_remove(set, data); }

Set *Set_union(Set *set1, Set *set2) {
  if (!set1 || !set2 || !set1->compar || !set2->compar) {
    return NULL;
  }

  Set *set_union = Set_new(set1->destroy, set1->compar);

  SList_elem *member = NULL;

  member = SList_head(set1);
  while (member) {
    if (Set_insert(set_union, (const void *)member->data) == -1) {
      SList_free(set_union);
      return NULL;
    }
    member = member->next;
  }
  member = SList_head(set2);
  while (member) {
    if (Set_insert(set_union, (const void *)member->data) == -1) {
      SList_free(set_union);
      return NULL;
    }
    member = member->next;
  }
  return set_union;
}

Set *Set_intersection(Set *set1, Set *set2) {
  if (!set1 || !set2 || !set1->compar || !set2->compar) {
    return NULL;
  }
  Set *set_inter = Set_new(set1->destroy, set1->compar);

  SList_elem *member = NULL;

  member = SList_head(set1);
  while (member) {
    if (Set_isMember(set2, (const void *)member->data)) {
      if (Set_insert(set_inter, (const void *)member->data) == -1) {
        SList_free(set_inter);
        return NULL;
      }
    }
    member = member->next;
  }
  return set_inter;
}

Set *Set_difference(Set *set1, Set *set2) {
  if (!set1 || !set2 || !set1->compar || !set2->compar) {
    return NULL;
  }
  Set *set_diff = Set_new(set1->destroy, set1->compar);

  SList_elem *member = NULL;

  member = SList_head(set1);
  while (member) {
    if (!Set_isMember(set2, (const void *)member->data)) {
      if (Set_insert(set_diff, (const void *)member->data) == -1) {
        SList_free(set_diff);
        return NULL;
      }
    }
    member = member->next;
  }
  return set_diff;
}
Set *Set_symetric_difference(Set *set1, Set *set2) {
  return Set_union(Set_difference(set1, set2), Set_difference(set2, set1));
}

bool Set_isMember(Set *set, const void *data) {
  if (SList_index(set, data) >= 0) {
    return true;
  }
  return false;
}

bool Set_isSubset(Set *set1, Set *set2) {
  if (Set_size(set1) > Set_size(set2)) {
    return false;
  }
  SList_elem *member = NULL;

  member = SList_head(set1);
  while (member) {
    if (!Set_isMember(set2, member->data)) {
      return false;
    }
    member = member->next;
  }
  return true;
}

bool Set_isSuperset(Set *set1, Set *set2) {
  if (Set_size(set1) < Set_size(set2)) {
    return false;
  }

  SList_elem *member = NULL;

  member = SList_head(set2);
  while (member) {
    if (!Set_isMember(set1, member->data)) {
      return false;
    }
    member = member->next;
  }
  return true;
}

bool Set_isDisjoint(Set *set1, Set *set2) {
  Set *set_inter = Set_intersection(set1, set2);
  const bool isDisjoint = Set_isEmpty(set_inter);
  Set_destroy(set_inter);

  return isDisjoint;
}

bool Set_isEqual(Set *set1, Set *set2) {
  if (Set_size(set1) != Set_size(set2)) {
    return false;
  }
  return Set_isSubset(set1, set2);
}

bool Set_isEmpty(Set *set) { return SList_isEmpty(set); }

long Set_size(Set *set) { return SList_length(set); }
