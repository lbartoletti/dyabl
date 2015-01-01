#include "SList.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

SList *SList_new(void) {
  SList *p_list = NULL;
  p_list = malloc(sizeof(struct slist));

  if (!p_list) {
    return NULL;
  }

  p_list->head = NULL;
  p_list->elem = NULL;
  p_list->tail = NULL;
  p_list->destroy = NULL;
  p_list->compar = NULL;
  // p_list->is_circular = false;
  p_list->nb_elem = 0;

  return p_list;
}

SList *SList_new_init(void (*destroy)(void *),
                      int (*compar)(const void *, const void *)) {
  SList *p_list = NULL;
  p_list = SList_new();

  if (!p_list) {
    return NULL;
  }

  /* p_list not NULL */
  (void)SList_set_destroy(p_list, destroy);
  (void)SList_set_compar(p_list, compar);

  return p_list;
}

int SList_set_destroy(SList *list, void (*destroy)(void *)) {
  if (!list) {
    return SLIST_ERR_NOLIST;
  }

  list->destroy = destroy;

  return SLIST_SUCCESS;
}

int SList_set_compar(SList *list, int (*compar)(const void *, const void *)) {
  if (!list) {
    return SLIST_ERR_NOLIST;
  }

  list->compar = compar;

  return SLIST_SUCCESS;
}

int SList_append(SList *list, const void *data) {
  if (!list) {
    return SLIST_ERR_NOLIST;
  }

  SList_elem *new_elt = NULL;

  new_elt = malloc(sizeof(struct slist_elem));
  if (!new_elt) {
    return SLIST_ERR_MEMORY;
  }

  new_elt->data = DECONST(void *, data);
  new_elt->next = NULL;

  if (list->head == NULL) {
    list->head = new_elt;
    list->tail = new_elt;
    list->elem = new_elt;
  } else {
    list->tail->next = new_elt;
    list->tail = new_elt;
    list->elem = new_elt;
  }
  list->nb_elem++;

  return SLIST_SUCCESS;
}

int SList_prepend(SList *list, const void *data) {
  if (!list) {
    return SLIST_ERR_NOLIST;
  }

  if (list->head == NULL) {
    return SList_append(list, data);
  }

  SList_elem *new_elt = NULL;
  new_elt = malloc(sizeof(struct slist_elem));
  if (!new_elt) {
    return SLIST_ERR_MEMORY;
  }

  new_elt->data = DECONST(void *, data);
  new_elt->next = list->head;
  list->elem = new_elt;
  list->head = new_elt;
  list->nb_elem++;

  return SLIST_SUCCESS;
}

int SList_insert_position(SList *list, const void *data, const long position) {
  long pos;

  if (!list) {
    return SLIST_ERR_NOLIST;
  }

  if (position < 0) {
    if ((position + SList_length(list)) < 0) { /* out of range */
      return SLIST_ERR;
    }
    { pos = list->nb_elem + position; }
  } else if (position > SList_length(list)) {
    return SLIST_ERR;
  } else {
    pos = position;
  }

  SList_elem *new_elt = NULL;

  if (pos == 0) {
    return SList_prepend(list, data);
  }
  if (pos >= list->nb_elem) {
    return SList_append(list, data);
  }

  SList_elem *p_l = list->head;

  new_elt = malloc(sizeof *new_elt);
  if (!new_elt) {
    return SLIST_ERR_MEMORY;
  }

  long i;

  for (i = 0; i < pos - 1; i++) {
    p_l = p_l->next;
  }

  new_elt->data = DECONST(void *, data);

  SList_elem *sentinelle = p_l;

  new_elt->next = sentinelle->next;
  sentinelle->next = new_elt;

  list->elem = new_elt;
  list->nb_elem++;

  return SLIST_SUCCESS;
}

int SList_unique_insert(SList *list, const void *data) {
  if (!list) {
    return SLIST_ERR_NOLIST;
  }
  if (!list->compar) {
    return SLIST_ERR_NOCOMPAR;
  }

  if (SList_index(list, data) == -1) {
    return SList_append(list, data);
  }

  return 0;
}

int SList_remove_tail(SList *list, void **data) {
  if (!list) {
    return SLIST_ERR_NOLIST;
  }

  *data = SList_tail(list)->data;

  SList_elem *iterator = list->head;

  for (unsigned int i = 0; i < SList_length(list) - 2; i++) {
    iterator = iterator->next;
  }

  list->tail = iterator;
  iterator->next = NULL;
  free(iterator->next);

  list->nb_elem--;
  list->elem = SList_tail(list);

  return SLIST_SUCCESS;
}

int SList_remove_head(SList *list, void **data) {
  if (!list) {
    return SLIST_ERR_NOLIST;
  }
  if (!list->head) {
    return SLIST_ERR;
  }

  SList_elem *first = SList_head(list);

  *data = first->data;

  list->head = first->next;
  list->nb_elem--;

  free(first);

  list->elem = SList_head(list);
  if (list->nb_elem == 0) {
    list->tail = NULL;
  }

  return SLIST_SUCCESS;
}

int SList_remove_next(SList *list, void **data) {
  if (!list) {
    return SLIST_ERR_NOLIST;
  }
  if (!list->elem) {
    return SLIST_ERR;
  }

  SList_elem *p_next = list->elem->next;

  if (p_next == NULL) {
    return SList_remove_tail(list, data);
  }

  list->elem->next = p_next->next;
  *data = p_next->data;

  free(p_next);

  list->nb_elem--;
  return SLIST_SUCCESS;
}

int SList_remove(SList *list, const void *data) {
  if (!list) {
    return SLIST_ERR_NOLIST;
  }
  if (!list->compar) {
    return SLIST_ERR_NOCOMPAR;
  }

  int ret = SLIST_SUCCESS;

  bool found = false;

  list->elem = list->head;

  SList_elem *p_l = list->head;
  SList_elem *p_lb = NULL;

  while (p_l && ret && !found) {
    if (list->compar(p_l->data, data) == 0) {
      found = true;
      if (p_lb) {
        list->elem = p_lb;
        void *datadeleted;
        ret = SList_remove_next(list, &datadeleted);
        if (list->destroy != NULL) {
          list->destroy(datadeleted);
        }
      } else {
        void *datadeleted;
        ret = SList_remove_head(list, &datadeleted);
        if (list->destroy != NULL) {
          list->destroy(datadeleted);
        }
      }
    } else {
      p_lb = p_l;
      p_l = p_l->next;
    }
  }

  return ret;
}

int SList_remove_all(SList *list, const void *data) {
  if (!list) {
    return SLIST_ERR_NOLIST;
  }
  if (!list->compar) {
    return SLIST_ERR_NOCOMPAR;
  }

  int ret = SLIST_SUCCESS;

  list->elem = list->head;

  SList_elem *p_l = list->head;
  SList_elem *p_lb = NULL;

  while (p_l && ret) {
    if (list->compar(p_l->data, data) == 0) {
      if (p_lb) {
        list->elem = p_lb;
        void *datadeleted;
        ret = SList_remove_next(list, &datadeleted);
        if (list->destroy != NULL) {
          list->destroy(datadeleted);
        }
        p_lb = p_l;
        // p_l = p_l->next;
        p_l = NULL;
      } else {
        void *datadeleted;
        ret = SList_remove_head(list, &datadeleted);
        if (list->destroy != NULL) {
          list->destroy(datadeleted);
        }
        p_lb = NULL;
        p_l = SList_head(list);
      }
    } else {
      p_lb = p_l;
      p_l = p_l->next;
    }
  }

  return ret;
}

int SList_empty(SList *list) {
  if (!list) {
    return SLIST_ERR_NOLIST;
  }

  while (list->nb_elem != 0) {
    void *data = NULL;
    SList_remove_head(list, &data);
    if (list->destroy != NULL) {
      list->destroy(data);
    }
  }

  return SLIST_SUCCESS;
}

int SList_free(SList *list) {
  if (!list) {
    return SLIST_ERR_NOLIST;
  }

  SList_empty(list);
  free(list);

  return SLIST_SUCCESS;
}

SList *SList_reverse(SList *list) {
  if (!list) {
    return NULL;
  }

  SList_elem *p_l = list->head;
  SList *r_l = SList_new();

  if (r_l) {

    while (p_l) {
      SList_prepend(r_l, p_l->data);
      p_l = p_l->next;
    }
  }

  return r_l;
}

long SList_index(SList *list, const void *data) {
  if (!list) {
    return SLIST_ERR_NOLIST;
  }
  if (!list->compar) {
    return SLIST_ERR_NOCOMPAR;
  }

  long ret = SLIST_ERR;

  bool found = false;
  long i = 0;

  SList_elem *p_l = list->head;

  while (p_l && !found) {
    if (p_l->data == data) {
      if (list->compar((const void *)p_l->data, data) == 0) {
        ret = i;
        found = true;
      }
    }
    i++;
    p_l = p_l->next;
  }

  return ret;
}

int SList_foreach_safe(SList *list, void (*func)(const void *)) {
  if (!list) {
    return SLIST_ERR_NOLIST;
  }

  SList_elem *p_l = list->head;

  while (p_l) {
    func((const void *)p_l->data);
    p_l = p_l->next;
  }

  return SLIST_SUCCESS;
}

int SList_foreach(SList *list, void (*func)(void **)) {
  if (!list) {
    return SLIST_ERR_NOLIST;
  }

  SList_elem *p_l = list->head;

  while (p_l) {
    func(&p_l->data);
    p_l = p_l->next;
  }

  return SLIST_SUCCESS;
}

/* UTILITY FUNCTIONS */
/* Split the nodes of the given list into front and back halves,
     and return the two lists using the reference parameters.
     If the length is odd, the extra node should go in the front list.
     Uses the fast/slow pointer strategy.  */
static void FrontBackSplit(SList_elem *source, SList_elem **frontRef,
                           SList_elem **backRef) {
  SList_elem *fast;
  SList_elem *slow;

  if (source == NULL || source->next == NULL) {
    /* length < 2 cases */
    *frontRef = source;
    *backRef = NULL;
  } else {
    slow = source;
    fast = source->next;

    /* Advance 'fast' two nodes, and advance 'slow' one node */
    while (fast != NULL) {
      fast = fast->next;
      if (fast != NULL) {
        slow = slow->next;
        fast = fast->next;
      }
    }

    /*
     * 'slow' is before the midpoint in the list, so split it in
     * two at that point.
     */
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
  }
}

/* See http://geeksforgeeks.org/?p=3622 for details of this
   function */
static SList_elem *SortedMerge(SList_elem *a, SList_elem *b, compar cmp) {
  SList_elem *result = NULL;

  /* Base cases */
  if (a == NULL) {
    return (b);
  }
  if (b == NULL) {
    return (a);
  }

  /* Pick either a or b, and recur */
  if (cmp(a->data, b->data) <= 0) {
    result = a;
    result->next = SortedMerge(a->next, b, cmp);
  } else {
    result = b;
    result->next = SortedMerge(a, b->next, cmp);
  }
  return (result);
}

/* sorts the linked list by changing next pointers (not data) */
static void MergeSort(SList_elem **headRef, compar cmp) {
  SList_elem *head = *headRef;
  SList_elem *a;
  SList_elem *b;

  /* Base case -- length 0 or 1 */
  if ((head == NULL) || (head->next == NULL)) {
    return;
  }
  /* Split head into 'a' and 'b' sublists */
  FrontBackSplit(head, &a, &b);

  /* Recursively sort the sublists */
  MergeSort(&a, cmp);
  MergeSort(&b, cmp);

  /* answer = merge the two sorted lists together */
  *headRef = SortedMerge(a, b, cmp);
}

int SList_sort(SList *list) {
  if (!list) {
    return SLIST_ERR_NOLIST;
  }
  if (!list->compar) {
    return SLIST_ERR_NOCOMPAR;
  }

  MergeSort(&list->head, list->compar);

  return SLIST_SUCCESS;
}

SList_elem *SList_find(SList *list, const void *data) {
  if (!list) {
    return NULL;
  }
  if (!list->compar) {
    return NULL;
  }

  SList_elem *ret = NULL;

  bool found = false;

  SList_elem *p_l = list->head;

  while (p_l && !found) {
    if (p_l->data == data) {
      if (list->compar((const void *)p_l->data, data) == 0) {
        found = true;
        ret = p_l;
      }
    }
    p_l = p_l->next;
  }

  return ret;
}
