/**
 *
 * \file test_stack-queue.cpp
 *
 * \brief gv
 *
 * \version 0.1
 * \date  10.03.2016 14:40:20
 * \author Loïc BARTOLETTI, l.bartoletti@free.fr
 *
 */

extern "C" {
#include "Dequeue.h"
#include "Queue.h"
#include "Stack.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
}

#include <gtest/gtest.h>

TEST(Stack /*unused*/, stackTests /*unused*/) {
  Stack *s = Stack_new(NULL);
  ASSERT_NE(s, nullptr);
  ASSERT_TRUE(Stack_isEmpty(s));

  Stack_push(s, (const void *)(intptr_t)15);
  ASSERT_EQ(1, Stack_size(s));
  ASSERT_FALSE(Stack_isEmpty(s));
  Stack_push(s, (const void *)(intptr_t)16);
  ASSERT_EQ(2, Stack_size(s));
  Stack_push(s, (const void *)(intptr_t)20);
  ASSERT_EQ(3, Stack_size(s));
  Stack_push(s, (const void *)(intptr_t)21);
  ASSERT_EQ(4, Stack_size(s));
  ASSERT_EQ(21, (intptr_t)Stack_peek(s));

  int p;
  Stack_pop(s, (void **)&p);
  ASSERT_EQ(3, Stack_size(s));
  ASSERT_EQ(20, (intptr_t)Stack_peek(s));

  Stack_destroy(s);
}

TEST(Queue /*unused*/, queueTests /*unused*/) {
  Queue *q = Queue_new(NULL);
  ASSERT_NE(q, nullptr);
  ASSERT_TRUE(Queue_isEmpty(q));
  Queue_push(q, (const void *)(intptr_t)15);
  ASSERT_EQ(1, Queue_size(q));
  ASSERT_FALSE(Queue_isEmpty(q));
  Queue_push(q, (const void *)(intptr_t)16);
  ASSERT_EQ(2, Queue_size(q));
  Queue_push(q, (const void *)(intptr_t)20);
  ASSERT_EQ(3, Queue_size(q));
  Queue_push(q, (const void *)(intptr_t)21);
  ASSERT_EQ(4, Queue_size(q));
  ASSERT_EQ(15, (intptr_t)Queue_peek(q));

  int p;
  Queue_pop(q, (void **)&p);
  ASSERT_EQ(3, Queue_size(q));
  ASSERT_EQ(16, (intptr_t)Queue_peek(q));

  Queue_destroy(q);
}

TEST(Dequeue /*unused*/, dequeueTests /*unused*/) {
  Dequeue *q = Dequeue_new(NULL);
  ASSERT_NE(q, nullptr);
  ASSERT_TRUE(Dequeue_isEmpty(q));
  Dequeue_push_back(q, (const void *)(intptr_t)15);
  ASSERT_EQ(1, Dequeue_size(q));
  ASSERT_FALSE(Dequeue_isEmpty(q));
  Dequeue_push_front(q, (const void *)(intptr_t)16);
  ASSERT_EQ(2, Dequeue_size(q));
  Dequeue_push_front(q, (const void *)(intptr_t)20);
  ASSERT_EQ(3, Dequeue_size(q));
  Dequeue_push_back(q, (const void *)(intptr_t)21);
  ASSERT_EQ(4, Dequeue_size(q));
  ASSERT_EQ(20, (intptr_t)Dequeue_peek_front(q));
  ASSERT_EQ(21, (intptr_t)Dequeue_peek_back(q));

  int p;
  Dequeue_pop_back(q, (void **)&p);
  ASSERT_EQ(21, p);
  ASSERT_EQ(3, Dequeue_size(q));
  Dequeue_pop_back(q, (void **)&p);
  ASSERT_EQ(15, p);
  ASSERT_EQ(2, Dequeue_size(q));

  Dequeue_pop_front(q, (void **)&p);
  ASSERT_EQ(20, p);
  ASSERT_EQ(1, Dequeue_size(q));

  Dequeue_destroy(q);
}
