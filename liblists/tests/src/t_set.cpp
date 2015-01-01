extern "C" {
#include "Set.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>

char str[100];
static void affInt(const void *data) {
  char tmp[100];
  intptr_t d = (intptr_t)data;
  snprintf(tmp, 100, "%ld", d);
  if (strlen(str) > 0) {
    strncat(str, " ", 1);
  }
  size_t size;
  size = 100 - strlen(str) - 1;
  strncat(str, tmp, size);
}

static auto printSL(SList *l) -> char * {
  memset(str, 0, 100);
  SList_foreach_safe(l, affInt);
  return str;
}
}
#include <gtest/gtest.h>

TEST(Set /*unused*/, createSet /*unused*/) {
  Set *set = Set_new(NULL, NULL);
  ASSERT_EQ(set, nullptr);
  ASSERT_EQ(Set_isEmpty(set), true);
  set = Set_new(NULL, comparIntptr);
  ASSERT_NE(set, nullptr);
  ASSERT_EQ(Set_isEmpty(set), true);

  Set_insert(set, (const void *)(intptr_t)1);
  Set_insert(set, (const void *)(intptr_t)2);
  Set_insert(set, (const void *)(intptr_t)3);
  Set_insert(set, (const void *)(intptr_t)4);
  Set_insert(set, (const void *)(intptr_t)5);
  ASSERT_EQ(5, Set_size(set));
  ASSERT_EQ(Set_isEmpty(set), false);

  Set_insert(set, (const void *)(intptr_t)1);
  Set_insert(set, (const void *)(intptr_t)2);
  Set_insert(set, (const void *)(intptr_t)3);
  Set_insert(set, (const void *)(intptr_t)4);
  Set_insert(set, (const void *)(intptr_t)5);
  ASSERT_EQ(5, Set_size(set));

  Set_insert(set, (const void *)(intptr_t)6);
  ASSERT_EQ(6, Set_size(set));

  Set_remove(set, (const void *)(intptr_t)1);
  Set_remove(set, (const void *)(intptr_t)2);
  Set_remove(set, (const void *)(intptr_t)3);
  Set_remove(set, (const void *)(intptr_t)4);
  ASSERT_EQ(2, Set_size(set));

  Set_destroy(set);
}

TEST(Set /*unused*/, Set_operations /*unused*/) {
  Set *set1 = Set_new(NULL, comparIntptr);
  ASSERT_NE(set1, nullptr);
  Set *set2 = Set_new(NULL, comparIntptr);
  ASSERT_NE(set2, nullptr);

  Set_insert(set1, (const void *)(intptr_t)1);
  Set_insert(set1, (const void *)(intptr_t)2);
  Set_insert(set1, (const void *)(intptr_t)3);
  Set_insert(set1, (const void *)(intptr_t)4);
  Set_insert(set1, (const void *)(intptr_t)5);

  Set_insert(set2, (const void *)(intptr_t)1);
  Set_insert(set2, (const void *)(intptr_t)12);
  Set_insert(set2, (const void *)(intptr_t)31);
  Set_insert(set2, (const void *)(intptr_t)41);
  Set_insert(set2, (const void *)(intptr_t)5);

  Set *set_union = Set_union(set1, set2);
  ASSERT_NE(set_union, nullptr);
  ASSERT_EQ(8, Set_size(set_union));
  set1->compar = NULL;
  ASSERT_FALSE(Set_union(set1, set2));
  set1->compar = comparIntptr;
  set2->compar = NULL;
  ASSERT_FALSE(Set_union(set1, set2));
  set2->compar = comparIntptr;
  ASSERT_FALSE(Set_union(NULL, set2));
  ASSERT_FALSE(Set_union(set1, NULL));

  Set *set_inter = Set_intersection(set1, set2);
  ASSERT_NE(set_inter, nullptr);
  ASSERT_EQ(2, Set_size(set_inter));
  set1->compar = NULL;
  ASSERT_FALSE(Set_intersection(set1, set2));
  set1->compar = comparIntptr;
  set2->compar = NULL;
  ASSERT_FALSE(Set_intersection(set1, set2));
  set2->compar = comparIntptr;
  ASSERT_FALSE(Set_intersection(NULL, set2));
  ASSERT_FALSE(Set_intersection(set1, NULL));

  Set *set_diff = Set_difference(set1, set2);
  set1->compar = NULL;
  ASSERT_FALSE(Set_difference(set1, set2));
  set1->compar = comparIntptr;
  set2->compar = NULL;
  ASSERT_FALSE(Set_difference(set1, set2));
  set2->compar = comparIntptr;
  ASSERT_FALSE(Set_difference(NULL, set2));
  ASSERT_FALSE(Set_difference(set1, NULL));
  ASSERT_NE(set_diff, nullptr);
  ASSERT_EQ(3, Set_size(set_diff));
  Set_destroy(set_diff);
  set_diff = Set_difference(set2, set1);
  ASSERT_NE(set_diff, nullptr);
  ASSERT_EQ(3, Set_size(set_diff));
  Set_destroy(set_diff);

  set_diff = Set_symetric_difference(set1, set2);
  ASSERT_NE(set_diff, nullptr);
  ASSERT_EQ(6, Set_size(set_diff));
  ASSERT_STREQ("2 3 4 12 31 41", printSL(set_diff));

  Set_destroy(set_union);
  Set_destroy(set_inter);
  Set_destroy(set_diff);
  Set_destroy(set1);
  Set_destroy(set2);
}

TEST(Set /*unused*/, Set_isSubset /*unused*/) {

  Set *set1 = Set_new(NULL, comparIntptr);
  ASSERT_NE(set1, nullptr);
  Set *setu = Set_new(NULL, comparIntptr);
  ASSERT_NE(setu, nullptr);

  Set_insert(set1, (const void *)(intptr_t)1);
  Set_insert(set1, (const void *)(intptr_t)2);
  Set_insert(set1, (const void *)(intptr_t)3);
  Set_insert(set1, (const void *)(intptr_t)4);
  Set_insert(set1, (const void *)(intptr_t)5);

  Set_insert(setu, (const void *)(intptr_t)1);
  Set_insert(setu, (const void *)(intptr_t)2);
  Set_insert(setu, (const void *)(intptr_t)3);

  ASSERT_TRUE(Set_isSubset(setu, set1));
  Set_insert(setu, (const void *)(intptr_t)4);
  Set_insert(setu, (const void *)(intptr_t)5);
  Set_insert(setu, (const void *)(intptr_t)6);
  ASSERT_FALSE(Set_isSubset(setu, set1));
  Set_insert(setu, (const void *)(intptr_t)6);
  Set_insert(setu, (const void *)(intptr_t)3);
  ASSERT_FALSE(Set_isSubset(setu, set1));

  Set_destroy(setu);
  Set_destroy(set1);
}

TEST(Set /*unused*/, Set_isSuperset /*unused*/) {
  Set *set1 = Set_new(NULL, comparIntptr);
  ASSERT_NE(set1, nullptr);
  Set *setu = Set_new(NULL, comparIntptr);
  ASSERT_NE(setu, nullptr);

  Set_insert(set1, (const void *)(intptr_t)1);
  Set_insert(set1, (const void *)(intptr_t)2);
  Set_insert(set1, (const void *)(intptr_t)3);
  Set_insert(set1, (const void *)(intptr_t)4);
  Set_insert(set1, (const void *)(intptr_t)5);

  Set_insert(setu, (const void *)(intptr_t)1);
  Set_insert(setu, (const void *)(intptr_t)2);
  Set_insert(setu, (const void *)(intptr_t)3);
  ASSERT_FALSE(Set_isSuperset(setu, set1));

  Set_insert(setu, (const void *)(intptr_t)4);
  Set_insert(setu, (const void *)(intptr_t)5);
  Set_insert(setu, (const void *)(intptr_t)6);
  ASSERT_TRUE(Set_isSuperset(setu, set1));

  Set_remove(setu, (const void *)(intptr_t)3);
  Set_insert(setu, (const void *)(intptr_t)7);
  ASSERT_FALSE(Set_isSuperset(setu, set1));

  Set_destroy(setu);
  Set_destroy(set1);
}

TEST(Set /*unused*/, Set_isDisjoint /*unused*/) {
  Set *set1 = Set_new(NULL, comparIntptr);
  ASSERT_NE(set1, nullptr);
  Set *set2 = Set_new(NULL, comparIntptr);
  ASSERT_NE(set2, nullptr);

  Set_insert(set1, (const void *)(intptr_t)1);
  Set_insert(set1, (const void *)(intptr_t)2);
  Set_insert(set1, (const void *)(intptr_t)3);
  Set_insert(set1, (const void *)(intptr_t)4);
  Set_insert(set1, (const void *)(intptr_t)5);

  Set_insert(set2, (const void *)(intptr_t)6);
  Set_insert(set2, (const void *)(intptr_t)7);
  Set_insert(set2, (const void *)(intptr_t)8);
  ASSERT_TRUE(Set_isDisjoint(set2, set1));

  Set_insert(set2, (const void *)(intptr_t)1);
  ASSERT_FALSE(Set_isSuperset(set2, set1));
  ASSERT_FALSE(Set_isDisjoint(set2, set1));

  Set_destroy(set2);
  Set_destroy(set1);
}

TEST(Set /*unused*/, Set_isEqual /*unused*/) {
  Set *set1 = Set_new(NULL, comparIntptr);
  ASSERT_NE(set1, nullptr);
  Set *set2 = Set_new(NULL, comparIntptr);
  ASSERT_NE(set2, nullptr);

  Set_insert(set1, (const void *)(intptr_t)1);
  Set_insert(set1, (const void *)(intptr_t)2);
  Set_insert(set1, (const void *)(intptr_t)3);
  Set_insert(set1, (const void *)(intptr_t)4);
  Set_insert(set1, (const void *)(intptr_t)5);

  Set_insert(set2, (const void *)(intptr_t)1);
  Set_insert(set2, (const void *)(intptr_t)2);
  Set_insert(set2, (const void *)(intptr_t)3);
  ASSERT_FALSE(Set_isEqual(set1, set2));

  Set_insert(set2, (const void *)(intptr_t)4);
  Set_insert(set2, (const void *)(intptr_t)5);
  ASSERT_TRUE(Set_isEqual(set1, set2));

  Set_insert(set2, (const void *)(intptr_t)6);
  ASSERT_FALSE(Set_isEqual(set1, set2));

  Set_remove(set2, (const void *)(intptr_t)4);
  ASSERT_FALSE(Set_isEqual(set1, set2));

  Set_destroy(set2);
  Set_destroy(set1);
}
