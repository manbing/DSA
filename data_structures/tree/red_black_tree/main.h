#ifndef _MAIN_H_
#define _MAIN_H_

#define TEXT_COLOR_RED "\033[0;31m"
#define TEXT_COLOR_END "\033[0;m"

enum menu_index { INSERT, DELETE, TRAVERSE, SELF_TEST, EXIT, MAX };

enum color {
	BLACK,
	RED,
};

struct rbtree_node {
	int value;
	enum color color;
	struct rbtree_node *parent;
	struct rbtree_node *left;
	struct rbtree_node *right;
};

#endif /* !_MAIN_H_ */
