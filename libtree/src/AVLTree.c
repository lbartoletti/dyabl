#include "AVLTree.h"
#include "Queue.h"
static inline int max(int a, int b) { return (a > b) ? a : b; }

static inline int height(AVLTree_node *node) {
  if (BiTree_isEob(node)) {
    return -1;
  }

  if (BiTree_isLeaf(node)) {
    return 0;
  }
  if (BiTree_left(node) == NULL) {
    return 1 + height(BiTree_right(node));
  }
  if (BiTree_right(node) == NULL) {
    return 1 + height(BiTree_left(node));
  }
  return 1 + max(height(BiTree_left(node)), height(BiTree_right(node)));
}
static inline void setBalance(AVLTree_node *node) {
  BiTree_balance(node) = height(BiTree_right(node)) - height(BiTree_left(node));
}

static inline void setLevel(AVLTree *bitree) {
  Queue *q = NULL;
  q = Queue_new(bitree->destroy);
  if (q == NULL) {
    return;
  }

  Queue_push(q, BiTree_root(bitree));
  while (!Queue_isEmpty(q)) {
    BiTree_node *node;
    Queue_pop(q, (void *)&node);
    if (BiTree_parent(node) == NULL) {
      BiTree_level(node) = 1;
    } else {
      BiTree_level(node) = 1 + BiTree_level(BiTree_parent(node));
    }
    if (BiTree_left(node) != NULL) {
      Queue_push(q, BiTree_left(node));
    }
    if (BiTree_right(node) != NULL) {
      Queue_push(q, BiTree_right(node));
    }
  }
  Queue_destroy(q);
}

static inline AVLTree_node *left_rotation(AVLTree_node *node) {
  /* Right Right case */
  /** 3 (node)
   *   \
   *    4 (nr)
   *    \
   *      5 (leaf)
   */
  BiTree_node *nr = BiTree_right(node);
  BiTree_parent(nr) = BiTree_parent(node);

  BiTree_right(node) = BiTree_left(nr);
  if (BiTree_right(node) != NULL) {
    BiTree_parent(BiTree_right(node)) = node;
  }

  BiTree_left(nr) = node;
  BiTree_parent(node) = nr;

  if (BiTree_parent(nr) != NULL) {
    if (BiTree_right(BiTree_parent(nr)) == node) {
      BiTree_right(BiTree_parent(nr)) = nr;
    } else if (BiTree_left(BiTree_parent(nr)) == node) {
      BiTree_left(BiTree_parent(nr)) = nr;
    }
  }

  setBalance(node);
  setBalance(nr);

  return nr;
}

static inline AVLTree_node *right_rotation(AVLTree_node *node) {
  /* Left left case */
  /**     5 (grand parent)
   *      /
   *    4 (parent)
   *    /
   *   3 (node)
   */
  BiTree_node *nr = BiTree_left(node);
  BiTree_parent(nr) = BiTree_parent(node);

  BiTree_left(node) = BiTree_right(nr);
  if (BiTree_left(node) != NULL) {
    BiTree_parent(BiTree_left(node)) = node;
  }

  BiTree_right(nr) = node;
  BiTree_parent(node) = nr;

  if (BiTree_parent(nr) != NULL) {
    if (BiTree_right(BiTree_parent(nr)) == node) {
      BiTree_right(BiTree_parent(nr)) = nr;
    } else if (BiTree_left(BiTree_parent(nr)) == node) {
      BiTree_left(BiTree_parent(nr)) = nr;
    }
  }

  setBalance(node);
  setBalance(nr);

  return nr;
}

static inline AVLTree_node *right_left_rotation(AVLTree_node *node) {
  /* Right Left case */
  /** 3 (grand parent)
   *   \
   *    5 (parent)
   *    /
   *   4 (node)
   */

  BiTree_right(node) = right_rotation(BiTree_right(node));
  return left_rotation(node);
}

static inline AVLTree_node *left_right_rotation(AVLTree_node *node) {
  /* Left right case */
  /**     5 (grand parent)
   *     /
   *    3 (parent)
   *      \
   *       4 (node)
   */

  BiTree_left(node) = left_rotation(BiTree_left(node));
  return right_rotation(node);
}

AVLTree *AVLTree_init(compar_fn_t compar, destroy_fn_t destroy) {
  AVLTree *tree = NULL;
  if ((tree = malloc(sizeof(AVLTree))) != NULL) {
    tree->nb_elem = 0;
    tree->root = NULL;
    AVLTree_set_compar(tree, compar);
    AVLTree_set_destroy(tree, destroy);
  }

  return tree;
}

static void rebalance(AVLTree *tree, AVLTree_node *node) {
  if (!node) {
    return;
  }
  setBalance(node);
  int balance = BiTree_balance(node);
  if (balance == -2) {
    if (height(BiTree_left(BiTree_left(node))) >=
        height(BiTree_right(BiTree_left(node)))) {
      right_rotation(node);
    } else {
      left_right_rotation(node);
    }
  } else if (balance == 2) {
    if (height(BiTree_right(BiTree_right(node))) >=
        height(BiTree_left(BiTree_right((node))))) {
      left_rotation(node);
    } else {
      right_left_rotation(node);
    }
  }
  if (BiTree_parent(node) != NULL) {
    rebalance(tree, BiTree_parent(node));
  } else {
    tree->root = node;
    setLevel(tree);
  }
}

int AVLTree_insert(AVLTree *tree, const void *data) {
  if (!tree) {
    return TREE_ERR_NOTREE;
  }
  if (!tree->compar) {
    return TREE_ERR_NOCOMPAR;
  }

  BiTree_node *node = NULL;
  BiTree_node *prev = NULL;
  int direction;
  int cmpval;

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

  if (direction == 0) {
    return BiTree_add_left(tree, NULL, data);
  }

  if (direction == 1) {
    int ret = BiTree_add_left(tree, prev, data);
    if (ret != TREE_SUCCESS) {
      return ret;
    }
    node = BiTree_left(prev);
  }

  if (direction == 2) {
    int ret = BiTree_add_right(tree, prev, data);
    if (ret != TREE_SUCCESS) {
      return ret;
    }
    node = BiTree_right(prev);
  }
  rebalance(tree, node);
  return TREE_SUCCESS;
}

void AVLTree_free(AVLTree *tree) { BiTree_free(tree); }

int AVLTree_remove(AVLTree *tree, const void *data) {
  AVLTree_node *node;
  AVLTree_node *temp = NULL;
  int ret = BST_search(tree, data, &node);
  if (ret != TREE_SUCCESS) {
    return ret;
  }

  if (BiTree_isLeaf(node)) {
    if (BiTree_isEob(BiTree_parent(node))) {
      if (tree->destroy) {
        tree->destroy(node->data);
      }
      free(node);
      node = NULL;
      tree->root = NULL;
    }
    if (node) {
      temp = BiTree_parent(node);
      if (BiTree_left(temp) == node) {
        BiTree_remove_left(tree, temp);
      } else {
        BiTree_remove_right(tree, temp);
      }
    }
  }

  else if (BiTree_isEob(BiTree_right(node)) ||
           BiTree_isEob(BiTree_left(node))) {
    AVLTree_node *p = BiTree_parent(node);
    temp = BiTree_left(node) ? BiTree_left(node) : BiTree_right(node);
    if (p == NULL) {
      tree->root = temp;
      BiTree_parent(temp) = p;
    } else {
      if (BiTree_left(p) == node) {
        BiTree_left(p) = temp;
      } else {
        BiTree_right(p) = temp;
      }
      BiTree_parent(temp) = p;
    }
    if (tree->destroy) {
      tree->destroy(node->data);
    }
    free(node);
    node = NULL;
  } else {
    temp = BST_inOrderSuccessor(node);
    if (BiTree_parent(temp)) {
      if (BiTree_left(BiTree_parent(temp)) == temp) {
        BiTree_left(BiTree_parent(temp)) = BiTree_right(temp);
      } else {
        BiTree_right(BiTree_parent(temp)) = BiTree_right(temp);
      }
    }
    if (BiTree_right(temp)) {
      BiTree_parent(BiTree_right(temp)) = BiTree_parent(temp);
    }

    if (tree->destroy) {
      tree->destroy(node->data);
    }
    node->data = temp->data;
    free(temp);

    temp = node;
  }

  rebalance(tree, temp);
  return TREE_SUCCESS;
}

int AVLTree_search(AVLTree *tree, const void *data, AVLTree_node **node) {
  return BST_search(tree, data, node);
}

void AVLTree_set_destroy(AVLTree *tree, destroy_fn_t destroy) {
  if (tree) {
    tree->destroy = destroy;
  }
}

void AVLTree_set_compar(AVLTree *tree, compar_fn_t compar) {
  if (tree) {
    tree->compar = compar;
  }
}
