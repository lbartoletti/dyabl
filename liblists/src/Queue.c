/**
 *
 *\file Queue.c
 *
 *\brief Queue using SList
 *
 *\version 0.1
 *\date  10.03.2016 09:33:41
 *\author Loïc BARTOLETTI, l.bartoletti@free.fr
 *
 */

#include "Queue.h"

Queue *Queue_new(void (*destroy)(void *)) {
  return SList_new_init(destroy, NULL);
}

void Queue_destroy(Queue *queue) { SList_free(queue); }

int Queue_push(Queue *queue, const void *data) {
  return SList_append(queue, data);
}

int Queue_pop(Queue *queue, void **data) {
  return SList_remove_head(queue, data);
}

void *Queue_peek(Queue *queue) { return SList_head(queue)->data; }

bool Queue_isEmpty(Queue *queue) { return SList_isEmpty(queue); }

long Queue_size(Queue *queue) { return SList_length(queue); }
