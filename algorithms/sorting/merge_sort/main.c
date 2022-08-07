#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void merge(int *list, int left, int mid, int right)
{
	int *list_l = list + left;
	int list_l_len = mid - left + 1;

	int *list_r = list + mid + 1;
	int list_r_len = right - mid;

	int tmp_size = right - left + 1;
	int *tmp = (int *)calloc(tmp_size, sizeof(int));
	int i = 0, j = 0, index = 0;

	while (i < list_l_len || j < list_r_len) {
		if ((i < list_l_len && list_l[i] <= list_r[j]) ||
		    (j >= list_r_len)) {
			tmp[index++] = list_l[i++];
		} else if ((j < list_r_len && list_l[i] > list_r[j]) ||
			   (i >= list_l_len)) {
			tmp[index++] = list_r[j++];
		}
	}

	memcpy(list_l, tmp, tmp_size * sizeof(int));
	free(tmp);
}

static void merge_sort(int *list, int left, int right)
{
	int mid = (left + right) / 2;

	if (left < right) {
		merge_sort(list, left, mid);
		merge_sort(list, mid + 1, right);
		merge(list, left, mid, right);
	}
}

static void sort(int *list, int size)
{
	merge_sort(list, 0, size - 1);
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
