/**
 *
 * @file BiTree.h
 *
 * @brief Binary tree
 *
 * @version 0.1
 * @date  09.04.2016 12:50:23
 * @author Loïc BARTOLETTI, l.bartoletti@free.fr
 *
 */

#ifndef DYABL_BINARY_TREE_H__
#define DYABL_BINARY_TREE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "Utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef int (*compar_fn_t)(const void *, const void *);
typedef void (*destroy_fn_t)(void *);

typedef enum tree_config_error {
  TREE_ERR = -1,
  TREE_ERR_NOTREE = -2,
  TREE_ERR_NOCOMPAR = -3,
  TREE_ERR_NODESTROY = -4,
  TREE_ERR_MEMORY = -5,
  TREE_ERR_NOTLEAF = -6,
  TREE_ERR_NOTEROOT = -7, // Not Empty Root
  TREE_ERR_DATA = -10,
  TREE_SUCCESS = 1
} error_tree;

static errordesc tree_errors[] = {
    {TREE_ERR, "Error"},
    {TREE_ERR_NOTREE, "No tree"},
    {TREE_ERR_NOCOMPAR, "No compar function"},
    {TREE_ERR_NODESTROY, "No destroy function"},
    {TREE_ERR_MEMORY, "Cannot allocate memory"},
    {TREE_ERR_NOTLEAF, "Error with a leaf"},
    {TREE_ERR_NOTEROOT, "Not an empty root"},
    {TREE_ERR_DATA, "Error with data in the tree"},
    {TREE_SUCCESS, "No error"}};

/***************************************************************************************/
/* Binary node structure */
/**
 * bitree_node
 * @brief Node of a binary tree.
 *
 * bitree_node contain data and pointers to childs.
 */
typedef struct bitree_node {
  void *data;                 /*!< data of the element. */
  struct bitree_node *parent; /*!< pointer to the parent node */
  struct bitree_node *left;   /*!< pointer to the left child of the node. */
  struct bitree_node *right;  /*!< pointer to the right child of the node. */
  unsigned int level;         /*!< level of node into the tree */
  int balance;                /*!< Used for balanced binary */
} BiTree_node; /* ----------  end of struct bitree_node  ---------- */

typedef struct bitree {
  BiTree_node *root;    /*!< head node of the binary tree. */
  destroy_fn_t destroy; /*!< function to delete data element. */
  compar_fn_t compar;   /*!< function to compar elements. */
  long nb_elem;         /*!< number of nodes into the binary tree. */
} BiTree;

/***************************************************************************************/
/** BiTree initialization */
/**
 * @brief Create a binary tree.
 *
 * @return A new empty binary tree, else NULL.
 */
BiTree *BiTree_new(void);

/**
 * @brief Create a binary tree and initialize functions (delete and compar).
 *
 * @param[in] destroy The function to destroy data
 * @param[in] compar The comparison function
 *
 * @return A new binary tree, else NULL.
 */
BiTree *BiTree_new_init(destroy_fn_t destroy, compar_fn_t compar);

/**
 * @brief Set the destroy function.
 *
 * @param[in] bitree The tree to set the destroy function.
 * @param[in] destroy The destroy function.
 */
void BiTree_set_destroy(BiTree *bitree, destroy_fn_t destroy);

/**
 * @brief Set the comparison function.
 *
 * @param[in] bitree The list to set the comparison function.
 * @param[in] compar The comparison function.
 */
void BiTree_set_compar(BiTree *bitree, compar_fn_t compar);

/***************************************************************************************/
/* BiTree add nodes */
/**
 * @brief Add a new element on left node.
 *
 * @param[in] bitree The binary tree to add a left node with data.
 * @param[in] node The node were insert new data or NULL for root.
 * @param[in] data Data to insert into the node.
 *
 * @return TREE_ERR_NOTEROOT if node is not empty root ; TREE_ERR_NOTLEAF if
 * node is not a leaf ; TREE_ERR_MEMORY at memory error ; TREE_SUCCESS if ok ;
 * TREE_ERR_NOTREE if binary tree doesn't exist.
 */
int BiTree_add_left(BiTree *bitree, BiTree_node *node, const void *data);
/**
 * @brief Add a new element on right node.
 *
 * @param[in] bitree The binary tree to add a right node with data.
 * @param[in] node The node were insert new data or NULL for root.
 * @param[in] data Data to insert into the node.
 *
 * @return TREE_ERR_NOTEROOT if node is not empty root ; TREE_ERR_NOTLEAF if
 * node is not a leaf ; TREE_ERR_MEMORY at memory error ; TREE_SUCCESS if ok ;
 * TREE_ERR_NOTREE if binary tree doesn't exist.
 */
int BiTree_add_right(BiTree *bitree, BiTree_node *node, const void *data);

/**
 * @brief Merge two tree with data into merged root.
 *
 * @param[in] left_tree The binary which be at left
 * @param[in] right_tree The binary which be at right
 * @param[in] data Data to insert at root.
 *
 * @return merged tree or NULL
 */
BiTree *BiTree_merge(BiTree *left_tree, BiTree *right_tree, const void *data);

/***************************************************************************************/
/* BiTree remove nodes */
/**
 * @brief Remove recursively left node(s).
 *
 * @param[in] bitree The binary tree.
 * @param[in] node The node (and childs) to remove (NULL for destroy bitree).
 */
void BiTree_remove_left(BiTree *bitree, BiTree_node *node);
/**
 * @brief Remove recursively right node(s).
 *
 * @param[in] bitree The binary tree.
 * @param[in] node The node (and childs) to remove (NULL for destroy bitree).
 */
void BiTree_remove_right(BiTree *bitree, BiTree_node *node);

/***************************************************************************************/
/* BiTree free and delete binary tree*/
/**
 * @brief Free all of the memory used by a BiTree and delete it
 *
 * @param bitree The binary tree to free'd
 */
void BiTree_free(BiTree *bitree);

#define BiTree_size(bitree) ((bitree) ? (bitree)->nb_elem : 0)
#define BiTree_root(bitree) ((bitree) ? (bitree)->root : NULL)
#define BiTree_data(node) ((node) ? (node)->data : NULL)
#define BiTree_level(node) ((node)->level)
#define BiTree_parent(node) ((node)->parent)
#define BiTree_balance(node) ((node)->balance)
#define BiTree_left(node) ((node)->left)
#define BiTree_right(node) ((node)->right)
#define BiTree_isLeaf(node) (BiTree_left((node)) == NULL && BiTree_right((node)) == NULL)
#define BiTree_isEob(node) ((node) == NULL)

/***************************************************************************************/
/* BiTree traversal */
/* Depth-first search */
/**
 * @brief Pre-order traverse
 *
 * @param[in] bitree The binary tree to traverse.
 * @param[in] callback Reference to user-defined callback function.
 */
void BiTree_preorder(BiTree *bitree, void (*callback)(const void *));
/**
 * @brief In-order traverse
 *
 * @param[in] bitree The binary tree to traverse.
 * @param[in] callback Reference to user-defined callback function.
 */
void BiTree_inorder(BiTree *bitree, void (*callback)(const void *));
/**
 * @brief Post-order traverse
 *
 * @param[in] bitree The binary tree to traverse.
 * @param[in] callback Reference to user-defined callback function.
 */
void BiTree_postorder(BiTree *bitree, void (*callback)(const void *));
/* Breadth-first search */
/**
 * @brief Level-order traverse
 *
 * @param[in] bitree The binary tree to traverse.
 * @param[in] callback Reference to user-defined callback function.
 */
void BiTree_levelorder(BiTree *bitree, void (*callback)(const void *));

#ifdef __cplusplus
}
#endif

#endif
