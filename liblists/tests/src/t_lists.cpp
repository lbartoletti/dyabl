#include "t_lists.hpp"

TEST(SList /*unused*/, init /*unused*/) {

  SList *list = NULL;
  list = SList_new();
  ASSERT_NE(list, nullptr);
  SList_free(list);
  list = SList_new_init(NULL, NULL);
  ASSERT_NE(list, nullptr);
  SList_free(list);
  list = NULL;
  ASSERT_EQ(SList_isEmpty(list), true);
}

TEST(SList /*unused*/, insertTests /*unused*/) {

  SList *listInt = NULL;
  listInt = SList_new_init(del, comparIntptr);
  ASSERT_NE(listInt, nullptr);

  ASSERT_EQ(SList_append(NULL, (const void *)(intptr_t)q[1]), SLIST_ERR_NOLIST);
  ASSERT_EQ(SList_prepend(NULL, (const void *)(intptr_t)q[1]),
            SLIST_ERR_NOLIST);
  ASSERT_EQ(SList_insert_position(NULL, (const void *)(intptr_t)q[1], 2),
            SLIST_ERR_NOLIST);

  SList_append(listInt, (const void *)(intptr_t)q[1]);
  ASSERT_EQ(78, (intptr_t)SList_head(listInt)->data);
  ASSERT_EQ(78, (intptr_t)SList_tail(listInt)->data);
  ASSERT_EQ(1, SList_length(listInt));
  ASSERT_STREQ("78", printSL(listInt));

  SList_prepend(listInt, (const void *)(intptr_t)q[0]);
  ASSERT_STREQ("65 78", printSL(listInt));

  SList_append(listInt, (const void *)(intptr_t)q[4]);
  ASSERT_STREQ("65 78 78", printSL(listInt));

  ASSERT_EQ(SList_insert_position(listInt, (const void *)(intptr_t)q[1],
                                  -(SList_length(listInt) + 1)),
            SLIST_ERR);
  ASSERT_EQ(SList_insert_position(listInt, (const void *)(intptr_t)q[1],
                                  (SList_length(listInt) + 1)),
            SLIST_ERR);
  SList_insert_position(listInt, (const void *)(intptr_t)q[2], 2);
  ASSERT_STREQ("65 78 98 78", printSL(listInt));
  SList_insert_position(listInt, (const void *)(intptr_t)q[2], 0);
  ASSERT_STREQ("98 65 78 98 78", printSL(listInt));
  SList_insert_position(listInt, (const void *)(intptr_t)q[2],
                        SList_length(listInt));
  ASSERT_STREQ("98 65 78 98 78 98", printSL(listInt));

  SList_free(listInt);
}

TEST(SList /*unused*/, removeTests /*unused*/) {
  SList *listInt = NULL;
  ASSERT_EQ(SList_remove_tail(listInt, NULL), SLIST_ERR_NOLIST);
  ASSERT_EQ(SList_remove_head(listInt, NULL), SLIST_ERR_NOLIST);
  ASSERT_EQ(SList_remove_next(listInt, NULL), SLIST_ERR_NOLIST);
  ASSERT_EQ(SList_remove(listInt, NULL), SLIST_ERR_NOLIST);
  ASSERT_EQ(SList_remove_all(listInt, NULL), SLIST_ERR_NOLIST);
  SList_free(listInt);

  listInt = SList_new();
  ASSERT_NE(listInt, nullptr);
  ASSERT_EQ(SList_remove(listInt, NULL), SLIST_ERR_NOCOMPAR);
  ASSERT_EQ(SList_remove_all(listInt, NULL), SLIST_ERR_NOCOMPAR);
  SList_free(listInt);

  listInt = SList_new_init(del, comparIntptr);
  ASSERT_NE(listInt, nullptr);
  ASSERT_EQ(SList_remove_head(listInt, NULL), SLIST_ERR);
  ASSERT_EQ(SList_remove_next(listInt, NULL), SLIST_ERR);

  for (int i : q) {
    SList_append(listInt, (const void *)(intptr_t)i);
  }

  void *data;
  SList_remove_tail(listInt, &data);
  ASSERT_STREQ("65 78 98 12 78 312", printSL(listInt));
  ASSERT_EQ(6, (intptr_t)data);
  ASSERT_TRUE(312 == (intptr_t)listInt->elem->data);

  SList_remove_head(listInt, &data);
  ASSERT_STREQ("78 98 12 78 312", printSL(listInt));
  ASSERT_EQ(65, (intptr_t)data);
  ASSERT_TRUE(78 == (intptr_t)listInt->elem->data);

  SList_remove(listInt, (const void *)(intptr_t)12);
  ASSERT_STREQ("78 98 78 312", printSL(listInt));
  ASSERT_TRUE(98 == (intptr_t)listInt->elem->data);

  SList_remove(listInt, (const void *)(intptr_t)5);
  ASSERT_STREQ("78 98 78 312", printSL(listInt));
  ASSERT_TRUE(78 == (intptr_t)listInt->elem->data);

  SList_remove_all(listInt, (const void *)(intptr_t)78);
  ASSERT_STREQ("98 312", printSL(listInt));
  ASSERT_TRUE(98 == (intptr_t)listInt->elem->data);
  ASSERT_FALSE(listInt->elem == NULL);
  ASSERT_TRUE(SList_isFirst(listInt, listInt->elem));

  SList_remove_next(listInt, &data);
  ASSERT_STREQ("98", printSL(listInt));

  SList_remove(listInt, listInt->elem->data);
  ASSERT_TRUE(SList_head(listInt) == SList_tail(listInt));
  ASSERT_TRUE(SList_isEmpty(listInt));

  for (int i : q) {
    SList_append(listInt, (const void *)(intptr_t)i);
  }

  ASSERT_STREQ("65 78 98 12 78 312 6", printSL(listInt));
  ASSERT_TRUE(SList_head(listInt)->data == (const void *)(intptr_t)65);
  ASSERT_TRUE(SList_tail(listInt)->data == (const void *)(intptr_t)6);

  SList_remove_all(listInt, (const void *)(intptr_t)5);
  ASSERT_STREQ("65 78 98 12 78 312 6", printSL(listInt));
  ASSERT_TRUE(65 == (intptr_t)listInt->elem->data);

}

TEST(SList /*unused*/, utilityTests /*unused*/) {
  SList *listInt = NULL;
  ASSERT_EQ(SList_sort(listInt), SLIST_ERR_NOLIST);
  ASSERT_EQ(SList_index(listInt, nullptr), SLIST_ERR_NOLIST);
  ASSERT_TRUE(SList_find(listInt, nullptr) == NULL);
  ASSERT_EQ(SList_free(listInt), SLIST_ERR_NOLIST);
  ASSERT_EQ(SList_empty(listInt), SLIST_ERR_NOLIST);
  ASSERT_TRUE(SList_reverse(listInt) == NULL);
  SList_free(listInt);
  listInt = SList_new_init(NULL, NULL);
  ASSERT_NE(listInt, nullptr);
  ASSERT_EQ(SList_sort(listInt), SLIST_ERR_NOCOMPAR);
  ASSERT_EQ(SList_index(listInt, nullptr), SLIST_ERR_NOCOMPAR);
  ASSERT_TRUE(SList_find(listInt, nullptr) == NULL);
  SList_free(listInt);
  listInt = SList_new_init(del, comparIntptr);
  ASSERT_NE(listInt, nullptr);

  for (int i : q) {
    SList_append(listInt, (const void *)(intptr_t)i);
  }
  ASSERT_STREQ("65 78 98 12 78 312 6", printSL(listInt));

  ASSERT_EQ(SList_index(listInt, (const void *)(intptr_t)65), 0);
  ASSERT_EQ(SList_index(listInt, (const void *)(intptr_t)78), 1);
  ASSERT_EQ(SList_find(listInt, (const void *)(intptr_t)65),
            SList_head(listInt));
  ASSERT_EQ(SList_find(listInt, (const void *)(intptr_t)6),
            SList_tail(listInt));
  ASSERT_EQ(SList_find(listInt, (const void *)(intptr_t)78),
            (SList_head(listInt))->next);

  SList *r = SList_reverse(listInt);
  ASSERT_NE(r, nullptr);
  ASSERT_STREQ("6 312 78 12 98 78 65", printSL(r));
  SList_free(r);

  SList_sort(listInt);
  ASSERT_STREQ("6 12 65 78 78 98 312", printSL(listInt));
  ASSERT_EQ(SList_isEmpty(listInt), false);

  SList_free(listInt);
}

TEST(SList /*unused*/, foreachTests /*unused*/) {
  SList *listInt = NULL;
  ASSERT_EQ(SList_foreach_safe(listInt, constPrint), SLIST_ERR_NOLIST);
  ASSERT_EQ(SList_foreach(listInt, myFunc), SLIST_ERR_NOLIST);

  listInt = SList_new();
  ASSERT_NE(listInt, nullptr);
  for (int i : q) {
    SList_append(listInt, (const void *)(intptr_t)i);
  }
  ASSERT_STREQ("65 78 98 12 78 312 6", printSL(listInt));

  ASSERT_EQ(SList_foreach_safe(listInt, constPrint), SLIST_SUCCESS);
  SList_foreach(listInt, myFunc);
  ASSERT_STREQ("66 79 99 13 79 313 7", printSL(listInt));
}

TEST(SList /*unused*/, uniqueTests /*unused*/) {
  SList *listInt = NULL;
  ASSERT_EQ(SList_unique_insert(listInt, (const void *)(intptr_t)1),
            SLIST_ERR_NOLIST);
  SList_free(listInt);
  listInt = SList_new_init(NULL, NULL);
  ASSERT_NE(listInt, nullptr);
  ASSERT_EQ(SList_unique_insert(listInt, (const void *)(intptr_t)1),
            SLIST_ERR_NOCOMPAR);
  SList_free(listInt);

  listInt = SList_new_init(NULL, comparIntptr);
  ASSERT_NE(listInt, nullptr);

  SList_unique_insert(listInt, (const void *)(intptr_t)1);
  SList_unique_insert(listInt, (const void *)(intptr_t)2);
  SList_unique_insert(listInt, (const void *)(intptr_t)3);
  SList_unique_insert(listInt, (const void *)(intptr_t)4);
  SList_unique_insert(listInt, (const void *)(intptr_t)5);
  ASSERT_EQ(5, SList_length(listInt));

  SList_unique_insert(listInt, (const void *)(intptr_t)1);
  SList_unique_insert(listInt, (const void *)(intptr_t)2);
  SList_unique_insert(listInt, (const void *)(intptr_t)3);
  SList_unique_insert(listInt, (const void *)(intptr_t)4);
  SList_unique_insert(listInt, (const void *)(intptr_t)5);
  ASSERT_EQ(5, SList_length(listInt));

  SList_unique_insert(listInt, (const void *)(intptr_t)6);
  ASSERT_EQ(6, SList_length(listInt));

  SList_free(listInt);
}
