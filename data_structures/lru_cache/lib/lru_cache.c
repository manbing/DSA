#include <stdio.h>

#include "lru_cache.h"

/* Helper Function  */
static void list_unlink(struct lru_cache *c, uint32_t i)
{
	struct entry *e = &c->pool[i];

	if (e->prev != UINT32_MAX) {
		c->pool[e->prev].next = e->next;
	} else {
		c->head = e->next;
	}

	if (e->next != UINT32_MAX) {
		c->pool[e->next].prev = e->prev;
	} else {
		c->tail = e->prev;
	}
}

static void list_put_front(struct lru_cache *c, uint32_t i)
{
	struct entry *e = &c->pool[i];

	e->next = c->head;
	e->prev = UINT32_MAX;

	if (c->head != UINT32_MAX)
		c->pool[c->head].prev = i;
	
	c->head = i;

	if (c->tail == UINT32_MAX)
		c->tail = i;
}

static uint32_t hash(struct lru_cache *c, int key)
{
	uint32_t h = (uint32_t)key;

	h ^= h >> 16;
	h *= 0x45d9f3bU;
	h ^= h >> 16;
	return h % c->nbuckets;
	
}

static uint32_t hash_find(struct lru_cache *c, int key)
{
	uint32_t i = c->buckets[hash(c, key)];

	while (i != UINT32_MAX && c->pool[i].key != key)
		i = c->pool[i].hnext;

	return i;
}

static void hash_remove(struct lru_cache *c, uint32_t i)
{
	uint32_t *ptr_bucket = &c->buckets[hash(c, c->pool[i].key)];

	while (*ptr_bucket != i) {
		ptr_bucket = &c->pool[*ptr_bucket].hnext;
	}

	*ptr_bucket = c->pool[i].hnext;
}

static void hash_insert(struct lru_cache *c, uint32_t i)
{
	uint32_t b = hash(c, c->pool[i].key);
	c->pool[i].hnext = c->buckets[b];
	c->buckets[b] = i;
}


/* Pulibic C API */

/**
 * lru_init -
 * @mem: the start address of free memeory space
 * @size: the sizeof free memeory space
 *
 * Return the address if success, NULL otherwise.
 */
struct lru_cache *lru_init(void *mem, size_t size)
{
	uintptr_t p = (uintptr_t)mem;
	uintptr_t aligned = (p + _Alignof(struct lru_cache) - 1) & ~(uintptr_t)(_Alignof(struct lru_cache) - 1);
	size_t pad_aligned = aligned - p;
	uint32_t i = 0;

	if (pad_aligned + sizeof(struct lru_cache) > size)
		return NULL;

	size -= pad_aligned + sizeof(struct lru_cache);

	/* sizeof(struct entry) is for pool; sizeof(uint32_t) is for bucket*/
	size_t cap = size / (sizeof(struct entry) + sizeof(uint32_t));
	if (cap == 0)
		return NULL;

	if (cap >= UINT32_MAX)
		cap = UINT32_MAX - 1;

	struct lru_cache *c = (struct lru_cache *)aligned;
	c->capacity = (uint32_t)cap;
	c->nbuckets = (uint32_t)cap;
	c->count = 0;
	c->head = c->tail = UINT32_MAX;
	/* (strcut lru_cache *)c + 1 is locating at LRU head end */
	c->pool = (struct entry *)(c + 1);
	/* bucket space is following pool space */
	c->buckets = (uint32_t *)(c->pool + cap);

	for (i = 0; i < c->nbuckets; i++)
		c->buckets[i] = UINT32_MAX;

	for (i = 0; i < c->capacity; i ++)
		c->pool[i].next = (i + 1 < c->capacity)? i + 1 : UINT32_MAX;

	c->free = 0;

	return c;
}

/**
 * lru_get -
 * @c:
 * @key:
 * @out:
 *
 * retrieve the data from @c and put in the @out, if found.
 *
 * Return 1 if found. 0 otherwise.
 */
int lru_get(struct lru_cache *c, int key, int *out)
{
	uint32_t i = hash_find(c, key);

	if (i == UINT32_MAX)
		return 0;

	list_unlink(c, i);
	list_put_front(c, i);
	*out = c->pool[i].value;
	return 1;
}

/**
 * lru_put -
 * @c:
 * @key:
 * @value:
 *
 * store @value in @c.
 */
void lru_put(struct lru_cache *c, int key, int value)
{
	uint32_t i = hash_find(c, key);

	if (i != UINT32_MAX) {
		c->pool[i].value = value;
		list_unlink(c, i);
		list_put_front(c, i);
		return;
	}

	if (c->free != UINT32_MAX) {
		i = c->free;
		c->free = c->pool[i].next;
		c->count++;
	} else {
		/* Full: evict the LRU node */
		i = c->tail;
		hash_remove(c, i);
		list_unlink(c, i);
	}

	c->pool[i].key = key;
	c->pool[i].value = value;
	hash_insert(c, i);
	list_put_front(c, i);
}

/**
 * lru_print -
 * @c:
 *
 * Return 1 if found. 0 otherwise.
 */
void lru_print(const struct lru_cache *c)
{
    printf("[MRU] ");
	for (uint32_t i = c->head; i != UINT32_MAX; i = c->pool[i].next)
		printf("(%d:%d) ", c->pool[i].key, c->pool[i].value);
    printf("[LRU]\n");
}
