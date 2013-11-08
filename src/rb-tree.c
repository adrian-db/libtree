#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "rb-tree.h"

RBReturnCode createTree (void *value, RBTreeNode **root)
{
	*root = malloc (sizeof (RBTreeNode));
	
	if (*root == NULL) return RB_NOT_ENOUGH_MEMORY;
	
	**root = (RBTreeNode){.value = value, .colour = RB_BLACK, .left = NULL, .right = NULL};
	
	return RB_SUCCESS;
}

void freeTree (RBTreeNode *root)
{
	if (root == NULL) return;
	
	// Currently this has to be recursive, as we don't have a way of going back up the tree
	// from a leaf - so we need to remember where we have been.
	// TODO: make nodes point back to their parent, and make this iterative, so that we don't
	// run out of stack when freeing large trees.
	
	freeTree (root->left);
	freeTree (root->right);
	
	free (root);
	root = NULL;
}

RBReturnCode add (RBTreeNode *root, void *value, bool *createdNewNode, RBComparator comparator)
{
	// Can't add a node to a null tree, or a null node.
	assert (root != NULL);
	
	RBTreeNode *parent;
	RBTreeBranch branch;
	RBTreeNode **attachmentPoint;
	
	*createdNewNode = false;
	
	findInsertionPoint (root, value, comparator, &parent, &branch);
	
	// The value matches the root - no need to add the node.
	if (parent == NULL) return RB_SUCCESS;
	
	if (branch == RB_LEFT) {
		
		attachmentPoint = &(parent->left);
		
	}
	else if (branch == RB_RIGHT) {
		
		attachmentPoint = &(parent->right);
		
	}
	
	if (*attachmentPoint == NULL) {
		
		*attachmentPoint = malloc (sizeof (RBTreeNode));
		
		if (*attachmentPoint == NULL) {
			
			return RB_NOT_ENOUGH_MEMORY;
			
		}
		else {
			
			*createdNewNode = true;
			
		}
		
		**attachmentPoint = (RBTreeNode){.value = value, .colour = RB_RED, .left = NULL, .right = NULL};
		
	}
	
	return RB_SUCCESS;
	
}

void findInsertionPoint (RBTreeNode *root, void *value, RBComparator comparator, 
	RBTreeNode **parent, RBTreeBranch *branch)
{	
	*parent = NULL;
	
	RBTreeNode *currentNode = root;
	
	while (currentNode != NULL) {
		
		int comparison = comparator (value, currentNode->value);
		
		// Comparators should only return 0, 1, or -1 values, but for robustness I'm
		// treating 0, +ve, or -ve values as being allowed, and the equivalent of the above.
		if (comparison == 0) {
			
			// The current node has the given value. We can stop here.
			return;
			
		}
		else if (comparison > 0) {
			
			// The value is greater than the current node's value.
			// Look at any nodes attached via the right branch.
			*branch = RB_RIGHT;
			*parent = currentNode;
			currentNode = (*parent)->right;
			break;
			
		}
		else {
			
			// The value is less than the current node's value.
			// Look at any nodes attached via the left branch.
			*branch = RB_LEFT;
			*parent = currentNode;
			currentNode = (*parent)->left;
			break;
			
		}
		
	}
	
	// If we got this far, we travelled down the tree in appropriate steps and found an
	// empty spot where the value should go - we can return now, as we will have set the
	// parent and branch values appropriately.
	
}

RBTreeNode *rotate (RBTreeNode *root, RBTreeBranch branchToMoveUp)
{
	// Below, 'leading' is in the direction of rotation.
	// 'trailing' is opposite to the direction of rotation.
	RBTreeBranch trailing = branchToMoveUp;

	RBTreeBranch leading = opposite (trailing);
	
	RBTreeNode *incomingRoot = getChild (root, trailing);
	
	// The tricky bit of rotation comes here.
	// The old root slips into the leading position on the new root.
	// The branch that used to be in the leading position takes the old place vacated by the
	// new root.

	RBTreeNode *orphanNode = setChild (incomingRoot, root, leading);	
	
	setChild (root, orphanNode, trailing);

	return incomingRoot;
}

RBTreeNode *setChild (RBTreeNode *node, RBTreeNode *child, RBTreeBranch branch)
{
	if (node == NULL || child == NULL) return NULL;
	
	RBTreeNode *oldChild = NULL;
	
	if (branch == RB_LEFT) {
		
		oldChild = node->left;
		node->left = child;
		
	}
	else if (branch == RB_RIGHT) {
	
		oldChild = node->right;
		node->right = child;
		
	}
	
	return oldChild;
}

RBTreeNode *getChild (RBTreeNode *node, RBTreeBranch branch)
{
	if (node == NULL) return NULL;
	
	if (branch == RB_LEFT) return node->left;
	
	return node->right;
}

RBTreeBranch opposite (RBTreeBranch orientation)
{
	if (orientation == RB_LEFT) return RB_RIGHT;
	
	return RB_LEFT;
}