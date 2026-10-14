#include<stdio.h>
#include<stdlib.h>

#include "lru_cache.h"

int main(void)
{
    // A small region: room for exactly 3 entries after the header.
    static unsigned char mem[sizeof(struct lru_cache) + 3 * (sizeof(struct entry) + sizeof(uint32_t))
                             + _Alignof(struct lru_cache)];
    struct lru_cache * c = lru_init(mem, sizeof(mem));
    if (c == NULL) {
        printf("buffer too small\n");
        return 1;
    }
    printf("capacity = %u\n", c->capacity);

    lru_put(c, 1, 100);
    lru_put(c, 2, 200);
    lru_put(c, 3, 300);
    lru_print(c);                  // 3 2 1

    int v;
    if (lru_get(c, 1, &v))
        printf("get(1) = %d\n", v);
    lru_print(c);                  // 1 3 2

	printf("put (4:400)\n");
    lru_put(c, 4, 400);            // evicts 2
    lru_print(c);                  // 4 1 3
    
	printf("get(2) %s\n", lru_get(c, 2, &v) ? "hit" : "miss");
    lru_print(c);                  // 4 1 3 (miss leaves order unchanged)

	printf("put (3:333)\n");
    lru_put(c, 3, 333);            // update moves 3 to front
    lru_print(c);                  // 3 4 1
    return 0;
}
