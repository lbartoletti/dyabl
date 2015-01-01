extern "C" {
#include "Array.h"
#include "DArray.h"
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>

static int myCompare(const void *p1, const void *p2) {
  return strcmp((const char *)p1, (const char *)p2);
}
static int comparInt_inv(const void *a, const void *b) {
  int left = *(const int *)b;
  int right = *(const int *)a;
  return ((left > right) - (left < right));
}

void printTab(int *t, size_t nbElem) {
  for (size_t i = 0; i < nbElem; i++)
    printf("%d ", t[i]);

  puts("");
}

//    TODO: move
/*
static int
comparInt_noPt_inv (const void *a, const void *b)
{
int _a, _b;
_a = (intptr_t)b;
_b = (intptr_t)a;

if (_a < _b)
    return -1;

else if (_a > _b)
    return 1;

return 0;
}

static int
comparInt_noPt (const void *a, const void *b)
{
int _a, _b;
_a = (intptr_t)a;
_b = (intptr_t)b;

if (_a < _b)
    return -1;

else if (_a > _b)
    return 1;

return 0;
}*/
}

#include <gtest/gtest.h>

TEST(Array, isSorted) {
  int q[] = {98, 78, 12, 12, 10, 5};
  int r[] = {5, 10, 12, 12, 78, 98};
  ASSERT_TRUE(
      isSorted(q, sizeof(q) / sizeof(q[0]), sizeof(q[0]), comparInt_inv));
  ASSERT_FALSE(isSorted(q, sizeof(q) / sizeof(q[0]), sizeof(q[0]), comparInt));
  ASSERT_TRUE(isSorted(r, sizeof(r) / sizeof(r[0]), sizeof(r[0]), comparInt));
  ASSERT_FALSE(
      isSorted(r, sizeof(r) / sizeof(r[0]), sizeof(r[0]), comparInt_inv));
  int s[] = {5, 5, 5, 5};
  ASSERT_TRUE(isSorted(s, sizeof(s) / sizeof(s[0]), sizeof(s[0]), comparInt));
  ASSERT_TRUE(
      isSorted(s, sizeof(s) / sizeof(s[0]), sizeof(s[0]), comparInt_inv));
  int t[] = {4, 5};
  ASSERT_TRUE(isSorted(t, sizeof(t) / sizeof(t[0]), sizeof(t[0]), comparInt));
  ASSERT_FALSE(
      isSorted(t, sizeof(t) / sizeof(t[0]), sizeof(t[0]), comparInt_inv));
  int u[] = {5, 4};
  ASSERT_TRUE(
      isSorted(u, sizeof(u) / sizeof(u[0]), sizeof(u[0]), comparInt_inv));
  ASSERT_FALSE(isSorted(u, sizeof(u) / sizeof(u[0]), sizeof(u[0]), comparInt));

  int v[] = {0};
  ASSERT_TRUE(
      isSorted(v, sizeof(v) / sizeof(v[0]), sizeof(v[0]), comparInt_inv));
  ASSERT_TRUE(isSorted(v, sizeof(v) / sizeof(v[0]), sizeof(v[0]), comparInt));
}

TEST(Array, CountBinary) {
  int q[] = {5, 5, 10};
  ASSERT_EQ(
      CountBinary(&q[0], q, sizeof(q) / sizeof(q[0]), sizeof(q[0]), comparInt),
      2);
  int r[] = {10, 5, 5};
  ASSERT_EQ(CountBinary(&r[1], r, sizeof(r) / sizeof(r[0]), sizeof(r[0]),
                        comparInt_inv),
            2);
  int s[] = {1, 2, 5, 5, 5, 5, 10};
  ASSERT_EQ(
      CountBinary(&s[3], s, sizeof(s) / sizeof(s[0]), sizeof(s[0]), comparInt),
      4);
}

TEST(Array, CountLinear) {
  int q[] = {5, 5, 10};
  ASSERT_EQ(
      CountLinear(&q[0], q, sizeof(q) / sizeof(q[0]), sizeof(q[0]), comparInt),
      2);
}

TEST(Array, InterpolationSearch) {
  int q[] = {1, 5, 7, 10, 32, 78, 98};
  // start
  ASSERT_EQ(InterpolationSearch_rec(q[0], q, 0, 5, comparInt), &q[0]);

  ASSERT_EQ(InterpolationSearch_rec(q[4], q, 0, 5, comparInt), &q[4]);

  ASSERT_TRUE(InterpolationSearch_rec(q[0], q, 1, 1, comparInt) == NULL);
}

TEST(Array, Search) {

  int q[8] = {1, 2, 3, 4, 5, 6, 7};
  size_t element_size = sizeof(q[0]);
  size_t ar_size = sizeof(q) / element_size;

  int key;
  for (int i = 0; i < 7; i++) {
    key = q[i];
    ASSERT_EQ(LinearFind(&key, q, &ar_size, element_size, comparInt), &q[i]);
    ASSERT_EQ(bsearch(&key, q, 7, element_size, comparInt), &q[i]);
    ASSERT_EQ(BinarySearch(&key, q, 7, element_size, comparInt), &q[i]);
    ASSERT_EQ(BinarySearch_rec(&key, q, 0, 7, element_size, comparInt), &q[i]);
    ASSERT_TRUE(BinarySearch_rec(&key, q, 7, 1, element_size, comparInt) ==
                NULL);
  }
  key = 8;
  ASSERT_TRUE(LinearFind(&key, q, &ar_size, element_size, comparInt) == NULL);

  int *ret = (int *)LinearSearch(&key, q, &ar_size, element_size, comparInt);
  ASSERT_EQ(key, *ret);
}

TEST(DArray, init_free) {
  size_t n = 10;
  size_t st = 2;
  /* DArray_new */
  DArray *ar = NULL;
  ASSERT_EQ(ar, nullptr);

  ar = DArray_new(sizeof(int));
  ASSERT_NE(ar, nullptr);

  ASSERT_TRUE(DArray_isEmpty(ar));
  ASSERT_TRUE(DArray_isFull(ar));

  ASSERT_EQ(DArray_size(ar), 0);
  ASSERT_EQ(DArray_max_size(ar), 0);

  DArray_free(ar);
  /* DArray_sized_new */
  ar = DArray_sized_new(sizeof(int), n);
  ASSERT_NE(ar, nullptr);

  ASSERT_TRUE(DArray_isEmpty(ar));
  ASSERT_FALSE(DArray_isFull(ar));

  ASSERT_EQ(DArray_size(ar), 0);
  ASSERT_EQ(DArray_max_size(ar), n);

  DArray_flush(ar);
  ASSERT_EQ(DArray_size(ar), 0);
  ASSERT_EQ(DArray_max_size(ar), 0);

  DArray_free(ar);
  /* DArray_sized_step_new */
  ar = DArray_sized_step_new(sizeof(int), n, st);
  ASSERT_NE(ar, nullptr);

  ASSERT_TRUE(DArray_isEmpty(ar));
  ASSERT_FALSE(DArray_isFull(ar));

  ASSERT_EQ(DArray_size(ar), 0);
  ASSERT_EQ(DArray_max_size(ar), n);
  ASSERT_EQ(ar->step, st);

  DArray_free(ar);
  /* DArray_new_init */
  ar = DArray_new_init(sizeof(int), n, st, myCompare);
  ASSERT_NE(ar, nullptr);

  ASSERT_TRUE(DArray_isEmpty(ar));
  ASSERT_FALSE(DArray_isFull(ar));

  ASSERT_EQ(DArray_size(ar), 0);
  ASSERT_EQ(DArray_max_size(ar), n);
  ASSERT_EQ(ar->step, st);

  ASSERT_EQ(ar->compar((void *)"test", (void *)"azer"),
            myCompare((void *)"test", (void *)"azer"));

  DArray_free(ar);
}

TEST(DArray, setters) {
  size_t n = 10, st = 2;
  DArray *ar = NULL;
  ar = DArray_sized_new(sizeof(int), n);
  ASSERT_NE(ar, nullptr);
  ASSERT_EQ(ar->step, 1);
  ASSERT_TRUE(ar->compar == NULL);

  DArray_set_compar(ar, myCompare);
  ASSERT_EQ(ar->compar((void *)"test", (void *)"azer"),
            myCompare((void *)"test", (void *)"azer"));

  DArray_set_step(ar, st);
  ASSERT_EQ(ar->step, st);

  DArray_free(ar);
}

TEST(DArray, grow) {
  /* grow */
  size_t n = 10;
  DArray *ar = NULL;
  ASSERT_EQ(DArray_grow(ar, n, true), DARRAY_ERR_NOARRAY);

  ar = DArray_new(sizeof(int));
  ASSERT_NE(ar, nullptr);
  ASSERT_EQ(ar->nb_elem, 0);

  ASSERT_EQ(DArray_grow(ar, n, true), DARRAY_SUCCESS);
  ASSERT_EQ(ar->nb_elem, n);

  ASSERT_EQ(DArray_grow(ar, n - 5, true), DARRAY_ERR);
  ASSERT_EQ(ar->nb_elem, n);

  DArray_free(ar);
}

TEST(DArray, shrink) {
  DArray *ar = NULL;
  size_t n = 10, step = 2;
  /* shrink */
  ASSERT_EQ(DArray_shrink(ar, n, true), DARRAY_ERR_NOARRAY);
  ar = DArray_sized_step_new(sizeof(int), n, step);
  ASSERT_NE(ar, nullptr);
  ASSERT_EQ(ar->nb_elem, n);
  ASSERT_EQ(ar->pos, 0);

  ASSERT_EQ(DArray_shrink(ar, n, true), DARRAY_ERR);
  ASSERT_EQ(DArray_shrink(ar, n + 1, true), DARRAY_ERR);

  ASSERT_EQ(DArray_shrink(ar, n - 1, true), DARRAY_SUCCESS);
  ASSERT_EQ(ar->nb_elem, n - step);
  ASSERT_EQ(DArray_grow(ar, n, false), DARRAY_SUCCESS);
  ASSERT_EQ(ar->nb_elem, n);

  ASSERT_EQ(DArray_shrink(ar, n - 1, false), DARRAY_SUCCESS);
  ASSERT_EQ(ar->nb_elem, n - 1);

  DArray_free(ar);
}

TEST(DArray, shrink_to_fit) {
  DArray *ar = NULL;
  size_t n = 10;
  ASSERT_FALSE(DArray_shrink_to_fit(ar));
  /* shrink_to_fit */
  ar = DArray_sized_new(sizeof(int), n);
  ASSERT_NE(ar, nullptr);
  ASSERT_EQ(ar->nb_elem, n);
  ASSERT_EQ(ar->pos, 0);

  size_t ns2 = n / 2;
  ar->pos = ns2;
  ASSERT_EQ(ar->pos, ns2);
  ASSERT_EQ(ar->nb_elem, n);

  ASSERT_TRUE(DArray_shrink_to_fit(ar));
  ASSERT_EQ(ar->pos, ns2);
  ASSERT_EQ(ar->nb_elem, ns2);

  ar->pos = 0;
  ASSERT_EQ(ar->pos, 0);
  ASSERT_TRUE(DArray_shrink_to_fit(ar));
  ASSERT_EQ(ar->pos, 0);
  ASSERT_EQ(ar->nb_elem, 0);

  DArray_free(ar);
}

TEST(DArray, insert) {
  size_t n = 5, step = 2;
  char *q[10] = {"azer", "qsdf", "gfds", "hgd",    "try",
                 "rfty", "fgth", "xwv",  "ertfgh", "hgcv"};

  DArray *ar = NULL;

  /* test when array is NULL */
  ASSERT_EQ(DArray_append(ar, q[5]), DARRAY_ERR_NOARRAY);
  ASSERT_EQ(DArray_prepend(ar, q[5]), DARRAY_ERR_NOARRAY);
  ASSERT_EQ(DArray_insert_position(ar, q[5], 1), DARRAY_ERR_NOARRAY);

  ar = DArray_sized_step_new(sizeof(char *), n, step);
  ASSERT_NE(ar, nullptr);
  ASSERT_EQ(ar->nb_elem, n);

  /* append */
  ASSERT_EQ(DArray_append(ar, q[2]), DARRAY_SUCCESS);
  ASSERT_EQ(ar->nb_elem, n);
  ASSERT_EQ(ar->pos, 1);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 0), q[2]), 0);

  /* prepend */
  ASSERT_EQ(DArray_prepend(ar, q[0]), DARRAY_SUCCESS);
  ASSERT_EQ(ar->nb_elem, n);
  ASSERT_EQ(ar->pos, 2);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 0), q[0]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 1), q[2]), 0);

  /* insert_position */
  ASSERT_EQ(DArray_insert_position(ar, q[1], 1), DARRAY_SUCCESS);
  ASSERT_EQ(ar->nb_elem, n);
  ASSERT_EQ(ar->pos, 3);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 0), q[0]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 1), q[1]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 2), q[2]), 0);

  ASSERT_EQ(DArray_insert_position(ar, q[3], 0), DARRAY_SUCCESS);
  ASSERT_EQ(ar->nb_elem, n);
  ASSERT_EQ(ar->pos, 4);
  ASSERT_TRUE(DArray_isFull(ar));
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 0), q[3]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 1), q[0]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 2), q[1]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 3), q[2]), 0);

  ASSERT_EQ(DArray_insert_position(ar, q[4], 4), DARRAY_SUCCESS);
  ASSERT_EQ(ar->nb_elem, n + step);
  ASSERT_EQ(ar->pos, 5);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 0), q[3]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 1), q[0]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 2), q[1]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 3), q[2]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 4), q[4]), 0);

  DArray_grow(ar, 1, true);
  ASSERT_EQ(DArray_insert_position(ar, q[5], n + 1), DARRAY_ERR_NOTRANGE);

  /* unique_insert */
  ASSERT_EQ(DArray_unique_insert(NULL, q[4]), DARRAY_ERR_NOARRAY);
  ASSERT_EQ(DArray_unique_insert(ar, q[4]), DARRAY_ERR_NOCOMPAR);
  DArray_set_compar(ar, myCompare);
  ASSERT_EQ(DArray_unique_insert(ar, q[4]), DARRAY_ERR_EXIST);

  ASSERT_EQ(DArray_unique_insert(ar, q[5]), DARRAY_SUCCESS);
  ASSERT_EQ(ar->nb_elem, n + step);
  ASSERT_EQ(ar->pos, 6);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 0), q[3]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 1), q[0]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 2), q[1]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 3), q[2]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 4), q[4]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 5), q[5]), 0);
  DArray_free(ar);

  ar = DArray_new(sizeof(char *));
  ASSERT_NE(ar, nullptr);
  ASSERT_EQ(DArray_prepend(ar, q[5]), DARRAY_SUCCESS);

  DArray_free(ar);
}

TEST(DArray, set_position) {
  size_t n = 5, step = 2;
  char *q[10] = {"azer", "qsdf", "gfds", "hgd",    "try",
                 "rfty", "fgth", "xwv",  "ertfgh", "hgcv"};

  DArray *ar = NULL;
  ASSERT_EQ(DArray_set_position(ar, q[4], 44), DARRAY_ERR_NOARRAY);

  ar = DArray_sized_step_new(sizeof(char *), n, step);
  ASSERT_NE(ar, nullptr);
  ASSERT_EQ(ar->nb_elem, n);
  DArray_append(ar, q[0]);

  ASSERT_EQ(DArray_set_position(ar, q[0], 44), DARRAY_ERR_NOTRANGE);
  ASSERT_EQ(DArray_set_position(ar, q[1], 1), DARRAY_SUCCESS);

  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 0), q[0]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 1), q[1]), 0);

  DArray_free(ar);
}

TEST(DArray, get_position) {
  DArray *ar = NULL;
  ASSERT_FALSE(DArray_get_position(ar, 0));
  ar = DArray_new(sizeof(char *));
  ASSERT_NE(ar, nullptr);

  ASSERT_FALSE(DArray_get_position(ar, 0));
  DArray_append(ar, "test");
  ASSERT_EQ(DArray_get_position(ar, 1), nullptr);
  ASSERT_TRUE(myCompare("test", DArray_get_position(ar, 0)) == 0);
}

TEST(DArray, remove) {
  size_t n = 5, step = 2;
  char *q[10] = {"azer", "qsdf", "gfds", "hgd",    "try",
                 "rfty", "fgth", "xwv",  "ertfgh", "hgcv"};

  DArray *ar = NULL;
  /* test when array is NULL */
  ASSERT_EQ(DArray_remove_tail(ar, NULL), DARRAY_ERR_NOARRAY);
  ASSERT_EQ(DArray_remove_head(ar, NULL), DARRAY_ERR_NOARRAY);
  ASSERT_EQ(DArray_remove_position(ar, NULL, 1), DARRAY_ERR_NOARRAY);

  ar = DArray_sized_step_new(sizeof(char *), n, step);
  ASSERT_NE(ar, nullptr);
  ASSERT_EQ(ar->nb_elem, n);

  DArray_append(ar, q[0]);
  DArray_append(ar, q[1]);
  DArray_append(ar, q[2]);
  DArray_append(ar, q[3]);
  DArray_append(ar, q[4]);
  DArray_append(ar, q[5]);
  ASSERT_EQ(ar->nb_elem, n + step);
  size_t pos = n + step - 1;
  ASSERT_EQ(ar->pos, pos);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 0), q[0]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 1), q[1]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 2), q[2]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 3), q[3]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 4), q[4]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 5), q[5]), 0);

  void **data = (void **)malloc(sizeof(void *));
  ASSERT_NE(data, nullptr);
  ASSERT_EQ(DArray_remove_tail(ar, data), DARRAY_SUCCESS);
  ASSERT_NE(data, nullptr);
  ASSERT_NE(*data, nullptr);
  ASSERT_EQ(strcmp((char *)*data, q[5]), 0);
  ASSERT_EQ(ar->nb_elem, n + step);
  ASSERT_EQ(ar->pos, --pos);

  ASSERT_EQ(DArray_remove_head(ar, data), DARRAY_SUCCESS);
  ASSERT_EQ(strcmp((char *)*data, q[0]), 0);
  ASSERT_EQ(ar->nb_elem, n + step);
  ASSERT_EQ(ar->pos, --pos);
  ASSERT_EQ(myCompare(ar->data[0], q[1]), 0);

  ASSERT_EQ(DArray_remove_position(ar, data, 200), DARRAY_ERR_NOTRANGE);
  ASSERT_EQ(DArray_remove_position(ar, data, 2), DARRAY_SUCCESS);
  ASSERT_EQ(strcmp((char *)*data, q[3]), 0);
  ASSERT_EQ(ar->nb_elem, n + step);
  ASSERT_EQ(ar->pos, --pos);

  DArray_free(ar);
  ar = NULL;

  ASSERT_EQ(DArray_remove(ar, data), DARRAY_ERR_NOARRAY);
  ar = DArray_sized_step_new(sizeof(int), n, step);
  ASSERT_NE(ar, nullptr);
  ASSERT_EQ(ar->nb_elem, n);
  DArray_append(ar, q[0]);
  DArray_append(ar, q[1]);
  DArray_append(ar, q[2]);
  DArray_append(ar, q[3]);
  DArray_append(ar, q[4]);

  *data = (void *)q[6];
  ASSERT_EQ(DArray_remove(ar, data), DARRAY_ERR_NOCOMPAR);

  DArray_set_compar(ar, myCompare);
  ASSERT_EQ(DArray_remove(ar, data), DARRAY_ERR);

  *data = (void *)q[1];
  ASSERT_EQ(DArray_remove(ar, data), DARRAY_SUCCESS);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 0), q[0]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 1), q[2]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 2), q[3]), 0);
  ASSERT_EQ(strcmp((char *)DArray_get_position(ar, 3), q[4]), 0);

  ASSERT_EQ(DArray_remove_position(ar, NULL, 0), DARRAY_SUCCESS);
  ASSERT_EQ(DArray_remove_position(ar, NULL, ar->pos), DARRAY_SUCCESS);
  ASSERT_EQ(DArray_remove_position(ar, NULL, ar->pos + 1), DARRAY_ERR_NOTRANGE);
  free(data);
}

int main(int argc, char *argv[]) {
  srand((unsigned)time(NULL));
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
