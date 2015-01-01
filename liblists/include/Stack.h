/**
 *
 * \file Stack.h
 *
 * \brief Stack using SList
 *
 * \version 0.1
 * \date  10.03.2016 10:07:55
 * \author Loïc BARTOLETTI, l.bartoletti@free.fr
 *
 */

#ifndef DYABL_STACK_H__
#define DYABL_STACK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "SList.h"

typedef SList Stack;
Stack *Stack_new(void (*destroy)(void *data));
void Stack_destroy(Stack *stack);
int Stack_push(Stack *stack, const void *data);
int Stack_pop(Stack *stack, void **data);
void *Stack_peek(Stack *stack);
bool Stack_isEmpty(Stack *stack);
long Stack_size(Stack *stack);

#ifdef __cplusplus
}

#endif
#endif /* ----- #ifndef __STACK_H__  ----- */
