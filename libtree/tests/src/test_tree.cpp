extern "C" {
#include "AVLTree.h"
#include "Array.h"
#include "Utils.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

static void fakeDestroy(void *data) { printf("rm %ld\n", (intptr_t)data); }

/*static void
aff (BiTree_node *racine, unsigned long prof)
{
    unsigned long i;
    for (i = 0; i < prof; i++) {
        fputs ("|___ ", stdout);
    }

    if (racine) {
        printf ("[%ld (%d) {%d}]\n", (intptr_t)racine->data, racine->level,
racine->balance);

        if (racine->left)
            aff (racine->left, prof + 1);
        if (racine->right)
            aff (racine->right, prof + 1);
    }
    else
        printf ("NULL\n");
}*/

int q[] = {20, 4, 26, 3, 9, 21, 30, 2, 7, 11};

static int getDepth(AVLTree_node *n) {
  int leftHeight = 0, rightHeight = 0;

  if (n->right != NULL)
    rightHeight = getDepth(n->right);
  if (n->left != NULL)
    leftHeight = getDepth(n->left);

  return MAX(rightHeight, leftHeight) + 1;
}
bool checkBalanceOfTree(AVLTree_node *current) {

  bool balancedRight = true, balancedLeft = true;
  int leftHeight = 0, rightHeight = 0;

  if (current->right != NULL) {
    balancedRight = checkBalanceOfTree(current->right);
    rightHeight = getDepth(current->right);
  }

  if (current->left != NULL) {
    balancedLeft = checkBalanceOfTree(current->left);
    leftHeight = getDepth(current->left);
  }

  return balancedLeft && balancedRight && abs(leftHeight - rightHeight) < 2;
}

static bool checkOrderingOfTree(AVLTree_node *current) {
  if (current->left != NULL) {
    if (comparIntptr(current->left->data, current->data) > 0)
      return false;
    else
      return checkOrderingOfTree(current->left);
  } else if (current->right != NULL) {
    if (comparIntptr(current->right->data, current->data) < 0)
      return false;
    else
      return checkOrderingOfTree(current->right);
  } else if (current->left == NULL && current->right == NULL)
    return true;

  return true;
}
}

#include <gtest/gtest.h>

TEST(AVLTREE, INIT) {
  AVLTree *tree = NULL;
  tree = AVLTree_init(comparIntptr, fakeDestroy);
  ASSERT_NE(tree, nullptr);
  ASSERT_NE(tree->compar, nullptr);
  ASSERT_NE(tree->destroy, nullptr);
  ASSERT_EQ(tree->root, nullptr);
  ASSERT_EQ(tree->nb_elem, 0);

  AVLTree_set_destroy(tree, NULL);
  ASSERT_EQ(tree->destroy, nullptr);
  AVLTree_set_destroy(tree, fakeDestroy);
  ASSERT_NE(tree->destroy, nullptr);
  ASSERT_TRUE(tree->destroy == fakeDestroy);

  AVLTree_set_compar(tree, NULL);
  ASSERT_TRUE(tree->compar == NULL);
  AVLTree_set_compar(tree, comparIntptr);
  ASSERT_TRUE(tree->compar != NULL);
  ASSERT_TRUE(tree->compar == comparIntptr);

  AVLTree_free(tree);
}

TEST(AVLTREE, INSERT) {
  AVLTree *tree = NULL;
  ASSERT_EQ(AVLTree_insert(tree, (const void *)(intptr_t)q[0]),
            TREE_ERR_NOTREE);

  tree = AVLTree_init(NULL, NULL);
  ASSERT_NE(tree, nullptr);

  ASSERT_EQ(AVLTree_insert(tree, (const void *)(intptr_t)q[0]),
            TREE_ERR_NOCOMPAR);

  AVLTree_set_compar(tree, comparIntptr);
  ASSERT_TRUE(tree->compar != NULL);
  ASSERT_TRUE(tree->compar == comparIntptr);

  size_t t = sizeof(q) / sizeof(*q);
  for (size_t i = 0; i < t; i++)
    ASSERT_EQ(AVLTree_insert(tree, (const void *)(intptr_t)q[i]), TREE_SUCCESS);

  /* unique insert */
  for (size_t i = 0; i < t; i++)
    ASSERT_EQ(AVLTree_insert(tree, (const void *)(intptr_t)q[i]),
              TREE_ERR_DATA);

  AVLTree_free(tree);
}

TEST(AVLTREE, BST_RULES) {
  AVLTree *tree = NULL;
  void *d;
  ASSERT_EQ(BST_findMin(tree, &d), 0);
  ASSERT_EQ(BST_findMax(tree, &d), 0);

  tree = AVLTree_init(NULL, NULL);
  ASSERT_NE(tree, nullptr);
  ASSERT_EQ(BST_findMin(tree, &d), -2);
  ASSERT_EQ(BST_findMax(tree, &d), -2);

  AVLTree_set_compar(tree, comparIntptr);
  size_t t = sizeof(q) / sizeof(*q);
  for (size_t i = 0; i < t; i++)
    ASSERT_EQ(AVLTree_insert(tree, (const void *)(intptr_t)q[i]), TREE_SUCCESS);

  ASSERT_EQ(BST_findMin(tree, &d), 1);
  ASSERT_EQ((intptr_t)d, 2);

  ASSERT_EQ(BST_findMax(tree, &d), 1);
  ASSERT_EQ((intptr_t)d, 30);

  ASSERT_TRUE(checkOrderingOfTree(BiTree_root(tree)));
  ASSERT_TRUE(checkBalanceOfTree(BiTree_root(tree)));

  AVLTree_free(tree);
}

TEST(AVLTREE, ROTATIONS) {
  AVLTree *tree = NULL;
  tree = AVLTree_init(comparIntptr, NULL);
  ASSERT_NE(tree, nullptr);

  AVLTree_insert(tree, (const void *)3);
  AVLTree_insert(tree, (const void *)4);
  AVLTree_insert(tree, (const void *)5);

  AVLTree_node *root = BiTree_root(tree);
  ASSERT_EQ((intptr_t)BiTree_data(root), 4);
  ASSERT_EQ((intptr_t)BiTree_data(BiTree_left(root)), 3);
  ASSERT_EQ((intptr_t)BiTree_data(BiTree_right(root)), 5);
  ASSERT_EQ(BiTree_parent(BiTree_left(root)), root);
  ASSERT_EQ(BiTree_parent(BiTree_right(root)), root);

  AVLTree_free(tree);

  tree = AVLTree_init(comparIntptr, NULL);
  ASSERT_NE(tree, nullptr);
  AVLTree_insert(tree, (const void *)5);
  AVLTree_insert(tree, (const void *)4);
  AVLTree_insert(tree, (const void *)3);

  root = BiTree_root(tree);
  ASSERT_EQ((intptr_t)BiTree_data(root), 4);
  ASSERT_EQ((intptr_t)BiTree_data(BiTree_left(root)), 3);
  ASSERT_EQ((intptr_t)BiTree_data(BiTree_right(root)), 5);
  ASSERT_EQ(BiTree_parent(BiTree_left(root)), root);
  ASSERT_EQ(BiTree_parent(BiTree_right(root)), root);

  AVLTree_free(tree);

  tree = AVLTree_init(comparIntptr, NULL);
  ASSERT_NE(tree, nullptr);
  AVLTree_insert(tree, (const void *)5);
  AVLTree_insert(tree, (const void *)3);
  AVLTree_insert(tree, (const void *)4);

  root = BiTree_root(tree);
  ASSERT_EQ((intptr_t)BiTree_data(root), 4);
  ASSERT_EQ((intptr_t)BiTree_data(BiTree_left(root)), 3);
  ASSERT_EQ((intptr_t)BiTree_data(BiTree_right(root)), 5);
  ASSERT_EQ(BiTree_parent(BiTree_left(root)), root);
  ASSERT_EQ(BiTree_parent(BiTree_right(root)), root);

  AVLTree_free(tree);

  tree = AVLTree_init(comparIntptr, NULL);
  ASSERT_NE(tree, nullptr);
  AVLTree_insert(tree, (const void *)3);
  AVLTree_insert(tree, (const void *)5);
  AVLTree_insert(tree, (const void *)4);

  root = BiTree_root(tree);
  ASSERT_EQ((intptr_t)BiTree_data(root), 4);
  ASSERT_EQ((intptr_t)BiTree_data(BiTree_left(root)), 3);
  ASSERT_EQ((intptr_t)BiTree_data(BiTree_right(root)), 5);
  ASSERT_EQ(BiTree_parent(BiTree_left(root)), root);
  ASSERT_EQ(BiTree_parent(BiTree_right(root)), root);

  AVLTree_free(tree);

  tree = AVLTree_init(comparIntptr, NULL);
  ASSERT_NE(tree, nullptr);
  size_t t = sizeof(q) / sizeof(*q);
  for (size_t i = 0; i < t; i++)
    ASSERT_EQ(AVLTree_insert(tree, (const void *)(intptr_t)q[i]), TREE_SUCCESS);
  ASSERT_EQ(AVLTree_insert(tree, (const void *)(intptr_t)15), TREE_SUCCESS);

  ASSERT_TRUE(checkOrderingOfTree(BiTree_root(tree)));
  ASSERT_TRUE(checkBalanceOfTree(BiTree_root(tree)));

  AVLTree_free(tree);
}

TEST(AVLTREE, REMOVE) {
  AVLTree *tree = NULL;
  tree = AVLTree_init(comparIntptr, NULL);
  ASSERT_NE(tree, nullptr);
  size_t t = sizeof(q) / sizeof(*q);
  for (size_t i = 0; i < t; i++)
    ASSERT_EQ(AVLTree_insert(tree, (const void *)(intptr_t)q[i]), TREE_SUCCESS);
  ASSERT_EQ(AVLTree_insert(tree, (const void *)(intptr_t)15), TREE_SUCCESS);

  ASSERT_TRUE(checkOrderingOfTree(BiTree_root(tree)));
  ASSERT_TRUE(checkBalanceOfTree(BiTree_root(tree)));

  for (size_t i = 0; i < t; i++) {
    printf("%d\n", q[i]);
    ASSERT_EQ(AVLTree_remove(tree, (const void *)(intptr_t)q[i]), TREE_SUCCESS);
    ASSERT_TRUE(checkOrderingOfTree(BiTree_root(tree)));
    ASSERT_TRUE(checkBalanceOfTree(BiTree_root(tree)));
  }
  AVLTree_free(tree);
}

TEST(AVLTREE, SEARCH) {
  AVLTree *tree = NULL;
  tree = AVLTree_init(comparIntptr, NULL);
  ASSERT_NE(tree, nullptr);
  size_t t = sizeof(q) / sizeof(*q);
  for (size_t i = 0; i < t; i++)
    ASSERT_EQ(AVLTree_insert(tree, (const void *)(intptr_t)q[i]), TREE_SUCCESS);
  ASSERT_EQ(AVLTree_insert(tree, (const void *)(intptr_t)15), TREE_SUCCESS);

  AVLTree_node **node = NULL;
  ASSERT_EQ(AVLTree_search(tree, (const void *)(intptr_t)42, node), TREE_ERR);
  ASSERT_EQ(AVLTree_search(tree, (const void *)(intptr_t)q[4], node),
            TREE_SUCCESS);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
