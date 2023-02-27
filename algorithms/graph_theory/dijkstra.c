#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

int vertexes = 6;

struct node {
	bool fixed;
	int value;
};

static int min_next(int **matrix, struct node *dis)
{
	int i;
	int min = INT_MAX, min_vertex = -1;

	for (i = 0; i < vertexes; i++) {
		if (dis[i].fixed == false && dis[i].value < min) {
			min = dis[i].value;
			min_vertex = i;
		}
	}

	return min_vertex;
}

static void dijkstra(int **matrix, struct node *dis, int src)
{
	int i = 0, j = 0;

	for (i = 0; i < vertexes; i++) {
		dis[i].fixed = false;
		dis[i].value = INT_MAX;
	}

	dis[src].value = 0;

	for (i = 0; i < vertexes; i++) {
		int next = min_next(matrix, dis);

		dis[next].fixed = true;

		for (j = 0; j < vertexes; j++) {
			if (dis[j].fixed)
				continue;

			if (matrix[next][j] == INT_MAX)
				continue;

			if (dis[j].value < dis[next].value + matrix[next][j])
				continue;

			dis[j].value = dis[next].value + matrix[next][j];
		}
	}
}

int main(void)
{
	int i = 0, j = 0;
	int **matrix = NULL;

	// init
	matrix = (int **)calloc(vertexes, sizeof(int *));

	for (i = 0; i < vertexes; i++) {
		matrix[i] = (int *)calloc(vertexes, sizeof(int));
	}

	for (i = 0; i < vertexes; i++) {
		for (j = 0; j < vertexes; j++) {
			if (i == j)
				matrix[i][j] = 0;
			else
				matrix[i][j] = INT_MAX;
		}
	}

	matrix[0][1] = 1;
	matrix[0][2] = 12;
	matrix[1][3] = 3;
	matrix[1][2] = 9;
	matrix[2][4] = 5;
	matrix[3][2] = 4;
	matrix[3][4] = 13;
	matrix[3][5] = 15;
	matrix[4][5] = 4;

	// shortest path
	struct node *dis = NULL;

	dis = (struct node *)calloc(vertexes, sizeof(struct node));
	dijkstra(matrix, dis, 0);

	// show result
	printf("the shortest patch from vertex, 0, to\n");
	for (i = 0; i < vertexes; i++) {
		printf("vertext, %d, is %d.\n", i, dis[i].value);
	}

	//deinit
	free(dis);

	for (i = 0; i < vertexes; i++) {
		free(matrix[i]);
	}

	free(matrix);

	return 0;
}
