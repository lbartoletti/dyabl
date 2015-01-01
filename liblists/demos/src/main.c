#include "Utils.h"
#include <DList.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct testStruct_t {
  int a;
  int b;
} testStruct;

static int comparStruct(const void *a, const void *b) {
  const testStruct _a = *((const testStruct *)(a));
  const testStruct _b = *((const testStruct *)(b));

  if (_a.a < _b.a)
    return -1;
  else if (_a.a > _b.a)
    return 1;
  else {
    if (_a.b < _b.b)
      return -1;
    else if (_a.b > _b.b)
      return 1;
  }

  return 0;
}
static int my_strcmp(const void *a, const void *b) {
  return strcmp((const char *)a, (const char *)b);
}

static void my_puts(const void *a) { puts((const char *)a); }

static void print(const void *data) {
  const testStruct d = *((const testStruct *)(data));
  printf("%d %d->", d.a, d.b);
}

static void structList(void) {
  DList *list = DList_new();
  if (!list) {
    return;
  }

  testStruct t, q, r;
  t.a = 1;
  t.b = 2;

  q.a = 5;
  q.b = 8;

  r.a = 1;
  r.b = 2;

  DList_append(list, (const void *)&t);
  DList_prepend(list, (const void *)&q);
  DList_append(list, (const void *)&r);

  DList_foreach_safe(list, print);
  printf("\b\b  \n");
  DList_reverse_foreach_safe(list, print);
  printf("\b\b  \n");

  printf("%d\n", comparStruct(&t, &q));
  printf("%d\n", comparStruct(&t, &r));

  DList_free(list);
}

static void addInt(void **data) {
  int d = (int)(intptr_t)*data + 1;
  *data = (void *)(intptr_t)d;
}
static void printInt(const void *i) { printf("%d\n", (int)(intptr_t)i); }

static void intList(void) {
  DList *list = DList_new_init(NULL, comparInt);
  if (!list)
    return;

  int *pi = malloc(sizeof(int));
  if (!pi) {
    DList_free(list);
    return;
  }

  *pi = 99;
  DList_append(list, (const void *)(intptr_t)*pi);
  *pi = 10;
  DList_append(list, (const void *)(intptr_t)*pi);
  free(pi);
  DList_free(list);

  list = DList_new_init(NULL, comparIntptr);
  if (!list)
    return;

  int key = 50;
  DList_append(list, (const void *)(intptr_t)key);
  key = 25;
  DList_append(list, (const void *)(intptr_t)key);

  DList_foreach_safe(list, printInt);
  printf("\b\b  \n");

  printf("Index: %ld\n", DList_index(list, (const void *)(intptr_t)15));

  printf("Insert: %d\n", DList_unique_insert(list, (const void *)(intptr_t)20));
  printf("Insert: %d\n", DList_unique_insert(list, (const void *)(intptr_t)20));
  DList_foreach_safe(list, printInt);
  printf("\b\b  \n");

  DList_foreach(list, addInt);
  DList_foreach_safe(list, printInt);
  printf("\b\b  \n");

  DList_sort(list);
  DList_foreach_safe(list, printInt);
  printf("\b\b  \n");

  DList_reverse_foreach(list, addInt);
  DList_reverse_foreach_safe(list, printInt);
  printf("\b\b  \n");
  DList_free(list);
}

static void charList(void) {
  DList *list = DList_new_init(free, my_strcmp);
  if (!list)
    return;

  DList_append(list, strdup("second"));
  DList_prepend(list, strdup("first"));
  DList_append(list, strdup("third"));
  DList_insert_position(list, strdup("insert"), 1);
  DList_unique_insert(list, strdup("last"));
  char *p = strdup("last");
  DList_unique_insert(list, p);
  free(p);
  p = NULL;

  DList_foreach_safe(list, my_puts);
  printf("\b\b  \n");

  DList_remove_tail(list, (void **)&p);
  printf("%s\n", p);
  free(p);
  p = NULL;

  DList_remove_head(list, (void **)&p);
  printf("%s\n", p);
  free(p);
  p = NULL;

  DList_remove_next(list, (void **)&p);
  printf("%s\n", p);
  free(p);
  p = NULL;

  puts("List:");
  DList_foreach_safe(list, my_puts);
  printf("\b\b  \n");
  printf("Head now: %s\n", (char *)DList_head(list)->data);
  printf("Tail now: %s\n", (char *)DList_tail(list)->data);

  printf("Index: %ld\n", DList_index(list, "second"));
  DList_free(list);
}

int main(void) {
  structList();
  intList();
  charList();

  return EXIT_SUCCESS;
}
