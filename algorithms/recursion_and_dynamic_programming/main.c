/**
 * You are climbing a staircase. It takes n steps to reach the top.
 *
 * Each time you can either climb 1 or 2 steps. In how many distinct ways can you climb to the top?
 */

static int *memory = NULL;

static int __climbStairs(int n)
{
    if (n == 1 || n == 0)
        return 1;
    else if (memory[n - 1])
        return memory[n - 1];
    else {
        memory[n - 1] = __climbStairs(n - 1) + __climbStairs(n - 2);
        return memory[n - 1];
    }
}

int climbStairs(int n)
{
    int retval = -1;

    memory = (int *)calloc(n, sizeof(int));
    memory[0] = 1;

    retval = __climbStairs(n);

    free(memory);
    memory = NULL;
t
    return retval;
}
