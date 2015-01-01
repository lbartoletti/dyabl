/**
 *
 * \file Stack.c
 *
 * \brief Stack using SList
 *
 * \version 0.1
 * \date  10.03.2016 10:09:02
 * \author Loïc BARTOLETTI, l.bartoletti@free.fr
 *
 */

#include "Stack.h"

Stack *Stack_new(void (*destroy)(void *)) {
  return SList_new_init(destroy, NULL);
}

void Stack_destroy(Stack *stack) { SList_free(stack); }

int Stack_push(Stack *stack, const void *data) {
  return SList_append(stack, data);
}

int Stack_pop(Stack *stack, void **data) {
  return SList_remove_tail(stack, data);
}

void *Stack_peek(Stack *stack) { return SList_tail(stack)->data; }

bool Stack_isEmpty(Stack *stack) { return SList_isEmpty(stack); }

long Stack_size(Stack *stack) { return SList_length(stack); }
