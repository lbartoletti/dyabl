#include "DList.h"
#include <stdbool.h>
#include <stdlib.h>

DList *DList_new(void) {
  DList *p_list = malloc(sizeof(struct dlist));

  if (!p_list) {
    return NULL;
  }

  p_list->head = NULL;
  p_list->elem = NULL;
  p_list->tail = NULL;
  p_list->destroy = NULL;
  p_list->compar = NULL;
  p_list->nb_elem = 0;

  return p_list;
}

DList *DList_new_init(void (*destroy)(void *),
                      int (*compar)(const void *, const void *)) {
  DList *p_list = DList_new();

  if (!p_list) {
    return NULL;
  }

  p_list->destroy = destroy;
  p_list->compar = compar;

  return p_list;
}

int DList_append(DList *list, const void *data) {
  if (!list) {
    return DLIST_ERNOLIST;
  }

  DList_elem *nouveau = NULL;

  nouveau = malloc(sizeof(struct dlist_elem));
  if (nouveau) {
    nouveau->prev = NULL;
    nouveau->data = DECONST(void *, data);
    nouveau->next = NULL;

    if (list->head == NULL) {
      list->head = nouveau;
      list->tail = nouveau;
      list->elem = nouveau;
    } else {
      nouveau->prev = list->tail;
      list->tail->next = nouveau;
      list->tail = nouveau;
      list->elem = nouveau;
    }
    list->nb_elem++;

  } else {
    return DLIST_ERMEMORY;
  }

  return DLIST_SUCCESS;
}

int DList_prepend(DList *list, const void *data) {
  if (!list) {
    return DLIST_ERNOLIST;
  }

  if (list->head != NULL) {
    DList_elem *nouveau = NULL;

    nouveau = malloc(sizeof(struct dlist_elem));
    if (nouveau) {
      nouveau->data = DECONST(void *, data);
      nouveau->prev = NULL;
      nouveau->next = list->head;
      list->head->prev = nouveau;
      list->elem = nouveau;
      list->head = nouveau;
      list->nb_elem++;
    } else {
      return DLIST_ERMEMORY;
    }
  } else {
    return DList_append(list, data);
  }

  return DLIST_SUCCESS;
}

int DList_insert_position(DList *list, const void *data, const long position) {
  if (!list) {
    return DLIST_ERNOLIST;
  }

  long pos;

  if (position < 0) {
    if ((position + DList_length(list)) < 0) { /* out of range */
      return DLIST_ERPOSITION;
    }
    { pos = list->nb_elem + position; }
  } else {
    if (position > DList_length(list)) {
      return DLIST_ERPOSITION;
    }
    { pos = position; }
  }

  DList_elem *nouveau = NULL;

  if (pos == 0) {
    DList_prepend(list, data);
  } else if (pos >= list->nb_elem) {
    DList_append(list, data);
  } else {

    nouveau = malloc(sizeof(struct dlist_elem));
    if (nouveau) {
      unsigned long i;
      DList_elem *p_l = NULL;

      if ((DList_length(list) - pos) <= (pos / 2)) {
        p_l = list->tail;
        for (i = 0; i < (unsigned long)(DList_length(list) - pos); i++) {
          p_l = p_l->prev;
        }
      } else {
        p_l = list->head;
        for (i = 0; i < (unsigned long)pos - 1; i++) {
          p_l = p_l->next;
        }
      }

      nouveau->data = DECONST(void *, data);
      DList_elem *sentinelle = p_l;

      nouveau->prev = sentinelle;
      nouveau->next = sentinelle->next;
      sentinelle->next = nouveau;
      nouveau->next->prev = nouveau;

      list->elem = nouveau;
      list->nb_elem++;

    } else {
      return DLIST_ERMEMORY;
    }
  }

  return DLIST_SUCCESS;
}

int DList_unique_insert(DList *list, const void *data) {
  if (!list) {
    return DLIST_ERNOLIST;
  }
  if (list->compar != NULL) {
    if (DList_index(list, data) == -1) {
      return DList_append(list, data);
    }
  } else {
    return DLIST_ERNOCMP;
  }

  return DLIST_SUCCESS;
}

int DList_remove_tail(DList *list, void **data) {
  if (!list) {
    return DLIST_ERNOLIST;
  }

  DList_elem *iterator = list->head;

  if (DList_tail(list) && iterator) {
    *data = DList_tail(list)->data;

    for (unsigned int i = 0; i < DList_length(list) - 2; i++) {
      iterator = iterator->next;
    }

    list->tail = iterator;
    iterator->next = NULL;
    free(iterator->next);
    list->nb_elem--;
    list->elem = DList_tail(list);

    return DLIST_SUCCESS;
  }
  return DLIST_ERROR;
}

int DList_remove_head(DList *list, void **data) {
  if (!list) {
    return DLIST_ERNOLIST;
  }

  DList_elem *first = DList_head(list);

  if (first) {
    *data = first->data;

    list->head = first->next;
    if (list->head) {
      list->head->prev = NULL;
    }
    list->nb_elem--;

    free(first);
    list->elem = DList_head(list);
    if (list->nb_elem == 0) {
      list->tail = NULL;
    }

    return DLIST_SUCCESS;
  }
  return DLIST_ERROR;
}

int DList_remove_next(DList *list, void **data) {
  if (!list) {
    return DLIST_ERNOLIST;
  }

  if (list->elem) {
    DList_elem *p_next = list->elem->next;

    if (p_next == NULL || DList_length(list) == 2) {
      return DList_remove_tail(list, data);
    }
    *data = p_next->data;

    list->elem->next = p_next->next;
    list->elem->next->prev = list->elem;

    free(p_next);

    list->nb_elem--;
    return DLIST_SUCCESS;
  }
  return DLIST_ERROR;
}

int DList_remove(DList *list, const void *data) {
  int ret = DLIST_ERROR;

  if (!list) {
    return DLIST_ERNOLIST;
  }
  if (!list->compar) {
    return DLIST_ERNOCMP;
  }

  bool found = false;

  list->elem = list->head;
  DList_elem *p_l = list->head;
  DList_elem *p_lb = NULL;

  while (p_l && !found) {
    if (list->compar(p_l->data, data) == 0) {
      found = true;
      if (p_lb) {
        list->elem = p_lb;
        void *datadeleted;
        ret = DList_remove_next(list, &datadeleted);
        if (list->destroy != NULL) {
          list->destroy(datadeleted);
        }
      } else {
        void *datadeleted;
        ret = DList_remove_head(list, &datadeleted);
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

int DList_remove_all(DList *list, const void *data) {
  if (!list) {
    return DLIST_ERNOLIST;
  }
  if (!list->compar) {
    return DLIST_ERNOCMP;
  }

  int ret = DLIST_SUCCESS;

  list->elem = list->head;
  DList_elem *p_l = list->head;
  DList_elem *p_lb = NULL;

  while (p_l) {
    if (list->compar(p_l->data, data) == 0) {
      if (p_lb) {
        list->elem = p_lb;
        void *datadeleted;
        ret = DList_remove_next(list, &datadeleted);
        if (list->destroy != NULL) {
          list->destroy(datadeleted);
        }
        p_lb = p_l;
        p_l = NULL;
        // p_l = p_l->next;
      } else {
        void *datadeleted;
        ret = DList_remove_head(list, &datadeleted);
        if (list->destroy != NULL) {
          list->destroy(datadeleted);
        }
        p_lb = NULL;
        p_l = DList_head(list);
      }
    } else {
      p_lb = p_l;
      p_l = p_l->next;
    }
  }

  return ret;
}

void DList_empty(DList *list) {
  if (!list) {
    return;
  }

  void *data = NULL;

  while (list->nb_elem != 0) {
    DList_remove_head(list, &data);
    if (list->destroy != NULL) {
      list->destroy(data);
    }
  }
}

void DList_free(DList *list) {
  if (!list) {
    return;
  }

  DList_empty(list);
  free(list);
}

DList *DList_reverse(DList *list) {
  if (!list) {
    return list;
  }

  DList_elem *p_l = list->head;
  DList *r_l = DList_new();

  if (p_l && r_l) {
    while (p_l) {
      DList_prepend(r_l, p_l->data);
      p_l = p_l->next;
    }
  } else {
    if (r_l) {
      free(r_l);
    }
    return NULL;
  }

  return r_l;
}

long DList_index(DList *list, const void *data) {
  long ret = -1;

  if (!list) {
    return DLIST_ERNOLIST;
  }
  if (!list->compar) {
    return DLIST_ERNOCMP;
  }

  bool found = false;
  int i = 0;
  DList_elem *p_l = list->head;

  while (p_l && !found) {
    if (list->compar((const void *)p_l->data, data) == 0) {
      ret = i;
      found = true;
    }
    i++;
    p_l = p_l->next;
  }

  return ret;
}

void DList_foreach_safe(DList *list, void (*func)(const void *)) {
  if (list) {
    DList_elem *p_l = list->head;

    while (p_l) {
      func((const void *)p_l->data);
      p_l = p_l->next;
    }
  }
}

void DList_foreach(DList *list, void (*func)(void **)) {
  if (list) {
    DList_elem *p_l = list->head;

    while (p_l) {
      func(&p_l->data);
      p_l = p_l->next;
    }
  }
}

void DList_reverse_foreach(DList *list, void (*func)(void **)) {
  if (list) {
    DList_elem *p_l = list->tail;

    while (p_l) {
      func(&p_l->data);
      p_l = p_l->prev;
    }
  }
}

void DList_reverse_foreach_safe(DList *list, void (*func)(const void *)) {
  if (list) {
    DList_elem *p_l = list->tail;

    while (p_l) {
      func((const void *)p_l->data);
      p_l = p_l->prev;
    }
  }
}

static void FrontBackSplit(DList_elem *head, DList_elem **aRef,
                           DList_elem **bRef) {
  DList_elem *slow_ptr = head;
  DList_elem *prev_slow_ptr = NULL;
  DList_elem *fast_ptr = head;

  if (head != NULL) {
    while (fast_ptr != NULL && fast_ptr->next != NULL) {
      prev_slow_ptr = slow_ptr;
      fast_ptr = fast_ptr->next->next;
      slow_ptr = slow_ptr->next;
    }
    // printf("The middle element is [%d]\n\n", slow_ptr->data);
  }
  *aRef = head;
  *bRef = slow_ptr;
  if (prev_slow_ptr) {
    prev_slow_ptr->next = NULL;
  }
}

// Function to merge two linked lists
static DList_elem *SortedMerge(DList_elem *a, DList_elem *b, compar cmp) {
  DList_elem *result = NULL;

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
    result->next->prev = result;
    result->prev = NULL;
  } else {
    result = b;
    result->next = SortedMerge(a, b->next, cmp);
    result->next->prev = result;
    result->prev = NULL;
  }
  return (result);
}

/* Function to do merge sort */
static void MergeSort(DList_elem **headRef, compar cmp) {
  DList_elem *head = *headRef;
  DList_elem *a;
  DList_elem *b;

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
void DList_sort(DList *list) {
  if (list->compar) {
    MergeSort(&list->head, list->compar);
  }

  /* readd tail */
  DList_elem *p_l = list->head;

  while (p_l) {
    list->tail = p_l;
    p_l = p_l->next;
  }
}
