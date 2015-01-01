#include "BiTree.h"
#include "Queue.h"
#include "Utils.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static void BiTreeNode_preorder(BiTree_node *node,
                                void (*callback)(const void *));

BiTree *BiTree_new(void) {
  BiTree *tree = NULL;
  if ((tree = malloc(sizeof(BiTree))) != NULL) {
    tree->nb_elem = 0;
    tree->destroy = NULL;
    tree->compar = NULL;
    tree->root = NULL;
  }

  return tree;
}

BiTree *BiTree_new_init(destroy_fn_t destroy, compar_fn_t compar) {
  BiTree *tree = BiTree_new();
  if (tree) {
    BiTree_set_destroy(tree, destroy);
    BiTree_set_compar(tree, compar);
  }

  return tree;
}

void BiTree_set_destroy(BiTree *bitree, destroy_fn_t destroy) {
  if (bitree) {
    bitree->destroy = destroy;
  }
}

void BiTree_set_compar(BiTree *bitree, compar_fn_t compar) {
  if (bitree) {
    bitree->compar = compar;
  }
}

int BiTree_add_left(BiTree *bitree, BiTree_node *node, const void *data) {
  BiTree_node *new_node = NULL;
  BiTree_node **pos;

  if (bitree == NULL) {
    return TREE_ERR_NOTREE;
  }

  new_node = malloc(sizeof(BiTree_node));
  if (new_node == NULL) {
    return TREE_ERR_MEMORY;
  }

  if (node == NULL) {
    if (BiTree_size(bitree) > 0) {
      free(new_node);
      return TREE_ERR_NOTEROOT;
    }
    pos = &bitree->root;
    new_node->level = 1;
    new_node->parent = NULL;
  } else {
    if (BiTree_left(node) != NULL) {
      free(new_node);
      return TREE_ERR_NOTLEAF;
    }
    pos = &node->left;
    new_node->level = node->level + 1;
    new_node->parent = node;
  }

  new_node->data = DECONST(void *, data);
  new_node->balance = 0;
  new_node->left = NULL;
  new_node->right = NULL;
  *pos = new_node;

  bitree->nb_elem++;

  return TREE_SUCCESS;
}

int BiTree_add_right(BiTree *bitree, BiTree_node *node, const void *data) {
  BiTree_node *new_node = NULL;
  BiTree_node **pos;

  if (bitree == NULL) {
    return TREE_ERR_NOTREE;
  }

  new_node = malloc(sizeof(BiTree_node));
  if (new_node == NULL) {
    return TREE_ERR_MEMORY;
  }

  if (node == NULL) {
    if (BiTree_size(bitree) > 0) {
      free(new_node);
      return TREE_ERR_NOTEROOT;
    }
    pos = &bitree->root;
    new_node->level = 1;
    new_node->parent = NULL;
  } else {
    if (BiTree_right(node) != NULL) {
      free(new_node);
      return TREE_ERR_NOTLEAF;
    }
    pos = &node->right;
    new_node->level = node->level + 1;
    new_node->parent = node;
  }

  new_node->data = DECONST(void *, data);
  new_node->balance = 0;
  new_node->left = NULL;
  new_node->right = NULL;
  *pos = new_node;

  bitree->nb_elem++;

  return TREE_SUCCESS;
}

BiTree *BiTree_merge(BiTree *left_tree, BiTree *right_tree, const void *data) {
  BiTree *merge_tree;
  BiTree_node *node;

  /* Initialize the merged tree */
  if ((merge_tree = BiTree_new_init(left_tree->destroy, left_tree->compar)) ==
      NULL) {
    return NULL;
  }

  /* Insert the data for the root node of the merged tree */
  if (BiTree_add_left(merge_tree, NULL, data) != TREE_ERR_NOTEROOT) {
    BiTree_free(merge_tree);
    return NULL;
  }

  /* Merge the two binary trees into a single binary tree */
  node = BiTree_root(merge_tree);
  node->left = BiTree_root(left_tree);
  node->right = BiTree_root(right_tree);

  /* Adjust the size of the new binary tree */
  merge_tree->nb_elem = BiTree_size(merge_tree) + BiTree_size(left_tree) +
                        BiTree_size(right_tree);

  /* Do not let the original trees access the merged nodes */
  left_tree->root = NULL;
  left_tree->nb_elem = 0;
  right_tree->root = NULL;
  right_tree->nb_elem = 0;

  return merge_tree;
}

void BiTree_remove_left(BiTree *bitree, BiTree_node *node) {
  BiTree_node **pos;

  if (!bitree || (bitree->nb_elem == 0)) {
    return;
  }

  if (node == NULL) {
    pos = &bitree->root;
  } else {
    pos = &node->left;
  }

  if (*pos != NULL) {
    BiTree_remove_left(bitree, *pos);
    BiTree_remove_right(bitree, *pos);

    if (bitree->destroy) {
      bitree->destroy((*pos)->data);
    }
  }
  free(*pos);
  *pos = NULL;

  bitree->nb_elem--;
}

void BiTree_remove_right(BiTree *bitree, BiTree_node *node) {
  BiTree_node **pos;

  if (!bitree || (bitree->nb_elem == 0)) {
    return;
  }

  if (node == NULL) {
    pos = &bitree->root;
  } else {
    pos = &node->right;
  }

  if (*pos != NULL) {
    BiTree_remove_left(bitree, *pos);
    BiTree_remove_right(bitree, *pos);

    if (bitree->destroy) {
      bitree->destroy((*pos)->data);
    }
  }
  free(*pos);
  *pos = NULL;

  bitree->nb_elem--;
}

void BiTree_free(BiTree *bitree) {
  BiTree_remove_left(bitree, NULL);
  free(bitree);
}

/* Tree traversal */
static void BiTreeNode_preorder(BiTree_node *node,
                                void (*callback)(const void *)) {
  if (node == NULL) {
    return;
  }
  callback(BiTree_data(node));
  BiTreeNode_preorder(BiTree_left(node), callback);
  BiTreeNode_preorder(BiTree_right(node), callback);
}
static void BiTreeNode_inorder(BiTree_node *node,
                               void (*callback)(const void *)) {
  if (node == NULL) {
    return;
  }
  BiTreeNode_inorder(BiTree_left(node), callback);
  callback(BiTree_data(node));
  BiTreeNode_inorder(BiTree_right(node), callback);
}
static void BiTreeNode_postorder(BiTree_node *node,
                                 void (*callback)(const void *)) {
  if (node == NULL) {
    return;
  }
  BiTreeNode_postorder(BiTree_left(node), callback);
  BiTreeNode_postorder(BiTree_right(node), callback);
  callback(BiTree_data(node));
}

void BiTree_preorder(BiTree *bitree, void (*callback)(const void *)) {
  BiTreeNode_preorder(BiTree_root(bitree), callback);
}

void BiTree_inorder(BiTree *bitree, void (*callback)(const void *)) {
  BiTreeNode_inorder(BiTree_root(bitree), callback);
}

void BiTree_postorder(BiTree *bitree, void (*callback)(const void *)) {
  BiTreeNode_postorder(BiTree_root(bitree), callback);
}

void BiTree_levelorder(BiTree *bitree, void (*callback)(const void *)) {
  Queue *q = NULL;
  q = Queue_new(bitree->destroy);
  if (q == NULL) {
    return;
  }

  Queue_push(q, BiTree_root(bitree));
  while (!Queue_isEmpty(q)) {
    BiTree_node *node = NULL;
    Queue_pop(q, (void *)&node);
    callback(BiTree_data(node));
    if (BiTree_left(node) != NULL) {
      Queue_push(q, BiTree_left(node));
    }
    if (BiTree_right(node) != NULL) {
      Queue_push(q, BiTree_right(node));
    }
  }
  Queue_destroy(q);
}
