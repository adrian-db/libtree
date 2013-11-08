#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include "rb-tree.h"

// Characters for node values, and to make the nodes easy to tell apart.
char letters[7] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};

// Nodes for use in making trees.
RBTreeNode a;
RBTreeNode b;
RBTreeNode c;
RBTreeNode d;
RBTreeNode e;
RBTreeNode f;
RBTreeNode g;

RBTreeNode *createNode (void *value);

// TEST FIXTURES

void setup (void)
{
	a = &(RBTreeNode){.value = letters[0]};
	b = &(RBTreeNode){.value = letters[1]};
	c = &(RBTreeNode){.value = letters[2]};
	d = &(RBTreeNode){.value = letters[3]};
	e = &(RBTreeNode){.value = letters[4]};
	f = &(RBTreeNode){.value = letters[5]};
	g = &(RBTreeNode){.value = letters[6]};
}

void teardown (void)
{
	/* DO NOTHING */
}

// SETUP FUNCTIONS

void setupAlphabeticalOrdering (void)
{
	a.left = NULL; a.right = NULL; a.colour = RB_BLACK;
	
	b.left = &a; b.right = &c; b.colour = RB_RED;
	
	c.left = NULL; c.right = NULL; c.colour = RB_BLACK;
	
	// Root node, therefore it should be black.
	d.left = &b; d.right = &f; d.colour = RB_BLACK;
	
	e.left = NULL; e.right = NULL; e.colour = RB_BLACK;
	
	f.left = &e; f.right = &g; f.colour = RB_RED;
	
	g.left = NULL; g.right = NULL; g.colour = RB_BLACK;
}

// ASSERTION CONVENIENCE FUNCTIONS

void checkIsLeaf(RBTreeNode *node, char *msg)
{
	ck_assert_msg (node->left == NULL, msg);
	ck_assert_msg (node->right == NULL, msg);
}

// COMPARATORS

int compareChars (void *left, void *right)
{
	char* l = (char *)left;
	char* r = (char *)right;
	
	return *l - *r;
}

// Checks that rotating a tree to the right works as expected.
START_TEST (rotate_right)
{	
	setupAlphabeticalOrdering();
	
	// Rotate right by rotating the left branch up to the top.
	RBTreeNode *newRoot = rotate (&d, RB_LEFT);
	
	ck_assert_msg (newRoot == &b, "Didn't find the expected root node after rotation.");
	ck_assert_msg (b.right == &d, "Didn't rotate the root node into the expected position.");
	ck_assert_msg (b.left == &a, "The trailing node of the new root has been changed unexpectedly.");
	ck_assert_msg (d.left == &c, "Didn't replace the rotated node as expected.");
	ck_assert_msg (d.right == &f, "The leading node of the old root has been changed unexpectedly.");
	
	checkIsLeaf (&a, "Node A should have been unchanged by rotation, but has been.");
	checkIsLeaf (&c, "Node C should have been unchanged by rotation, but has been.");
	checkIsLeaf (&e, "Node E should have been unchanged by rotation, but has been.");
	checkIsLeaf (&g, "Node G should have been unchanged by rotation, but has been.");
	
	ck_assert_msg (f.left == &e, "Node F should have been unchanged by rotation, but has been.");
	ck_assert_msg (f.right == &g, "Node F should have been unchanged by rotation, but has been.");
}
END_TEST
	
	// Checks that rotating a tree to the left works as expected.
START_TEST (rotate_left)
{
	setupAlphabeticalOrdering();

	// Rotate left by rotating the right branch up to the top.
	RBTreeNode *newRoot = rotate (&d, RB_RIGHT);

	ck_assert_msg (newRoot == &f, "Didn't find the expected root node after rotation.");
	ck_assert_msg (f.left == &d, "Didn't rotate the root node into the expected position.");
	ck_assert_msg (f.right == &g, "The trailing node of the new root has been changed unexpectedly.");
	ck_assert_msg (d.right == &e, "Didn't replace the rotated node as expected.");
	ck_assert_msg (d.left == &b, "The leading node of the old root has been changed unexpectedly.");

	checkIsLeaf (&a, "Node A should have been unchanged by rotation, but has been.");
	checkIsLeaf (&c, "Node C should have been unchanged by rotation, but has been.");
	checkIsLeaf (&e, "Node E should have been unchanged by rotation, but has been.");
	checkIsLeaf (&g, "Node G should have been unchanged by rotation, but has been.");

	ck_assert_msg (b.left == &a, "Node B should have been unchanged by rotation, but has been.");
	ck_assert_msg (b.right == &c, "Node B should have been unchanged by rotation, but has been.");
}
END_TEST
	
	// Checks that lower valued nodes are added to a single root node on the left.
START_TEST (add_simple_lessthan)
	
	RBTreeNode *root;
	int result;
	bool wasAdded;

	result = createTree (b.value, &root);
	
	ck_assert_msg (result == RB_SUCCESS, "Couldn't create a tree to test.");
	
	result = add (root, a.value, &wasAdded, compareChars);
	
	ck_assert_msg (result == RB_SUCCESS, "Didn't report adding a node as expected.");
	ck_assert_msg (wasAdded == true, "Didn't report adding a node as expected.");
	checkNodeEquals (b.left, &(RBTreeNode){.value = a.value, .colour = RB_RED});
	ck_assert_msg (b.right == NULL, "Changed the wrong branch.");
	
	freeTree (root);
	
END_TEST

	// Checks that higher valued nodes are added to a single root node on the right.	
START_TEST (add_simple_greaterthan)

	RBTreeNode *root;
	int result;
	bool wasAdded;

	result = createTree (b.value, &root);

	ck_assert_msg (result == RB_SUCCESS, "Couldn't create a tree to test.");

	result = add (root, c.value, &wasAdded, compareChars);

	ck_assert_msg (result == RB_SUCCESS, "Didn't report adding a node as expected.");
	ck_assert_msg (wasAdded == true, "Didn't report adding a node as expected.");
	ck_assert_msg (b.left == NULL, "Changed the wrong branch.");
	checkNodeEquals (b.right, &(RBTreeNode){.value = c.value, .colour = RB_RED});
	
	freeTree (root);

END_TEST
	
	// Checks that adding a value to a multi-level tree puts it in the right place.
START_TEST (add_complex)

	RBTreeNode *root;
	int result;
	bool wasAdded;
	
	// Sets up standard alphabetical ordering, but ensures that c is not included yet.
	result = createTree (&'d', &root);
	
	ck_assert_msg (result == RB_SUCCESS, "Couldn't create a tree to test.");
	
	char insertOrder[] = {'b', 'f', 'a', 'c', 'e', 'g'};

	int i;
	for (i = 0; i < 6; ++i) {
		
		result = add (root, &insertOrder[i], &wasAdded, compareChars);
		ck_assert_msg (result == RB_SUCCESS, "Couldn't create a tree to test - couldn't add all the nodes.");
		
	}

	ck_assert_msg (*(char *)(b.right->value) == 'c', "Didn't add 'c' to the right of 'b'.");
	ck_assert_msg (b.right->colour == RB_RED, "Didn't set 'c' to a red node.");

	freeTree (root);

END_TEST

Suite *rb_tree_suite (void)
{
	Suite *s = suite_create ("rb tree suite");
	TCase *tc_core = tcase_create ("Core");
	tcase_add_checked_fixture (tc_core, setup, teardown);
	tcase_add_test (tc_core, rotate_right);
	tcase_add_test (tc_core, rotate_left);
	tcase_add_test (tc_core, add_simple_lessthan);
	tcase_add_test (tc_core, add_simple_greaterthan);
	tcase_add_test (tc_core, add_complex);
	suite_add_tcase (s, tc_core);
	
	return s;
}

int main (void)
{
	int nFailed;
	Suite *s = rb_tree_suite ();
	SRunner *sr = srunner_create (s);
	srunner_run_all (sr, CK_NORMAL);
	nFailed = srunner_ntests_failed (sr);
	srunner_free (sr);
	
	return (nFailed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}