extern "C" {
#include "Sort.h"
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>

static int comparInt(const void *a, const void *b) {
  int left = *(const int *)a;
  int right = *(const int *)b;

  return ((left > right) - (left < right));
}

void printTab(int *t, size_t nbElem) {
  for (size_t i = 0; i < nbElem; i++)
    printf("%d ", t[i]);
  puts("");
}
}
#include <gtest/gtest.h>

TEST(Sort, bubbleSort) {

  int q[] = {98, 6, 78, 12, 12, 78, 312, 65};
  int qs[] = {98, 6, 78, 12, 12, 78, 312, 65};
  bubbleSort(q, sizeof(q) / sizeof(*q), sizeof(*q), comparInt);
  qsort(qs, sizeof(qs) / sizeof(*qs), sizeof(*qs), comparInt);
  size_t n = sizeof(q) / sizeof(*q);
  ASSERT_TRUE(memcmp(qs, q, n) == 0);
}

TEST(Sort, insertionSort) {
  int q[] = {98, 6, 78, 12, 12, 78, 312, 65};
  int qs[] = {98, 6, 78, 12, 12, 78, 312, 65};
  insertionSort(q, sizeof(q) / sizeof(*q), sizeof(*q), comparInt);
  qsort(qs, sizeof(qs) / sizeof(*qs), sizeof(*qs), comparInt);
  size_t n = sizeof(q) / sizeof(*q);
  ASSERT_TRUE(memcmp(qs, q, n) == 0);
}

TEST(Sort, quickSort) {
  int q[] = {98, 6, 78, 12, 12, 78, 312, 65};
  int qs[] = {98, 6, 78, 12, 12, 78, 312, 65};
  quickSort(q, sizeof(q) / sizeof(*q), sizeof(*q), comparInt);
  qsort(qs, sizeof(qs) / sizeof(*qs), sizeof(*qs), comparInt);
  size_t n = sizeof(q) / sizeof(*q);
  ASSERT_TRUE(memcmp(qs, q, n) == 0);
}

TEST(Sort, selectionSort) {
  int q[] = {98, 6, 78, 12, 12, 78, 312, 65};
  int qs[] = {98, 6, 78, 12, 12, 78, 312, 65};
  selectionSort(q, sizeof(q) / sizeof(*q), sizeof(*q), comparInt);
  qsort(qs, sizeof(qs) / sizeof(*qs), sizeof(*qs), comparInt);
  size_t n = sizeof(q) / sizeof(*q);
  ASSERT_TRUE(memcmp(qs, q, n) == 0);
}

TEST(Sort, stoogeSort) {
  int q[] = {98, 6, 78, 12, 12, 78, 312, 65};
  int qs[] = {98, 6, 78, 12, 12, 78, 312, 65};
  stoogeSort(q, sizeof(q) / sizeof(*q), sizeof(*q), comparInt);
  qsort(qs, sizeof(qs) / sizeof(*qs), sizeof(*qs), comparInt);
  size_t n = sizeof(q) / sizeof(*q);
  ASSERT_TRUE(memcmp(qs, q, n) == 0);
}

TEST(Sort, gnomeSort) {
  int q[] = {98, 6, 78, 12, 12, 78, 312, 65};
  int qs[] = {98, 6, 78, 12, 12, 78, 312, 65};
  gnomeSort(q, sizeof(q) / sizeof(*q), sizeof(*q), comparInt);
  qsort(qs, sizeof(qs) / sizeof(*qs), sizeof(*qs), comparInt);
  size_t n = sizeof(q) / sizeof(*q);
  ASSERT_TRUE(memcmp(qs, q, n) == 0);
}

TEST(Sort, mergeSort) {
  int q[] = {98, 6, 78, 12, 12, 78, 312, 65};
  int qs[] = {98, 6, 78, 12, 12, 78, 312, 65};
  mergeSort(q, sizeof(q) / sizeof(*q), sizeof(*q), comparInt);
  qsort(qs, sizeof(qs) / sizeof(*qs), sizeof(*qs), comparInt);
  size_t n = sizeof(q) / sizeof(*q);
  ASSERT_TRUE(memcmp(qs, q, n) == 0);

  /*struct rlimit limit, nlimit;
  getrlimit (RLIMIT_AS, &limit);
  int ret = mergeSort (q, sizeof (q) / sizeof (*q), limit.rlim_cur, comparInt);
  ASSERT_EQ (ret, 0);*/
}

TEST(Sort, shellSortSh) {
  int q[] = {98, 6, 78, 12, 12, 78, 312, 65};
  int qs[] = {98, 6, 78, 12, 12, 78, 312, 65};
  shellSortSh(q, sizeof(q) / sizeof(*q), sizeof(*q), comparInt);
  qsort(qs, sizeof(qs) / sizeof(*qs), sizeof(*qs), comparInt);
  size_t n = sizeof(q) / sizeof(*q);
  ASSERT_TRUE(memcmp(qs, q, n) == 0);

  /*struct rlimit limit;
  getrlimit (RLIMIT_AS, &limit);
  int ret = shellSortSh (q, sizeof (q) / sizeof (*q), limit.rlim_cur,
  comparInt); ASSERT_EQ (ret, -1);*/
}

TEST(Sort, shellSortKn) {
  int q[] = {98, 6, 78, 12, 12, 78, 312, 65};
  int qs[] = {98, 6, 78, 12, 12, 78, 312, 65};
  shellSortKn(q, sizeof(q) / sizeof(*q), sizeof(*q), comparInt);
  qsort(qs, sizeof(qs) / sizeof(*qs), sizeof(*qs), comparInt);
  size_t n = sizeof(q) / sizeof(*q);
  ASSERT_TRUE(memcmp(qs, q, n) == 0);

  /*struct rlimit limit;
  getrlimit (RLIMIT_AS, &limit);
  int ret = shellSortKn (q, sizeof (q) / sizeof (*q), limit.rlim_cur,
  comparInt); ASSERT_EQ (ret, -1);*/
}

TEST(Sort, shellSortGo) {
  int q[] = {98, 6, 78, 12, 12, 78, 312, 65};
  int qs[] = {98, 6, 78, 12, 12, 78, 312, 65};
  shellSortGo(q, sizeof(q) / sizeof(*q), sizeof(*q), comparInt);
  qsort(qs, sizeof(qs) / sizeof(*qs), sizeof(*qs), comparInt);
  size_t n = sizeof(q) / sizeof(*q);
  ASSERT_TRUE(memcmp(qs, q, n) == 0);

  /*struct rlimit limit;
  getrlimit (RLIMIT_AS, &limit);
  int ret = shellSortGo (q, sizeof (q) / sizeof (*q), limit.rlim_cur,
  comparInt); ASSERT_EQ (ret, -1);*/
}

TEST(Sort, shellSortSe) {
  int q[] = {98, 6, 78, 12, 12, 78, 312, 65};
  int qs[] = {98, 6, 78, 12, 12, 78, 312, 65};
  shellSortSe(q, sizeof(q) / sizeof(*q), sizeof(*q), comparInt);
  qsort(qs, sizeof(qs) / sizeof(*qs), sizeof(*qs), comparInt);
  size_t n = sizeof q / sizeof *q;
  ASSERT_TRUE(memcmp(qs, q, n) == 0);

  /*struct rlimit limit;
  getrlimit (RLIMIT_AS, &limit);
  int ret = shellSortSe (q, sizeof (q) / sizeof (*q), limit.rlim_cur,
  comparInt); ASSERT_EQ (ret, -1);*/
}

TEST(Sort, shellSortPr) {
  int q[] = {98, 6, 78, 12, 12, 78, 312, 65};
  int qs[] = {98, 6, 78, 12, 12, 78, 312, 65};
  shellSortPr(q, sizeof(q) / sizeof(*q), sizeof(*q), comparInt);
  qsort(qs, sizeof(qs) / sizeof(*qs), sizeof(*qs), comparInt);
  size_t n = sizeof q / sizeof *q;
  ASSERT_TRUE(memcmp(qs, q, n) == 0);

  /*struct rlimit limit;
  getrlimit (RLIMIT_AS, &limit);
  int ret = shellSortPr (q, sizeof (q) / sizeof (*q), limit.rlim_cur,
  comparInt); ASSERT_EQ (ret, -1);*/
}

TEST(Sort, shellSortIS) {
  int q[] = {98, 6, 78, 12, 12, 78, 312, 65};
  int qs[] = {98, 6, 78, 12, 12, 78, 312, 65};
  shellSortIS(q, sizeof(q) / sizeof(*q), sizeof(*q), comparInt);
  qsort(qs, sizeof(qs) / sizeof(*qs), sizeof(*qs), comparInt);
  size_t n = sizeof q / sizeof *q;
  ASSERT_TRUE(memcmp(qs, q, n) == 0);

  /*struct rlimit limit;
  getrlimit (RLIMIT_AS, &limit);
  int ret = shellSortIS (q, sizeof (q) / sizeof (*q), limit.rlim_cur,
  comparInt); ASSERT_EQ (ret, -1);*/
}

TEST(Sort, integerSort) {
  int q[] = {98, 6, 78, 12, 12, 78, 312, 65};
  int qs[] = {98, 6, 78, 12, 12, 78, 312, 65};
  integerSort(q, sizeof(q) / sizeof(*q), comparInt);
  qsort(qs, sizeof(qs) / sizeof(*qs), sizeof(*qs), comparInt);
  size_t n = sizeof q / sizeof *q;
  ASSERT_TRUE(memcmp(qs, q, n) == 0);

  int r[] = {98};
  ASSERT_EQ(integerSort(r, sizeof(r) / sizeof(*r), comparInt), 0);
}

TEST(Sort, bogoSort) {
  int q[] = {98, 6, 78, 12};
  int qs[] = {98, 6, 78, 12};
  bogoSort(q, sizeof(q) / sizeof(*q), sizeof(*q), comparInt);
  qsort(qs, sizeof(qs) / sizeof(*qs), sizeof(*qs), comparInt);
  size_t n = sizeof q / sizeof *q;
  ASSERT_TRUE(memcmp(qs, q, n) == 0);
}

int main(int argc, char *argv[]) {

  srand((unsigned)time(NULL));
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
