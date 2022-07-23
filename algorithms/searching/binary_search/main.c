static int binary_serach(int *nums, int start, int end, int target)
{
    int retval = -1;
    int mid = (end + start) / 2;

    if (nums[mid] == target)
        return mid;
    else if (nums[mid] < target && mid + 1 <= end)
        retval = binary_serach(nums, mid + 1, end, target);
    else if (nums[mid] > target && start <= mid - 1)
        retval = binary_serach(nums, start, mid - 1, target);

    return retval;
}

int search(int* nums, int numsSize, int target){
    return binary_serach(nums, 0, numsSize - 1, target);
}
