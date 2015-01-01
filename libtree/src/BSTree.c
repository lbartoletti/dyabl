#include "BSTree.h"

int BST_search(BiTree *tree, const void *data, BiTree_node **node) {
  if (!tree) {
    return TREE_ERR_NOTREE;
  }

  if (node != NULL) {
    *node = NULL;
  }
  if (tree->compar == NULL) {
    return TREE_ERR;
  }

  BiTree_node *cur = BiTree_root(tree);
  int cmpval;
  while (!BiTree_isEob(cur)) {
    cmpval = tree->compar(data, BiTree_data(cur));
    if (cmpval == 0) {
      if (node != NULL) {
        *node = cur;
      }
      return TREE_SUCCESS;
    }
    if (cmpval < 0) {
      cur = BiTree_left(cur);
    } else {
      cur = BiTree_right(cur);
    }
  }

  return TREE_ERR;
}

int BST_insert(BiTree *tree, const void *data) {
  if (!tree) {
    return TREE_ERR_NOTREE;
  }

  BiTree_node *node = NULL;
  BiTree_node *prev = NULL;
  int direction;
  int cmpval;

  if (!tree->compar) {
    return TREE_ERR_NOCOMPAR;
  }

  node = BiTree_root(tree);
  direction = 0;

  while (!BiTree_isEob(node)) {
    prev = node;
    cmpval = tree->compar(data, BiTree_data(node));
    if (cmpval == 0) {
      return TREE_ERR_DATA;
    }
    if (cmpval < 0) {
      node = BiTree_left(node);
      direction = 1;
    } else {
      node = BiTree_right(node);
      direction = 2;
    }
  }

  switch (direction) {
  case 0:
    return BiTree_add_left(tree, NULL, data);
  case 1:
    return BiTree_add_left(tree, prev, data);
  case 2:
    return BiTree_add_right(tree, prev, data);
  default: // unreachable
    return TREE_ERR;
  }

  return TREE_ERR; // unreachable
}

BiTree_node *BST_inOrderSuccessor(BiTree_node *node) {
  if (!BiTree_isEob(BiTree_right(node))) {
    return BST_minValueNode(BiTree_right(node));
  }

  BiTree_node *p = BiTree_parent(node);
  while (!BiTree_isEob(p) && (node == BiTree_right(p))) {
    node = p;
    p = BiTree_parent(p);
  }
  return p;
}

BiTree_node *BST_minValueNode(BiTree_node *node) {
  BiTree_node *cur = node;

  while (!BiTree_isEob(BiTree_left(cur))) {
    cur = BiTree_left(cur);
  }

  return cur;
}

BiTree_node *BST_maxValueNode(BiTree_node *node) {
  BiTree_node *cur = node;

  while (!BiTree_isEob(BiTree_right(cur))) {
    cur = BiTree_right(cur);
  }

  return cur;
}

int BST_findMin(BiTree *bitree, void **data) {
  if (!bitree) {
    return 0;
  }
  if (bitree->compar == NULL) {
    return -2;
  }

  BiTree_node *node = BST_minValueNode(BiTree_root(bitree));
  if (node) {
    *data = BiTree_data(node);
    return 1;
  }

  return -1;
}

int BST_findMax(BiTree *bitree, void **data) {
  if (!bitree) {
    return 0;
  }
  if (bitree->compar == NULL) {
    return -2;
  }

  BiTree_node *node = BST_maxValueNode(BiTree_root(bitree));
  if (node) {
    *data = BiTree_data(node);
    return 1;
  }

  return -1;
}
