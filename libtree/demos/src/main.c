#include "AVLTree.h"
#include "BiTree.h"
#include "Queue.h"
#include "Utils.h"
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool str2int(const char *buff, int *num_int) {
  char *end_ptr;
  char *end = NULL;
  bool ret = false;

    errno = 0;

    const long sl = strtol(buff, &end_ptr, 10);

    if (end == buff) {
      fprintf(stderr, "%s: not a decimal number\n", buff);
    } else if ('\0' != *end) {
      fprintf(stderr, "%s: extra characters at end of input: %s\n", buff, end);
    } else if ((LONG_MIN == sl || LONG_MAX == sl) && ERANGE == errno) {
      fprintf(stderr, "%s out of range of type long\n", buff);
    } else if (sl > INT_MAX) {
      fprintf(stderr, "%ld greater than INT_MAX\n", sl);
    } else if (sl < INT_MIN) {
      fprintf(stderr, "%ld less than INT_MIN\n", sl);
    } else {
      *num_int = (int)sl;
      ret = true;
    }

  return ret;
}

static void printInt(const void *t) { printf("%ld ", (intptr_t)t); }

static void aff(BiTree_node *racine, unsigned long prof) {
  unsigned long i;
  for (i = 0; i < prof; i++) {
    fputs("|___ ", stdout);
  }

  if (racine) {
    printf("[%ld (%d) {%d}]\n", (intptr_t)racine->data, racine->level,
           racine->balance);

    if (racine->left)
      aff(racine->left, prof + 1);
    if (racine->right)
      aff(racine->right, prof + 1);
  } else
    printf("NULL\n");
}

static int bitree_fn(void) {
  BiTree *tree = NULL;
  tree = BiTree_new_init(NULL, comparInt);
  if (!tree)
    return -1;

  BiTree_node *node = NULL;
  BiTree_add_left(tree, NULL, (intptr_t *)0);
  node = BiTree_root(tree);
  BiTree_add_left(tree, node, (intptr_t *)1);
  BiTree_add_right(tree, node, (intptr_t *)2);

  BiTree_add_left(tree, BiTree_left(node), (intptr_t *)11);
  BiTree_add_right(tree, BiTree_left(node), (intptr_t *)12);

  BiTree_add_left(tree, BiTree_left(BiTree_left(node)), (intptr_t *)111);
  BiTree_add_right(tree, BiTree_left(BiTree_left(node)), (intptr_t *)112);

  BiTree_add_left(tree, BiTree_right(BiTree_left(node)), (intptr_t *)121);
  BiTree_add_right(tree, BiTree_right(BiTree_left(node)), (intptr_t *)122);

  BiTree_add_left(tree, BiTree_right(node), (intptr_t *)21);
  BiTree_add_right(tree, BiTree_right(node), (intptr_t *)22);

  BiTree_add_left(tree, BiTree_left(BiTree_right(node)), (intptr_t *)211);
  BiTree_add_right(tree, BiTree_left(BiTree_right(node)), (intptr_t *)212);

  BiTree_add_left(tree, BiTree_right(BiTree_right(node)), (intptr_t *)221);
  BiTree_add_right(tree, BiTree_right(BiTree_right(node)), (intptr_t *)222);

  printf("\npre: ");
  BiTree_preorder(tree, printInt);
  printf("\nin: ");
  BiTree_inorder(tree, printInt);
  printf("\npost: ");
  BiTree_postorder(tree, printInt);
  printf("\nlevel: ");
  BiTree_levelorder(tree, printInt);
  printf("\n-\n");

  BiTree_free(tree);

  return 1;
}

static int bst_fn(void) {
  BiTree *tree = NULL;
  tree = BiTree_new_init(NULL, comparInt);
  if (!tree)
    return -1;

  BST_insert(tree, (const void *)8);
  BST_insert(tree, (const void *)10);
  BST_insert(tree, (const void *)3);
  BST_insert(tree, (const void *)1);
  BST_insert(tree, (const void *)6);
  BST_insert(tree, (const void *)14);
  BST_insert(tree, (const void *)4);
  BST_insert(tree, (const void *)7);
  BST_insert(tree, (const void *)13);

  printf("\npre: ");
  BiTree_preorder(tree, printInt);
  printf("\nin: ");
  BiTree_inorder(tree, printInt);
  printf("\npost: ");
  BiTree_postorder(tree, printInt);
  printf("\nlevel: ");
  BiTree_levelorder(tree, printInt);
  printf("\n-\n");

  BiTree_node *n;
  printf("%d", BST_search(tree, (intptr_t *)14, &n));
  printf("Parent:");
  printInt(BiTree_parent(n)->data);
  printf("Gauche:");
  printInt(BiTree_left(n)->data);
  puts("");
  BiTree_free(tree);
  return 1;
}

static int avl_fn(void) {
  AVLTree *tree = NULL;
  tree = AVLTree_init(comparInt, NULL);
  if (!tree)
    return -1;

  int key = 0;
  char ch;

  do {
    do {
      puts("Insert (i) or delete (d):");
      if (scanf("%c", &ch) != 1) {
        printf("Bad input\n");
        BiTree_free(tree);
        return (EXIT_FAILURE);
      }
    } while (ch != 'i' && ch != 'd');
    puts("Insert a number (-1) to exit");
  char buff[25];
  if (fgets(buff, sizeof(buff), stdin) == NULL) {
    if (puts("EOF or read error\n") == EOF) {
      return -1;
    }
  }
    if (str2int(buff, &key) != true) {
      printf("Bad input\n");
      BiTree_free(tree);
      return (EXIT_FAILURE);
    }
    switch (ch) {
    case 'i':
      AVLTree_insert(tree, (const void *)(intptr_t)key);
      break;
    case 'd':
      AVLTree_remove(tree, (const void *)(intptr_t)key);
      break;
    default:
      break;
    }
    aff(BiTree_root(tree), 0);
    puts("");
  } while (key != -1);

  AVLTree_free(tree);
  return 1;
}

int main(void) {

  printf("Binary tree\n");
  bitree_fn();
  puts("");
  printf("Binary search tree\n");
  bst_fn();
  printf("AVL tree\n");
  avl_fn();

  return EXIT_SUCCESS;
}
