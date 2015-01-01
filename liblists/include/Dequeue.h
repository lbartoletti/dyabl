/**
 *
 * \file Dequeue.h
 *
 * \brief Dequeue using DList
 *
 * \version 0.1
 * \date  10.03.2016 09:33:52
 * \author Loïc BARTOLETTI, l.bartoletti@free.fr
 * @copyright
 *
 */

#ifndef DYABL_DEQUEUE_H__
#define DYABL_DEQUEUE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "DList.h"

/** @brief Define Dequeue as a Single Linked List */
typedef DList Dequeue;
Dequeue *Dequeue_new(void (*destroy)(void *data));
void Dequeue_destroy(Dequeue *queue);
int Dequeue_push_back(Dequeue *queue, const void *data);
int Dequeue_push_front(Dequeue *queue, const void *data);
int Dequeue_pop_back(Dequeue *queue, void **data);
int Dequeue_pop_front(Dequeue *queue, void **data);
void *Dequeue_peek_back(Dequeue *queue);
void *Dequeue_peek_front(Dequeue *queue);
bool Dequeue_isEmpty(Dequeue *queue);
long Dequeue_size(Dequeue *queue);
#ifdef __cplusplus
}
#endif
#endif /* ----- #ifndef __DEQUEUE_H__  ----- */
