#ifndef _MAIN_H_
#define _MAIN_H_
#include <stdbool.h>

#define TEXT_COLOR_RED "\033[0;31m"
#define TEXT_COLOR_END "\033[0;m"


enum menu_index { INSERT, DELETE, TRAVERSE, SELF_TEST, EXIT, MAX };

enum color {
	BLACK,
	RED,
	INVALID,
};

enum direction {
        DIR_NONE,
        DIR_LEFT,
        DIR_RIGHT,
};

#define child_dir(N) ( N == (N->parent)->right ? DIR_RIGHT : DIR_LEFT )

enum rbt_delete_case {
	RBT_DEL_CASE_VALID,
	RBT_DEL_CASE_1,
	RBT_DEL_CASE_2,
	RBT_DEL_CASE_3,
	RBT_DEL_CASE_4,
	RBT_DEL_CASE_5,
	RBT_DEL_CASE_6,
};

struct rbtree_node {
	int value;
	enum color color;
	bool null;
	struct rbtree_node *parent;
	struct rbtree_node *left;
	struct rbtree_node *right;
};


#endif /* !_MAIN_H_ */
