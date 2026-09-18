#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct queue_metadata {
	int size;
	int front;
	int back;
	int *queue;
};

struct queue_operations {
	int (*enqueue)(struct queue_metadata *queue, int value);
	int (*dequeue)(struct queue_metadata *queue, int *output);
	bool (*is_full)(struct queue_metadata *queue);
	bool (*is_empty)(struct queue_metadata *queue);
	struct queue_metadata *(*init)(int size);
	void (*destroy)(struct queue_metadata *queue);
	void (*show)(struct queue_metadata *queue);
	void (*clean)(struct queue_metadata *queue);
};

static struct queue_metadata *init(int size);
static int dequeue(struct queue_metadata *tmp, int *output);
static int enqueue(struct queue_metadata *tmp, int value);
static bool is_full(struct queue_metadata *tmp);
static bool is_empty(struct queue_metadata *tmp);
static void destroy(struct queue_metadata *tmp);
static void show(struct queue_metadata *tmp);
static void clean(struct queue_metadata *tmp);

static struct queue_operations queue_ops = {
	.enqueue = enqueue,
	.dequeue = dequeue,
	.is_full = is_full,
	.is_empty = is_empty,
	.init = init,
	.show = show,
	.destroy = destroy,
	.clean = clean,
};

static struct queue_metadata *init(int size)
{
	struct queue_metadata *tmp = NULL;

	tmp = (struct queue_metadata *)calloc(1, sizeof(struct queue_metadata));
	tmp->size = size + 1;
	tmp->front = tmp->back = 0;
	tmp->queue = (int *)calloc(size + 1, sizeof(int));

	return tmp;
}

static int dequeue(struct queue_metadata *tmp, int *output)
{
	if (queue_ops.is_empty(tmp)) {
		return -1;
	}

	tmp->front = (tmp->front + 1) % tmp->size;
        *output = tmp->queue[tmp->front];
	return tmp->front;
}

static int enqueue(struct queue_metadata *tmp, int value)
{
	if (queue_ops.is_full(tmp)) {
		return -1;
	}

	tmp->back = (tmp->back + 1) % tmp->size;
	tmp->queue[tmp->back] = value;
        return tmp->back;
}

bool is_full(struct queue_metadata *tmp)
{
	return ((tmp->back + 1) % tmp->size == tmp->front) ? true : false;
}

bool is_empty(struct queue_metadata *tmp)
{
	return (tmp->back == tmp->front) ? true : false;
}

void destroy(struct queue_metadata *tmp)
{
	free(tmp->queue);
	free(tmp);
}

void show(struct queue_metadata *tmp)
{
	int front = tmp->front;
	int back = tmp->back;
	int size = tmp->size;

	printf("[");
	while ((front % size) != back % size) {
		printf("%d, ", tmp->queue[(++front) % size]);
	}
	printf("]\n");
}

void clean(struct queue_metadata *tmp)
{
	tmp->front = tmp->back = 0;
}

int main(void)
{
	struct queue_metadata *queue = NULL;
        int output = 0, i = 0;

	queue = queue_ops.init(3);

        for (i = 1; i <= 4; i++) {
                if (queue_ops.enqueue(queue, i) == -1) {
                        printf("%d enqueue fail...\n", i);
                } else {
                        printf("%d enqueue success...\n", i);
                }
        }

        printf("show queue\n");
        queue_ops.show(queue);

        for (i = 1; i <= 4; i++) {
                if (queue_ops.dequeue(queue, &output) == -1) {
                        printf("dequeue fail...\n");
                } else {
                        printf("dequeue success, %d...\n", output);
                }
        }

        printf("show queue\n");
	queue_ops.show(queue);

        printf("enqueue 4\n");
	queue_ops.enqueue(queue, 4);
        printf("show queue\n");
	queue_ops.show(queue);
        printf("clean queue\n");
	queue_ops.clean(queue);
	if (queue_ops.is_empty(queue))
		printf("queue is empty\n");

        printf("show queue\n");
	queue_ops.show(queue);

        printf("detroy queue\n");
	queue_ops.destroy(queue);

	return 0;
}
