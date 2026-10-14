#ifndef __LRU_CACHE_H__
#define __LRU_CACHE_H__

#include <stdint.h>
#include <stddef.h>

struct entry {
	int key;
	int value;
	uint32_t prev; // toward MRU
	uint32_t next; // toward LRU
	uint32_t hnext;
};

struct lru_cache {
	uint32_t capacity;
	uint32_t nbuckets;
	uint32_t count;
	uint32_t head;
	uint32_t tail;
	uint32_t free;
	struct entry *pool;
	uint32_t *buckets;
};

struct lru_cache*lru_init(void *mem, size_t size);
int lru_get(struct lru_cache *c, int key, int *out);
void lru_put(struct lru_cache *c, int key, int value);
void lru_print(const struct lru_cache *c);

#endif /* !__LRU_CACHE_H__ */
