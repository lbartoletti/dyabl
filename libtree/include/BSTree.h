/**
 *
 * @file BiTree.h
 *
 * @brief Binary search tree
 *
 * @version 0.1
 * @date  29.05.2016 00:20:22
 * @author Loïc BARTOLETTI, l.bartoletti@free.fr
 *
 */

#ifndef DYABL_BINARY_SEARCH_TREE_H__
#define DYABL_BINARY_SEARCH_TREE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "BiTree.h"

/**
 * @brief Search the node containing data into the tree. Node can be get at
 * third argument
 *
 * @param[in] bitree The binary tree were search data
 * @param[in] data Data to search into the tree
 * @param[out] node Node were the data was found. Return NULL if node wasn't
 * found. Can pass NULL to don't get the node.
 *
 * @return TREE_ERR_NOTREE if tree doesn't exists ; TREE_ERR_NOCOMPAR if compar
 * is not set ; TREE_ERR if not found ; TREE_SUCCESS if found;
 */
int BST_search(BiTree *bitree, const void *data, BiTree_node **node);

/**
 * @brief Add a new element using search according to the rules of binary search
 * tree.
 *
 * @param[in] bitree The binary tree to add a new node with data.
 * @param[in] data Data to insert into the node.
 *
 * @return TREE_ERR_NOTREE if tree doesn't exists ; TREE_ERR_NOCOMPAR if compar
 * is not set ; TREE_ERR at error ; TREE_SUCCESS if ok ; TREE_ERR_DATA if data
 * already exists ; From insert: TREE_ERR_NOTEROOT if node is not empty root ;
 * TREE_ERR_NOTLEAF if node is not a leaf ; TREE_ERR_MEMORY at memory error ;
 * TREE_ERR else.
 */
int BST_insert(BiTree *bitree, const void *data);

/**
 * @brief Find the in order successor of a node
 *
 * @param[in] node The node where start search.
 *
 * @return the node found or NULL.
 */

BiTree_node *BST_inOrderSuccessor(BiTree_node *node);

/**
 * @brief Find the node containing the minus value from a leaf
 *
 * @param[in] node The node where start search.
 *
 * @return the node found or the node entered.
 */
BiTree_node *BST_minValueNode(BiTree_node *node);

/**
 * @brief Find the node containing the maximum value from a leaf
 *
 * @param[in] node The node where start search.
 *
 * @return the node found or the node entered.
 */
BiTree_node *BST_maxValueNode(BiTree_node *node);

/**
 * @brief Find the minus value into a BST
 *
 * @param[in] bitree The binary tree where find the minus value.
 * @param[out] data Pointer to the data of the minus value found.
 *
 * @return -2 if compar is not set ; -1 at error ; 1 if ok ; 0 if binary tree
 * doesn't exist.
 */
int BST_findMin(BiTree *bitree, void **data);

/**
 * @brief Find the maxmium value into a BST
 *
 * @param[in] bitree The binary tree where find the minus value.
 * @param[out] data Pointer to the data of the minus value found.
 *
 * @return -2 if compar is not set ; -1 at error ; 1 if ok ; 0 if binary tree
 * doesn't exist.
 */
int BST_findMax(BiTree *bitree, void **data);

#ifdef __cplusplus
}
#endif

#endif
