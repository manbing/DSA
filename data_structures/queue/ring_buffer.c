#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>

struct ring_buffer {
	size_t size;
	size_t head, tail;
	int *buffer;
	pthread_mutex_t lock;
	pthread_cond_t readable, writeable;
};

static inline bool is_tx_full(struct ring_buffer *q)
{
	size_t next = (q->tail + 1) % q->size;
	return (next == q->.head);
}

static inline bool is_tx_empty(struct ring_buffer *q)
{
	return (q->tail == q->head);
}

void put(struct ring_buffer *q, int value)
{
	pthread_mutex_lock(&q->lock);

	while (is_tx_full())
		pthread_cond_wait(&q->writeable, &q->lock);

	q->buffer[q->tail] = value;
	q->tail = (q->tail + 1) % q->size;

	pthread_cond_signal(&q->readable);
	pthread_mutex_unlock(&q->lock);
}

void get(struct ring_buffer *q, int *output)
{
	pthread_mutex_lock(&q->lock);
	
	while (is_tx_empty())
		pthread_cond_wait(&q->readable, &q->lock);

	*output = q->buffer[q->head];
	q->head = (q->head + 1) % q->size;

	pthread_cond_signal(&q->writeable);
	pthread_mutex_unlock(&q->lock);
}

void deinit(struct ring_buffer *q)
{
	pthread_mutex_destroy(&q->lock);
	pthread_cond_destroy(&q->readable);
	pthread_cond_destroy(&q->writeable);
	free(q->buffer);
	free(q);
}

struct ring_buffer *init(size_t size)
{
	struct ring_buffer *new = (struct ring_buffer *)calloc(1, sizeof(struct ring_buffer ));
	if (!new) {
		perror("calloc() fail\n");
		return NULL;
	}

	new->size = size;
	new->head = q->tail = 0;
	new->buffer = (int *)calloc(size, sizeof(int));
	pthread_mutex_init(&new->lock, NULL);
	pthread_cond_init(&new->readable, NULL);
	pthread_cond_init(&new->writeable, NULL);
	return new;
}
