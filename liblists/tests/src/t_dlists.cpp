#include "t_lists.hpp"

/**********************/
/*     DLIST          */
/**********************/

TEST(DList /*unused*/, insertTests /*unused*/) {

  DList *listInt = NULL;
  listInt = DList_new_init(del, comparIntptr);
  ASSERT_NE(listInt, nullptr);

  ASSERT_EQ(DList_append(NULL, NULL), DLIST_ERNOLIST);
  ASSERT_EQ(DList_prepend(NULL, NULL), DLIST_ERNOLIST);
  ASSERT_EQ(DList_insert_position(NULL, NULL, 0), DLIST_ERNOLIST);

  ASSERT_EQ(DList_append(listInt, (const void *)(intptr_t)q[1]), DLIST_SUCCESS);
  ASSERT_EQ(78, (intptr_t)DList_head(listInt)->data);
  ASSERT_EQ(78, (intptr_t)DList_tail(listInt)->data);
  ASSERT_EQ(1, DList_length(listInt));
  ASSERT_STREQ("78", printDL(listInt));

  ASSERT_EQ(DList_prepend(listInt, (const void *)(intptr_t)q[0]),
            DLIST_SUCCESS);
  ASSERT_STREQ("65 78", printDL(listInt));

  ASSERT_EQ(DList_append(listInt, (const void *)(intptr_t)q[4]), DLIST_SUCCESS);
  ASSERT_STREQ("65 78 78", printDL(listInt));

  ASSERT_EQ(DList_insert_position(listInt, (const void *)(intptr_t)q[2], 2),
            DLIST_SUCCESS);
  ASSERT_STREQ("65 78 98 78", printDL(listInt));

  DList_free(listInt);
  listInt = NULL;
  ASSERT_EQ(DList_isEmpty(listInt), true);
}

TEST(DList /*unused*/, removeTests /*unused*/) {
  DList *listInt = NULL;
  listInt = DList_new();
  ASSERT_NE(listInt, nullptr);

  ASSERT_EQ(DList_remove(NULL, NULL), DLIST_ERNOLIST);
  ASSERT_EQ(DList_remove(listInt, NULL), DLIST_ERNOCMP);
  ASSERT_EQ(DList_remove_all(NULL, NULL), DLIST_ERNOLIST);
  ASSERT_EQ(DList_remove_all(listInt, NULL), DLIST_ERNOCMP);
  ASSERT_EQ(DList_remove_tail(NULL, NULL), DLIST_ERNOLIST);
  ASSERT_EQ(DList_remove_tail(listInt, NULL), DLIST_ERROR);
  ASSERT_EQ(DList_remove_head(NULL, NULL), DLIST_ERNOLIST);
  ASSERT_EQ(DList_remove_head(listInt, NULL), DLIST_ERROR);
  ASSERT_EQ(DList_remove_next(NULL, NULL), DLIST_ERNOLIST);
  ASSERT_EQ(DList_remove_next(listInt, NULL), DLIST_ERROR);

  listInt->compar = comparIntptr;
  listInt->destroy = del;

  for (int i : q) {
    DList_append(listInt, (const void *)(intptr_t)i);
  }

  int fakeData;
  DList_remove_tail(listInt, (void **)&fakeData);
  ASSERT_STREQ("65 78 98 12 78 312", printDL(listInt));
  ASSERT_TRUE(312 == (intptr_t)listInt->elem->data);

  DList_remove_head(listInt, (void **)&fakeData);
  ASSERT_STREQ("78 98 12 78 312", printDL(listInt));
  ASSERT_TRUE(78 == (intptr_t)listInt->elem->data);

  DList_remove(listInt, (const void *)(intptr_t)12);
  ASSERT_STREQ("78 98 78 312", printDL(listInt));
  ASSERT_TRUE(98 == (intptr_t)listInt->elem->data);

  DList_remove(listInt, (const void *)(intptr_t)5);
  ASSERT_STREQ("78 98 78 312", printDL(listInt));
  ASSERT_TRUE(78 == (intptr_t)listInt->elem->data);

  DList_remove_all(listInt, (const void *)(intptr_t)78);
  ASSERT_STREQ("98 312", printDL(listInt));
  ASSERT_TRUE(98 == (intptr_t)listInt->elem->data);
  ASSERT_NE(listInt->elem, nullptr);
  ASSERT_TRUE(DList_isFirst(listInt, listInt->elem));

  DList_remove_next(listInt, (void **)&fakeData);
  ASSERT_STREQ("98", printDL(listInt));

  DList_remove(listInt, listInt->elem->data);
  ASSERT_TRUE(DList_head(listInt) == DList_tail(listInt));
  ASSERT_TRUE(DList_isEmpty(listInt));

  for (int i : q) {
    DList_append(listInt, (const void *)(intptr_t)i);
  }

  ASSERT_STREQ("65 78 98 12 78 312 6", printDL(listInt));
  ASSERT_TRUE(DList_head(listInt)->data == (const void *)(intptr_t)65);
  ASSERT_TRUE(DList_tail(listInt)->data == (const void *)(intptr_t)6);

  /*    int **data;
      DList_remove_next(listInt, (void**)data);
      ASSERT_EQ((const void*)(intptr_t)6, *data);*/

  DList_remove_all(listInt, (const void *)(intptr_t)5);
  ASSERT_STREQ("65 78 98 12 78 312 6", printDL(listInt));
  ASSERT_TRUE(65 == (intptr_t)listInt->elem->data);

  DList_free(listInt);
}

TEST(DList /*unused*/, utilityTests /*unused*/) {
  DList *listInt = NULL;
  listInt = DList_new_init(del, comparIntptr);
  ASSERT_NE(listInt, nullptr);

  for (int i : q) {
    DList_append(listInt, (const void *)(intptr_t)i);
  }
  ASSERT_STREQ("65 78 98 12 78 312 6", printDL(listInt));

  DList *r = DList_reverse(listInt);
  ASSERT_NE(r, nullptr);
  ASSERT_STREQ("6 312 78 12 98 78 65", printDL(r));
  DList_free(r);

  DList_sort(listInt);
  ASSERT_STREQ("6 12 65 78 78 98 312", printDL(listInt));
  DList_free(listInt);
}

TEST(DList /*unused*/, uniqueTests /*unused*/) {
  DList *listInt = NULL;
  listInt = DList_new_init(NULL, comparIntptr);
  ASSERT_NE(listInt, nullptr);

  DList_unique_insert(listInt, (const void *)(intptr_t)1);
  DList_unique_insert(listInt, (const void *)(intptr_t)2);
  DList_unique_insert(listInt, (const void *)(intptr_t)3);
  DList_unique_insert(listInt, (const void *)(intptr_t)4);
  DList_unique_insert(listInt, (const void *)(intptr_t)5);
  ASSERT_EQ(5, DList_length(listInt));

  DList_unique_insert(listInt, (const void *)(intptr_t)1);
  DList_unique_insert(listInt, (const void *)(intptr_t)2);
  DList_unique_insert(listInt, (const void *)(intptr_t)3);
  DList_unique_insert(listInt, (const void *)(intptr_t)4);
  DList_unique_insert(listInt, (const void *)(intptr_t)5);
  ASSERT_EQ(5, DList_length(listInt));

  DList_unique_insert(listInt, (const void *)(intptr_t)6);
  ASSERT_EQ(6, DList_length(listInt));

  DList_free(listInt);
}
