#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define LCHID(x) (x * 2 + 1)
#define RCHID(x) (x * 2 + 2)
#define PARENT(x) ((x - 1) / 2)

static void swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

static void sift_down(int i, int *obj, size_t size)
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
		sift_down(largest, obj, size);
	}
}

static void heapify(int *obj, size_t size)
{
	int i = 0;

	for (i = (size - 1) / 2; i >= 0; i--)
		sift_down(i, obj, size);
}

int main(void)
{
	int nums[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1 };
	int ans[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int numsSize = sizeof(nums) / sizeof(nums[0]);
	int end = 0;

	// heapify
	heapify(nums, numsSize);

	end = numsSize;
	while (end > 1) {
		end -= 1;
		swap(&nums[0], &nums[end]);
		sift_down(0, nums, end);
	}

	assert(!memcmp(nums, ans, numsSize * sizeof(int)) && "Wrong answer");

	return 0;
}
