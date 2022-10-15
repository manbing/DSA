#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"
#include "printf_bt.h"

static inline struct rbtree_node *G(struct rbtree_node *cur)
{
	struct rbtree_node *parent = cur->parent;

	if (!parent)
		return NULL;

	return parent->parent;
}

static inline struct rbtree_node *U(struct rbtree_node *cur)
{
	struct rbtree_node *parent = cur->parent;
	struct rbtree_node *grandparent = G(cur);

	if (!parent || !grandparent)
		return NULL;

	if (grandparent->right == parent)
		return grandparent->left;
	else
		return grandparent->right;
}

static inline struct rbtree_node *S(struct rbtree_node *cur)
{
	struct rbtree_node *parent = cur->parent;

	if (!parent)
		return NULL;

	if (parent->right == cur)
		return parent->left;
	else
		return parent->right;
}

char *menu[] = {
	[INSERT] = "Insert",	   [DELETE] = "Delete", [TRAVERSE] = "Traverse",
	[SELF_TEST] = "Self test", [EXIT] = "Exit",	[MAX] = NULL,
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
		retval->null = false;
		retval->left = NULL;
		retval->right = NULL;
	}

	return retval;
}

static struct rbtree_node *bst_add(struct rbtree_node *root,
				   struct rbtree_node *new_node)
{
	if (!root) {
		return new_node;
	}

	if (new_node->value > root->value) {
		if (!(root->right)) {
			root->right = new_node;
			new_node->parent = root;
		} else
			bst_add(root->right, new_node);
	} else if (new_node->value < root->value) {
		if (!(root->left)) {
			root->left = new_node;
			new_node->parent = root;
		} else
			bst_add(root->left, new_node);
	}

	return root;
}

static inline struct rbtree_node *find_successor(struct rbtree_node *root)
{
	struct rbtree_node *retval = root->right;

	while (retval->left) {
		retval = retval->left;
	}

	return retval;
}

static struct rbtree_node *rbt_bst_delete(struct rbtree_node *root,
					  struct rbtree_node *target,
					  struct rbtree_node **n)
{
	struct rbtree_node *retval = root;
	struct rbtree_node *l = target->left;
	struct rbtree_node *r = target->right;

	if (!root)
		return NULL;

	if (!l && !r) {
		target->null = true;
		*n = target;
	} else if (!l || !r) {
		if (l) {
			l->parent = target->parent;

			if (target->parent) {
				if (target->parent->left == target) {
					target->parent->left = l;
				} else {
					target->parent->right = l;
				}
			}

			if (root == target)
				retval = l;

			*n = l;
		} else {
			r->parent = target->parent;

			if (target->parent) {
				if (target->parent->left == target) {
					target->parent->left = r;
				} else {
					target->parent->right = r;
				}
			}

			if (root == target)
				retval = r;

			*n = r;
		}

		free(target);
	} else {
		struct rbtree_node *replace = NULL;

		//replace = find_predecessor(target);
		replace = find_successor(target);
		target->value = replace->value;

		replace->null = true;
		*n = replace;
	}

	return retval;
}

static struct rbtree_node *bst_delete(struct rbtree_node *root,
				      struct rbtree_node *target)
{
	struct rbtree_node *retval = root;
	struct rbtree_node *l = target->left;
	struct rbtree_node *r = target->right;

	if (!root)
		return NULL;

	if (!l && !r) {
		if (target->parent) {
			if (target->parent->left == target)
				target->parent->left = NULL;
			else
				target->parent->right = NULL;
		}

		if (target == root)
			retval = NULL;

		free(target);
	} else if (!l || !r) {
		if (l) {
			l->parent = target->parent;

			if (target == root) {
				retval = l;
			} else if (target->parent) {
				if (target->parent->left == target) {
					target->parent->left = l;
				} else {
					target->parent->right = l;
				}
			}
		} else {
			r->parent = target->parent;

			if (target == root) {
				retval = r;
			} else if (target->parent) {
				if (target->parent->left == target) {
					target->parent->left = r;
				} else {
					target->parent->right = r;
				}
			}
		}

		free(target);
	} else {
		struct rbtree_node *replace = NULL;

		//replace = find_predecessor(target);
		replace = find_successor(target);
		target->value = replace->value;

		if (replace->right) {
			replace->right->parent = replace->parent;
			replace->parent->left = replace->right;
		} else {
			replace->parent->left = NULL;
		}

		free(replace);
	}

	return retval;
}

static struct rbtree_node *bst_search(struct rbtree_node *root, int value)
{
	struct rbtree_node *retval = NULL;
	int r_value = root->value;

	if (r_value == value)
		return root;
	else if (r_value < value)
		retval = bst_search(root->right, value);
	else
		retval = bst_search(root->left, value);

	return retval;
}

static struct rbtree_node *right_rotate(struct rbtree_node *anchor)
{
	struct rbtree_node *new_root = NULL;
	struct rbtree_node *left = anchor->left;
	struct rbtree_node *left_right = left->right;

	left->parent = anchor->parent;
	anchor->left = left_right;

	if (left_right)
		left_right->parent = anchor;

	if (!(anchor->parent))
		new_root = left;
	else if (anchor == anchor->parent->left)
		anchor->parent->left = left;
	else
		anchor->parent->right = left;

	left->right = anchor;
	anchor->parent = left;

	return new_root;
}

static struct rbtree_node *left_rotate(struct rbtree_node *anchor)
{
	struct rbtree_node *new_root = anchor;
	struct rbtree_node *right = anchor->right;
	struct rbtree_node *right_left = right->left;

	if (!right)
		return new_root;

	right->parent = anchor->parent;
	anchor->right = right_left;

	if (right_left)
		right_left->parent = anchor;

	if (!(anchor->parent))
		new_root = right;
	else if (anchor == anchor->parent->left)
		anchor->parent->left = right;
	else
		anchor->parent->right = right;

	right->left = anchor;
	anchor->parent = right;

	return new_root;
}

static struct rbtree_node *rotate_bst(struct rbtree_node *anchor,
				      enum direction dir)
{
	struct rbtree_node *root = anchor;

	if (dir == DIR_LEFT)
		root = left_rotate(anchor);
	else if (dir == DIR_RIGHT)
		root = right_rotate(anchor);

	return root;
}

static struct rbtree_node *fixup(struct rbtree_node *root,
				 struct rbtree_node *cur)
{
	struct rbtree_node *parent = NULL;
	struct rbtree_node *grandparent = NULL;
	struct rbtree_node *uncle = NULL;

	while ((cur != root) && (cur->color == RED) &&
	       (cur->parent->color == RED)) {
		parent = cur->parent;
		grandparent = G(cur);

		if (parent == grandparent->left) {
			uncle = U(cur);

			// Case 2
			if (uncle && uncle->color == RED) {
				grandparent->color = RED;
				parent->color = BLACK;
				uncle->color = BLACK;
				cur = grandparent;
			} else {
				// Case 5
				if (cur == parent->right) {
					rotate_bst(parent, DIR_LEFT);
					cur = parent;
					parent = cur->parent;
				}

				// Case 6
				if (grandparent == root)
					root = rotate_bst(grandparent,
							  DIR_RIGHT);
				else
					rotate_bst(grandparent, DIR_RIGHT);
				grandparent->color = RED;
				parent->color = BLACK;
				cur = parent;
			}
		} else {
			/* 
			 * parent is right child of gandparent
			 */
			uncle = U(cur);

			// Case 2
			if (uncle && uncle->color == RED) {
				grandparent->color = RED;
				parent->color = BLACK;
				uncle->color = BLACK;
				cur = grandparent;
			} else {
				// Case 5
				if (cur == parent->left) {
					rotate_bst(parent, DIR_RIGHT);
					cur = parent;
					parent = cur->parent;
				}

				// Case 6
				if (grandparent == root) {
					root = rotate_bst(grandparent,
							  DIR_LEFT);
				} else
					rotate_bst(grandparent, DIR_LEFT);
				grandparent->color = RED;
				parent->color = BLACK;
				cur = parent;
			}
		}
	}

	root->color = BLACK;
	return root;
}

static bool __insert(struct rbtree_node **root, int value)
{
	struct rbtree_node *new_node = NULL;

	new_node = rbtree_ops.new(value);
	*root = bst_add(*root, new_node);
	*root = fixup(*root, new_node);

	return true;
}

static struct rbtree_node *delete_fixup(struct rbtree_node *root,
					struct rbtree_node *n)
{
	struct rbtree_node *p = NULL, *s = NULL, *c = NULL, *d = NULL;
	enum direction dir = DIR_NONE;

	p = n->parent;
	if (!p)
		goto valid;

	dir = child_dir(n);
	goto start_del;

	do {
		dir = child_dir(n);
	start_del:
		s = S(n);
		if (s) {
			c = s->left;
			d = s->right;
		}

		if (!s)
			goto valid;

		if (s->color == RED)
			goto RBT_DEL_CASE_3;

		if (d && d->color == RED)
			goto RBT_DEL_CASE_6;

		if (c && c->color == RED)
			goto RBT_DEL_CASE_5;

		if (p->color == RED)
			goto RBT_DEL_CASE_4;

		//RBT_DEL_CASE_1:
		s->color = RED;

		if (n->null)
			bst_delete(root, n);
		if (s && s->null)
			bst_delete(root, s);

		n = p;
	} while ((p = n->parent) != NULL);

	goto valid;

RBT_DEL_CASE_3:
	if (root == p)
		root = rotate_bst(p, dir);
	else
		rotate_bst(p, dir);

	s->color = BLACK;
	p->color = RED;
	if (dir == DIR_RIGHT)
		s = d;
	else
		s = c;

	if (!s)
		goto valid;

	d = s->right;
	c = s->left;

	if (d && d->color == RED)
		goto RBT_DEL_CASE_6;

	if (c && c->color == RED)
		goto RBT_DEL_CASE_5;

RBT_DEL_CASE_4:
	s->color = RED;
	p->color = BLACK;
	goto valid;

RBT_DEL_CASE_5:
	rotate_bst(s, ((dir == DIR_LEFT) ? DIR_RIGHT : DIR_LEFT));
	s->color = RED;
	c->color = BLACK;
	d = s;
	s = c;

RBT_DEL_CASE_6:
	if (root == p)
		root = rotate_bst(p, dir);
	else
		rotate_bst(p, dir);

	s->color = p->color;
	p->color = BLACK;
	if (dir == DIR_RIGHT) {
		if (c)
			c->color = BLACK;
	} else {
		if (d)
			d->color = BLACK;
	}

valid:
	if (n->null)
		root = bst_delete(root, n);
	if (s && s->null)
		root = bst_delete(root, s);
	if (root)
		root->color = BLACK;

	return root;
}

static bool __delete(struct rbtree_node **root, int value)
{
	bool retval = false;
	enum color target_color;
	struct rbtree_node *n = NULL;

	struct rbtree_node *target = bst_search(*root, value);
	if (!target) {
		printf("no target\n");
		return retval;
	}

	target_color = target->color;

	if (target_color == BLACK) {
		*root = rbt_bst_delete(*root, target, &n);
		*root = delete_fixup(*root, n);
	} else if (target_color == RED) {
		*root = bst_delete(*root, target);
	}

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

			rbtree_ops.traverse((const struct rbtree_node *)root);
			printf("\n\n\n");
			break;

		case DELETE:
			printf("Enter the element to delete:");
			scanf("%d", &data);
			rbtree_ops.delete(&root, data);

			rbtree_ops.traverse((const struct rbtree_node *)root);
			printf("\n\n\n");
			break;

		case TRAVERSE:
			rbtree_ops.traverse((const struct rbtree_node *)root);
			printf("\n\n\n");
			break;

		case SELF_TEST:
			test_mode(&root);
			rbtree_ops.traverse((const struct rbtree_node *)root);
			printf("\n\n\n");
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
