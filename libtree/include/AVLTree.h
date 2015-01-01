/**
 *
 * @file AVLTree.h
 *
 * @brief A.V.L. tree
 *
 * @version 0.1
 * @date  09.04.2016 12:50:23
 * @author Loïc BARTOLETTI, lbartoletti@tuxfamily.org
 *
 */

#ifndef DYABL_AVL_TREE_H__
#define DYABL_AVL_TREE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "BSTree.h"
typedef int (*compar_fn_t)(const void *, const void *);
typedef void (*destroy_fn_t)(void *);

typedef BiTree_node AVLTree_node;
typedef BiTree AVLTree;

/**
 * @brief Create an AVL tree and initialize functions (delete and compar).
 *
 * @param[in] destroy The function to destroy data
 * @param[in] compar The comparison function
 *
 * @return A new AVL tree, else NULL.
 */
AVLTree *AVLTree_init(compar_fn_t compar, destroy_fn_t destroy);

/***************************************************************************************/
/* AVLTree free and delete binary tree*/
/**
 * @brief Free all of the memory used by an AVLTree and delete it
 *
 * @param tree The binary tree to free'd
 */
void AVLTree_free(AVLTree *tree);

/**
 * @brief Insert a data \a data into the AVLTree \a tree
 *
 * @param[in] tree The AVLTree
 * @param[in] data Data to insert into the tree
 *
 * @return TREE_ERR_NOTREE if tree doesn't exists ; TREE_ERR_NOCOMPAR if compar
 * is not set ; -1 at error ; TREE_SUCCESS if ok ; TREE_ERR_DATA if data already
 * exists ; From insert: TREE_ERR_NOTEROOT if node is not empty root ;
 * TREE_ERR_NOTLEAF if node is not a leaf ; TREE_ERR_MEMORY at memory error ;
 * TREE_ERR else.
 */
int AVLTree_insert(AVLTree *tree, const void *data);

/**
 * @brief Remove the node containing data into the tree.
 *
 * @param[in] tree The AVL tree were search data
 * @param[in] data Data to search into the tree to remove
 *
 * @return TREE_ERR_NOTREE if tree doesn't exists ; TREE_ERR_NOCOMPAR if compar
 * is not set ; TREE_ERR if not found ; TREE_SUCCESS if found;
 */
int AVLTree_remove(AVLTree *tree, const void *data);

/**
 * @brief Search the node containing data into the tree. Node can be get at
 * third argument
 *
 * @param[in] tree The AVL tree were search data
 * @param[in] data Data to search into the tree
 * @param[out] node Node were the data was found. Return NULL if node wasn't
 * found. Can pass NULL to don't get the node.
 *
 * @return TREE_ERR_NOTREE if tree doesn't exists ; TREE_ERR_NOCOMPAR if compar
 * is not set ; TREE_ERR if not found ; TREE_SUCCESS if found;
 */
int AVLTree_search(AVLTree *tree, const void *data, AVLTree_node **node);

#define AVLTree_size(tree) ((tree)->nb_elem)

/**
 * @brief Set the destroy function.
 *
 * @param[in] tree The tree to set the destroy function.
 * @param[in] destroy The destroy function.
 */
void AVLTree_set_destroy(AVLTree *tree, destroy_fn_t destroy);

/**
 * @brief Set the comparison function.
 *
 * @param[in] tree The list to set the comparison function.
 * @param[in] compar The comparison function.
 */
void AVLTree_set_compar(AVLTree *tree, compar_fn_t compar);

#ifdef __cplusplus
}
#endif

#endif
