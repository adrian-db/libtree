/*
 * rb-tree.h
 *
 * A self-balancing binary tree, as described in the red-black tree article on Wikipedia:
 * http://en.wikipedia.org/wiki/Red_black_tree
 *
 * 26/10/2013	Adrian Bigland
 */

#ifndef RB_TREE_H
#define RB_TREE_H

#include <stdbool.h>

// =======================================================================
// = Structures, enumerations and other constants.
// =======================================================================

// Constants to use when referring to the two branches of a node.
typedef enum rb_tree_branch {RB_LEFT, RB_RIGHT} RBTreeBranch;

// The colours of nodes.
typedef enum rb_tree_colour {RB_BLACK, RB_RED} RBTreeColour;

// Success/failure codes for methods that may fail.
typedef enum rb_return_code {
	RB_SUCCESS, 
	RB_NOT_ENOUGH_MEMORY // Couldn't create a new node as there wasn't enough spare memory.
} RBReturnCode;

// A node in the red-black tree.
typedef struct rb_tree_node {
	
	void *value;

	RBTreeColour colour:1;
	
	struct rb_tree_node *parent;
	struct rb_tree_node *left;
	struct rb_tree_node *right;
	
} RBTreeNode;

/* 
 * A function used to order the nodes by value.
 *
 * Comaprator functions should return 0 if the two values are equal, -1 if the left value is less
 * than the right, or 1 if the left value is greater than the right.
 */
typedef int (*RBComparator) (void *, void *);

// =======================================================================
// = Functions to create and free resources used in a tree.
// =======================================================================

/*
 * Creates the root node of a new tree. You MUST call freeTree later, when you no longer need the
 * tree, to recover the memory used for the tree nodes.
 *
 * rootValue: the value of the new root node.
 * root: A pointer to a pointer - used to return the root node of the newly created tree.
 *
 * returns: a success code if the new root node could be created - otherwise an error.
 */
RBReturnCode createTree(void *rootValue, RBTreeNode **root);

/*
 * Frees memory used for the nodes of the tree.
 *
 * root: the root node of a tree you no longer need. The function will free the memory used by
 * this node, and all those connected to it. This should be called for every tree you create using
 * the createTree method, once they are no longer needed.
 */
void freeTree(RBTreeNode *root);

// =======================================================================
// = Functions to add and remove values from a tree.
// =======================================================================

/*
 * Adds the value to the tree.
 * If the tree already contains the value, this function still returns RB_SUCCESS, but will set
 * the createdNewNode boolean value to 'false' - so you can use this method to ensure a value
 * is present, and test wether it was already there, in one operation.
 *
 * root: the root of a tree.
 * value: a value to add.
 * createdNewNode: a pointer to a bool used to report wether a new node was actually created.
 * comparator: a function to order values within the tree.
 *
 * returns: a code showing success, or failure - failure can be caused by not having enough
 * memory to create a new tree node to hold the value.
 */
RBReturnCode add (RBTreeNode *root, void *value, bool *createdNewNode, RBComparator comparator);

// =======================================================================
// = Functions to search a tree.
// =======================================================================

/*
 * Finds the place to insert a node of the given value.
 * If the selected parent has a non-NULL node on the branch, then a node with the given value
 * is already in the tree at that position.
 * If the value happens to match the root node, the parent is set to NULL and the branch is unset,
 * as it has no meaning in this special case - so always check for a NULL parent.
 *
 * root: the root of a tree.
 * value: the value to insert.
 * comparator: a function to order values within the tree.
 * parent: a pointer to a pointer used to return the parent of the new node for the value.
 * branch: a pointer to a branch value used to return the parent branch that the new node 
 *   would attach to.
 */
void findInsertionPoint (RBTreeNode *root, void *value, RBComparator comparator, 
	RBTreeNode **parent, RBTreeBranch *branch);

// =======================================================================
// = Support functions.
// =======================================================================

/*
 * Rotates a tree.
 *
 * Instead of specifying the direction of rotation, you specify which branch of the root will
 * be rotated up to be the new root.
 */
RBTreeNode *rotate (RBTreeNode *root, RBTreeBranch branchToMoveUp);

/*
 * Sets the child of a node.
 */
RBTreeNode *setChild (RBTreeNode *node, RBTreeNode *child, RBTreeBranch branch);

/*
 * Gets the child of a node.
 */
RBTreeNode *getChild (RBTreeNode *node, RBTreeBranch branch);

/*
 * Gets the opposite orientation, i.e. LEFT for RIGHT.
 */
RBTreeBranch opposite (RBTreeBranch orientation);

#endif