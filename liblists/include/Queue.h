/**
 *
 * \file Queue.h
 *
 * \brief Queue using SList
 *
 * \version 0.1
 * \date  10.03.2016 09:33:52
 * \author Loïc BARTOLETTI, l.bartoletti@free.fr
 * @copyright
 *
 */

#ifndef DYABL_QUEUE_H__
#define DYABL_QUEUE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "SList.h"

/** @brief Define Queue as a Single Linked List */
typedef SList Queue;
Queue *Queue_new(void (*destroy)(void *data));
void Queue_destroy(Queue *queue);
int Queue_push(Queue *queue, const void *data);
int Queue_pop(Queue *queue, void **data);
void *Queue_peek(Queue *queue);
bool Queue_isEmpty(Queue *queue);
long Queue_size(Queue *queue);
#ifdef __cplusplus
}

#endif
#endif /* ----- #ifndef __QUEUE_H__  ----- */
