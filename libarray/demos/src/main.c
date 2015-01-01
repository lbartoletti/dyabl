#include <Array.h>
#include <DArray.h>
#include <Rand.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void printTab(int *t, size_t nb) {
  for (size_t i = 0; i < nb; i++)
    printf("%d ", t[i]);
  puts("");
}

static void shuffle(void) {
  int q[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  printTab(q, 10);
  bool r = Shuffle(q, 10, sizeof(int));
  if (r == true)
    printTab(q, 10);
}

static void Inter_search(void) {
  int q[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  printTab(q, 10);
  int *r = InterpolationSearch_rec(9, q, 0, 9, comparInt);
  if (r)
    printf("%d %d %p %p\n", *r, q[8], (void *)r, (void *)&q[8]);
}

static void Bsearch(void) {
  int q[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  printTab(q, 10);
  size_t element_size = sizeof(q[0]);
  size_t array_size = sizeof(q) / element_size;
  int p = 9;
  void *element;
  element = BinarySearch(&p, q, array_size, element_size, comparInt);
  if (element)
    printf("Element found: %d\n", *(int *)element);
}
static void Bsearch_rec(void) {
  int q[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  printTab(q, 10);
  size_t element_size = sizeof(q[0]);
  size_t array_size = sizeof(q) / element_size;
  int p = 9;
  void *element;
  element = BinarySearch_rec(&p, q, 0, array_size, element_size, comparInt);
  if (element)
    printf("Element found: %d\n", *(int *)element);
}

static void Lsearch(void) {
  const int q[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  size_t element_size = sizeof(q[0]);
  size_t array_size = sizeof(q) / element_size;
  int p = 9;
  void *element;
  element = LinearFind(&p, q, &array_size, element_size, comparInt);
  if (element)
    printf("Element found: %d\n", *(int *)element);
}

static void Darray(void) {
  size_t n = 5, step = 2;
  int q[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  printTab(q, 10);
  DArray *ar = DArray_sized_step_new(sizeof(int), n, step);
  DArray_append(ar, (const void *)(intptr_t)q[0]);
  DArray_prepend(ar, (const void *)(intptr_t)q[1]);
  DArray_prepend(ar, (const void *)(intptr_t)q[2]);
  DArray_prepend(ar, (const void *)(intptr_t)q[3]);
  DArray_prepend(ar, (const void *)(intptr_t)q[4]);
  DArray_prepend(ar, (const void *)(intptr_t)q[5]);
  DArray_prepend(ar, (const void *)(intptr_t)q[6]);

  for (size_t i = 0; i <= 6; i++)
    printf("%d\n", (int)(intptr_t *)DArray_get_position(ar, i));

  void *data;
  DArray_remove_head(ar, &data);
  printf("head : %d\n", (int)(intptr_t)data);

  DArray_remove_tail(ar, &data);
  printf("tail : %d\n", (int)(intptr_t)data);
}

int main(void) {
  puts("Interpolation search");
  Inter_search();

  puts("Linear search");
  Lsearch();

  puts("Binary search rec");
  Bsearch_rec();

  puts("Binary search");
  Bsearch();

  puts("Shuffle");
  lcg_seed((unsigned long long)time(NULL));
  shuffle();

  puts("DArray");
  Darray();

  return EXIT_SUCCESS;
}
