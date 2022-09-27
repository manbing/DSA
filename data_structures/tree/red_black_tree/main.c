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
#define N(x) (x->null)

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

static inline struct rbtree_node *S(struct rbtree_node *cur)
{
	struct rbtree_node *parent = P(cur);

	if (!parent)
		return NULL;

	if (R(parent) == cur)
		return L(parent);
	else
		return R(parent);

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
		if (!R(root)) {
			R(root) = new_node;
			P(new_node) = root;
		} else
			bst_add(R(root), new_node);
	} else if (new_node->value < root->value) {
		if (!L(root)) {
			L(root) = new_node;
			P(new_node) = root;
		} else
			bst_add(L(root), new_node);
	}

	return root;
}

static inline struct rbtree_node *find_successor(struct rbtree_node *root)
{
	 struct rbtree_node *retval = R(root);

	 while(L(retval)) {
		 retval = L(retval);
	 }

	 return retval;
}

static struct rbtree_node *rbt_bst_delete(struct rbtree_node *root, struct rbtree_node *target, struct rbtree_node **n)
{
	struct rbtree_node *retval = root;
	struct rbtree_node *l = L(target);
	struct rbtree_node *r = R(target);

	if (!root)
		return NULL;

	if (!l && !r) {
                N(target) = true;
		*n = target;
	} else if (!l || !r) {
		if (l) {
			P(l) = P(target);

                        if (P(target)) {
                                if (L(P(target)) == target) {
                                        L(P(target)) = l;
                                } else {
                                        R(P(target)) = l;
                                }
                        }

                        if (root == target)
                                retval = l;

			*n = l;
		} else {
			P(r) = P(target);

                        if (P(target)) {
                                if (L(P(target)) == target) {
                                        L(P(target)) = r;
                                } else {
                                        R(P(target)) = r;
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
                V(target) = V(replace);

                N(replace) = true;
                *n = replace;
        }

        return retval;
}

static struct rbtree_node *bst_delete(struct rbtree_node *root, struct rbtree_node *target)
{
	struct rbtree_node *retval = root;
	struct rbtree_node *l = L(target);
	struct rbtree_node *r = R(target);

	if (!root)
		return NULL;

	if (!l && !r) {
                if (P(target)) {
                        if (L(P(target)) == target)
                                L(P(target)) = NULL;
                        else 
                                R(P(target)) = NULL;
                }

		if (target == root)
			retval = NULL;

                free(target);
	} else if (!l || !r) {
		if (l) {
			P(l) = P(target);

                        if (target == root) {
                                retval = l;
                        } else if (P(target)) {
                                if (L(P(target)) == target) {
                                        L(P(target)) = l;
                                } else {
                                        R(P(target)) = l;
                                }
                        }
		} else {
			P(r) = P(target);

                        if (target == root) {
                                retval = r;
                        } else if (P(target)) {
                                if (L(P(target)) == target) {
                                        L(P(target)) = r;
                                } else {
                                        R(P(target)) = r;
                                }
                        }
		}

		free(target);
	} else {
		struct rbtree_node *replace = NULL;

		//replace = find_predecessor(target);
		replace = find_successor(target);
		V(target) = V(replace);
		
		if (R(replace)) {
			P(R(replace)) = P(replace);
			L(P(replace)) = R(replace);
                } else {
                        L(P(replace)) = NULL;
                }
                
                free(replace);
	}

	return retval;
}


static struct rbtree_node *bst_search(struct rbtree_node *root, int value)
{
	struct rbtree_node *retval = NULL;

	if (V(root) == value)
		return root;
	else if (V(root) < value)
		retval = bst_search(R(root), value);
	else
		retval = bst_search(L(root), value);

	return retval;
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
	struct rbtree_node *new_root = anchor;
	struct rbtree_node *right = R(anchor);
	struct rbtree_node *right_left = L(right);

        if (!right)
                return new_root;

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

static struct rbtree_node *rotate_bst(struct rbtree_node *anchor, enum direction dir)
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
                                        rotate_bst(parent, DIR_LEFT);
					cur = parent;
					parent = P(cur);
				}

				// Case 6
				if (grandparent == root)
					root = rotate_bst(grandparent, DIR_RIGHT);
				else
					rotate_bst(grandparent, DIR_RIGHT);
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
					rotate_bst(parent, DIR_RIGHT);
					cur = parent;
					parent = P(cur);
				}

				// Case 6
				if (grandparent == root) {
					root = rotate_bst(grandparent, DIR_LEFT);
				} else
                                        rotate_bst(grandparent, DIR_LEFT);
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
	*root = bst_add(*root, new_node);
	*root = fixup(*root, new_node);

	return true;
}

static struct rbtree_node *delete_fixup(struct rbtree_node *root, struct rbtree_node *Now)
{
        struct rbtree_node *Parent = NULL, *Sibling = NULL, *Close_nephew = NULL, *Distant_nephew = NULL;
        enum direction dir = DIR_NONE; 

        Parent = P(Now);
        if (!Parent)
                goto valid;

        dir = child_dir(Now);
        goto start_del;

        do {
                dir = child_dir(Now);
start_del:
                Sibling = S(Now);
                if (Sibling) {
                        Close_nephew = L(Sibling);
                        Distant_nephew = R(Sibling);
                }
                
                if (!Sibling)
                        goto valid;

                if (C(Sibling) == RED)
                        goto RBT_DEL_CASE_3;

                if (Distant_nephew && C(Distant_nephew) == RED)
                        goto RBT_DEL_CASE_6;

                if (Close_nephew && C(Close_nephew) == RED)
                        goto RBT_DEL_CASE_5;

                if (C(Parent) == RED)
                        goto RBT_DEL_CASE_4;

//RBT_DEL_CASE_1:
                C(Sibling) = RED;

                if (N(Now)) bst_delete(root, Now);
                if (Sibling && N(Sibling)) bst_delete(root, Sibling);
                
                Now = Parent;
        } while ((Parent = Now->parent) != NULL);

        goto valid;

RBT_DEL_CASE_3:
        if (root == Parent)
                root = rotate_bst(Parent, dir);
        else
                rotate_bst(Parent, dir);

        C(Sibling) = BLACK;
        C(Parent) = RED;
        if (dir == DIR_RIGHT)
                Sibling = Distant_nephew;
        else
                Sibling = Close_nephew;
        
        if (!Sibling)
                goto valid;

        Distant_nephew = R(Sibling);
        Close_nephew = L(Sibling);
        
        if (Distant_nephew && C(Distant_nephew) == RED)
                goto RBT_DEL_CASE_6;

        if (Close_nephew && C(Close_nephew) == RED)
                goto RBT_DEL_CASE_5;

RBT_DEL_CASE_4:
        C(Sibling) = RED;
        C(Parent) = BLACK;
        goto valid;


RBT_DEL_CASE_5:
        rotate_bst(Sibling, ((dir == DIR_LEFT)? DIR_RIGHT : DIR_LEFT));
        C(Sibling) = RED;
        C(Close_nephew) = BLACK;
        Distant_nephew = Sibling;
        Sibling = Close_nephew;

RBT_DEL_CASE_6:
        if (root == Parent)
                root = rotate_bst(Parent, dir);
        else
                rotate_bst(Parent, dir);

        C(Sibling) = C(Parent);
        C(Parent) = BLACK;
        if (dir == DIR_RIGHT) {
                if (Close_nephew) C(Close_nephew) = BLACK;
        } else {
        	if (Distant_nephew) C(Distant_nephew) = BLACK;
        }

valid:
        if (N(Now)) root = bst_delete(root, Now);
        if (Sibling && N(Sibling)) root = bst_delete(root, Sibling);
        if (root) C(root) = BLACK;

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

	target_color = C(target);
	
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
