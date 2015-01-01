/**
 *
 *\file Dequeue.c
 *
 *\brief Dequeue using DList
 *
 *\version 0.1
 *\date  10.03.2016 09:33:41
 *\author Loïc BARTOLETTI, l.bartoletti@free.fr
 *
 */

#include "Dequeue.h"

Dequeue *Dequeue_new(void (*destroy)(void *)) {
  return DList_new_init(destroy, NULL);
}

void Dequeue_destroy(Dequeue *queue) { DList_free(queue); }

int Dequeue_push_back(Dequeue *queue, const void *data) {
  return DList_append(queue, data);
}

int Dequeue_push_front(Dequeue *queue, const void *data) {
  return DList_prepend(queue, data);
}

int Dequeue_pop_back(Dequeue *queue, void **data) {
  return DList_remove_tail(queue, data);
}

int Dequeue_pop_front(Dequeue *queue, void **data) {
  return DList_remove_head(queue, data);
}

void *Dequeue_peek_back(Dequeue *queue) { return DList_tail(queue)->data; }

void *Dequeue_peek_front(Dequeue *queue) { return DList_head(queue)->data; }

bool Dequeue_isEmpty(Dequeue *queue) { return DList_isEmpty(queue); }

long Dequeue_size(Dequeue *queue) { return DList_length(queue); }
