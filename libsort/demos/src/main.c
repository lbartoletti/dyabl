#include "Sort.h"
#include <stdio.h>
#include <stdlib.h>
int comparInt(const void *a, const void *b);
void printTab(int *t, size_t nbElem);

int comparInt(const void *a, const void *b) {
  int left = *(const int *)a;
  int right = *(const int *)b;

  return ((left > right) - (left < right));
}

void printTab(int *t, size_t nbElem) {
  for (size_t i = 0; i < nbElem; i++)
    printf("%d ", t[i]);
  puts("");
}

int main(void) {
  puts("Bubble Sort:");
  int ret;
  int q[] = {98, 6, 78, 12, 12, 78, 312, 65};
  printTab(q, sizeof(q) / sizeof(*q));
  ret = bubbleSort(q, sizeof(q) / sizeof(*q), sizeof(*q), comparInt);
  printf("%d : ", ret);
  printTab(q, sizeof(q) / sizeof(*q));

  puts("Insertion Sort:");
  int q2[] = {98, 6, 78, 12, 12, 78, 312, 65};
  printTab(q2, sizeof(q2) / sizeof(*q2));
  ret = insertionSort(q2, sizeof(q2) / sizeof(*q2), sizeof(*q2), comparInt);
  printf("%d : ", ret);
  printTab(q2, sizeof(q2) / sizeof(*q2));

  puts("Quick Sort:");
  int q3[] = {98, 6, 78, 12, 12, 78, 312, 65};
  printTab(q3, sizeof(q3) / sizeof(*q3));
  ret = quickSort(q3, sizeof(q3) / sizeof(*q3), sizeof(*q3), comparInt);
  printf("%d : ", ret);
  printTab(q3, sizeof(q3) / sizeof(*q3));

  puts("Selection Sort:");
  int q4[] = {98, 6, 78, 12, 12, 78, 312, 65};
  printTab(q4, sizeof(q4) / sizeof(*q4));
  ret = selectionSort(q4, sizeof(q4) / sizeof(*q4), sizeof(*q4), comparInt);
  printf("%d : ", ret);
  printTab(q4, sizeof(q4) / sizeof(*q4));

  puts("Integer Sort:");
  int q5[] = {98, 6, 78, 12, 12, 78, 312, 65};
  printTab(q5, sizeof(q5) / sizeof(*q5));
  ret = integerSort(q5, sizeof(q5) / sizeof(*q5), comparInt);
  printf("%d : ", ret);
  printTab(q5, sizeof(q5) / sizeof(*q5));

  return EXIT_SUCCESS;
}
