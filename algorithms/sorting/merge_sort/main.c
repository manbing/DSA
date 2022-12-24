#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void merge_sort(int *obj, int left, int right)
{
	int i = 0, j = 0, k = 0;
	int len = right - left;
	int size = len + 1;
	int mid = (right + left) / 2;
	int *tmp = NULL;

	if (left == right)
		return;

	merge_sort(obj, left, mid);
	merge_sort(obj, mid + 1, right);

	tmp = (int *)calloc(size, sizeof(int));

	i = left;
	j = mid + 1;
	while (i <= mid || j <= right) {
		if (i > mid)
			tmp[k++] = obj[j++];
		else if (j > right)
			tmp[k++] = obj[i++];
		else if (obj[i] < obj[j])
			tmp[k++] = obj[i++];
		else
			tmp[k++] = obj[j++];
	}

	memcpy(&obj[left], tmp, size * sizeof(int));
	free(tmp);
}

static void sort(int *obj, int obj_size)
{
	merge_sort(obj, 0, obj_size - 1);
}

static void show_list(int *list, int size)
{
	int i = 0;

	for (i = 0; i < size; i++) {
		printf("%d, ", list[i]);
	}
}

int main(void)
{
	int list[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

	printf("Before sort:\n");
	show_list(list, sizeof(list) / sizeof(list[0]));

	sort(list, sizeof(list) / sizeof(list[0]));

	printf("\n\nAfter sort:\n");
	show_list(list, sizeof(list) / sizeof(list[0]));
	printf("\n\n");

	return 0;
}
