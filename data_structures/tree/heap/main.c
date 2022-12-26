#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LCHID(x)	(x * 2 + 1)
#define RCHID(x)	(x * 2 + 2)
#define PARENT(x)	((x - 1) / 2)

static void swap(int *obj, int *obj2)
{
	int tmp = 0;
	tmp = *obj;
	*obj = *obj2;
	*obj2 = tmp;
}

static void max_heapify(int *obj, int size,int i)
{
	int l = LCHID(i);
	int r = RCHID(i);
	int largest = i;

	if (l < size && obj[l] > obj[largest])
		largest = l;

	if (r < size && obj[r] > obj[largest])
		largest = r;

	if (largest != i) {
		swap(&obj[largest], &obj[i]);
		max_heapify(obj, size, largest);
	}
}


static void build_max_heap(int *obj, int size)
{
	int i = 0, len = size - 1;

	for (i = len / 2; i >= 0; i--) {
		max_heapify(obj, size, i);
	}
}

static void show(int *obj, int size)
{
	int i = 0;

	for (i = 0; i < size; i++) {
		printf("%d, ", obj[i]);
	}
}

int main(void)
{
	int list[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	printf("Before sort:\n");
	show(list, sizeof(list) / sizeof(list[0]));

	build_max_heap(list, sizeof(list) / sizeof(list[0]));

	printf("\n\nAfter sort:\n");
	show(list, sizeof(list) / sizeof(list[0]));
	printf("\n\n");

	return 0;
}
