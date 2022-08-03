#include <stdlib.h>
#include <stdio.h>

static void show_list(int list[], int start, int end)
{
	int i;

	for (i = start; i <= end; i++) {
		printf("%d, ", list[i]);
	}
	printf("\n");
}

static inline void swap_value(int *value, int *value2)
{
	int tmp = *value;
	*value = *value2;
	*value2 = tmp;
}

static void __quicksort(int list[], int left, int right)
{
	int pivot = list[right];
	int i = left, j = right - 1;

	if (right <= left)
		return;

	while (i != j) {
		for (; i < j && pivot >= list[i];)
			i++;

		for (; j > i && pivot < list[j];)
			j--;

		swap_value(&list[i], &list[j]);
	}

	if (list[i] > list[right])
		swap_value(&list[right], &list[i]);

	//left
	__quicksort(list, left, i - 1);

	//right
	__quicksort(list, i + 1, right);
}

static void quicksort(int list[], size_t size)
{
	__quicksort(list, 0, size - 1);
}

int main(void)
{
	int list[] = { 89, 34, 23, 78, 67, 100, 66, 27, 79,
		       55, 78, 78, 78, 88, 92,  96, 96, 23 };

	printf("Before sort:\n");
	show_list(list, 0, sizeof(list) / sizeof(list[0]) - 1);

	quicksort(list, sizeof(list) / sizeof(list[0]));

	printf("\nAfter sort:\n");
	show_list(list, 0, sizeof(list) / sizeof(list[0]) - 1);

	return 0;
}
