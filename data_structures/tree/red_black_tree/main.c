#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"
#include "printf_bt.h"

#define P(x) (x->parent)
#define R(x) (x->right)
#define L(x) (x->left)
#define C(x) (x->color)
#define V(x) (x->value)

static inline struct rbtree_node *G(struct rbtree_node *cur)
{
	struct rbtree_node *parent = P(cur);

	if (!parent)
		return NULL;

	return P(parent);
}

static inline struct rbtree_node *U(struct rbtree_node *cur)
{
	struct rbtree_node *parent = P(cur);
	struct rbtree_node *grandparent = G(cur);

	if (!parent || !grandparent)
		return NULL;

	if (R(grandparent) == parent)
		return L(grandparent);
	else
		return R(grandparent);
}

char *menu[] = {
	[INSERT] = "Insert",       [DELETE] = "Delete", [TRAVERSE] = "Traverse",
	[SELF_TEST] = "Self test", [EXIT] = "Exit",     [MAX] = NULL,
};

static struct rbtree_ops {
	struct rbtree_node *	(*new)(int value);
	bool 			(*insert)(struct rbtree_node **root, int value);
	bool 			(*delete)(struct rbtree_node **root, int value);
	void 			(*traverse)(const struct rbtree_node *root);
	void 			(*free)(struct rbtree_node *root);
} rbtree_ops;

static struct rbtree_node *__new(int value)
{
	struct rbtree_node *retval = NULL;
	retval = (struct rbtree_node *)malloc(1 * sizeof(struct rbtree_node));
	if (retval) {
		memset(retval, 0, sizeof(struct rbtree_node));
		retval->value = value;
		retval->color = RED;
		retval->parent = NULL;
		retval->left = NULL;
		retval->right = NULL;
	}

	return retval;
}

static struct rbtree_node *bst(struct rbtree_node *root,
			       struct rbtree_node *new_node)
{
	if (!root) {
		return new_node;
	}

	if (new_node->value > root->value) {
		if (!R(root)) {
			R(root) = new_node;
			P(new_node) = root;
		} else
			bst(R(root), new_node);
	} else if (new_node->value < root->value) {
		if (!L(root)) {
			L(root) = new_node;
			P(new_node) = root;
		} else
			bst(L(root), new_node);
	}

	return root;
}

static struct rbtree_node *right_rotate(struct rbtree_node *anchor)
{
	struct rbtree_node *new_root = NULL;
	struct rbtree_node *left = L(anchor);
	struct rbtree_node *left_right = R(left);

	P(left) = P(anchor);
	L(anchor) = left_right;

	if (left_right)
		P(left_right) = anchor;

	if (!P(anchor))
		new_root = left;
	else if (anchor == L(P(anchor)))
		L(P(anchor)) = left;
	else
		R(P(anchor)) = left;

	R(left) = anchor;
	P(anchor) = left;

	return new_root;
}

static struct rbtree_node *left_rotate(struct rbtree_node *anchor)
{
	struct rbtree_node *new_root = NULL;
	struct rbtree_node *right = R(anchor);
	struct rbtree_node *right_left = L(right);

	P(right) = P(anchor);
	R(anchor) = right_left;

	if (right_left)
		P(right_left) = anchor;

	if (!P(anchor))
		new_root = right;
	else if (anchor == L(P(anchor)))
		L(P(anchor)) = right;
	else
		R(P(anchor)) = right;

	L(right) = anchor;
	P(anchor) = right;

	return new_root;
}

static struct rbtree_node *fixup(struct rbtree_node *root,
				 struct rbtree_node *cur)
{
	struct rbtree_node *parent = NULL;
	struct rbtree_node *grandparent = NULL;
	struct rbtree_node *uncle = NULL;

	while ((cur != root) && (C(cur) == RED) && (C(P(cur)) == RED)) {
		parent = P(cur);
		grandparent = G(cur);

		if (parent == L(grandparent)) {
			uncle = U(cur);

			// Case 2
			if (uncle && C(uncle) == RED) {
				C(grandparent) = RED;
				C(parent) = BLACK;
				C(uncle) = BLACK;
				cur = grandparent;
			} else {
				// Case 5
				if (cur == R(parent)) {
					left_rotate(parent);
					cur = parent;
					parent = P(cur);
				}

				// Case 6
				if (grandparent == root)
					root = right_rotate(grandparent);
				else
					right_rotate(grandparent);
				C(grandparent) = RED;
				C(parent) = BLACK;
				cur = parent;
			}
		} else {
			/* 
			 * parent is right child of gandparent
			 */
			uncle = U(cur);

			// Case 2
			if (uncle && C(uncle) == RED) {
				C(grandparent) = RED;
				C(parent) = BLACK;
				C(uncle) = BLACK;
				cur = grandparent;
			} else {
				// Case 5
				if (cur == L(parent)) {
					right_rotate(parent);
					cur = parent;
					parent = P(cur);
				}

				// Case 6
				if (grandparent == root) {
					root = left_rotate(grandparent);
				} else
					left_rotate(grandparent);
				C(grandparent) = RED;
				C(parent) = BLACK;
				cur = parent;
			}
		}
	}

	C(root) = BLACK;
	return root;
}

static bool __insert(struct rbtree_node **root, int value)
{
	struct rbtree_node *new_node = NULL;

	new_node = rbtree_ops.new(value);
	*root = bst(*root, new_node);
	*root = fixup(*root, new_node);

	return true;
}

static bool __delete(struct rbtree_node **root, int value)
{
	bool retval = false;

	return retval;
}

static void __free(struct rbtree_node *root)
{
	if (!root)
		return;

	if (!root->left) {
		__free(root->left);
	}

	if (!root->right) {
		__free(root->right);
	}

	free(root);
}

static void test_mode(struct rbtree_node **root)
{
	int i = 0;
	int input[] = { 5, 11, 7, 9, 13, 14, 3, 1, 0, 2, 4, 6, 10, 8, 12 };
	int size = sizeof(input) / sizeof(input[0]);

	for (i = 0; i < size; i++) {
		rbtree_ops.insert(root, input[i]);
		//printf("[%s:%d] start\n", __func__, __LINE__);
		//rbtree_ops.traverse((const struct rbtree_node * const *) root);
		//printf("\n\n\n[%s:%d] end\n", __func__, __LINE__);
	}
}

int main(void)
{
	struct rbtree_node *root = NULL;
	int input = 0, data = 0;

	rbtree_ops.new = __new;
	rbtree_ops.insert = __insert;
	rbtree_ops.delete = __delete;
	rbtree_ops.traverse = print2D;
	rbtree_ops.free = __free;

	while (1) {
		printf("\n\n\n");
		printf("%d, %s\n", INSERT, menu[INSERT]);
		printf("%d, %s\n", DELETE, menu[DELETE]);
		printf("%d, %s\n", TRAVERSE, menu[TRAVERSE]);
		printf("%d, %s\n", SELF_TEST, menu[SELF_TEST]);
		printf("%d, %s\n", EXIT, menu[EXIT]);
		printf("\nEnter your choice:");
		scanf("%d", &input);

		switch (input) {
		case INSERT:
			printf("Enter the element to insert:");
			scanf("%d", &data);
			rbtree_ops.insert(&root, data);
			break;

		case DELETE:
			printf("Enter the element to insert:");
			scanf("%d", &data);
			rbtree_ops.delete(&root, data);
			break;

		case TRAVERSE:
			rbtree_ops.traverse((const struct rbtree_node *)root);
			printf("\n\n\n");
			break;

		case SELF_TEST:
			test_mode(&root);
			break;

		case EXIT:
			goto out_free;
			break;

		default:
			break;
		}
	}

out_free:
	rbtree_ops.free(root);

	return 0;
}
