#include <stdlib.h>

#include "cache.h"
#include "utils.h"
#include "cache_fa.h"

void cache_full_associative_init(struct cache_full_associative *fa, int size) {
    struct cache_entry *cache_array;
    /* Allocte size number of cache entries for cache_array */
    cache_array = (struct cache_entry *) \
        malloc_helper(sizeof(struct cache_entry) * size,
                      "cache_full_associative_init() : cache_entry");
    fa->cache_array = cache_array;
    fa->size = size;
    fa->refs = 0;
    fa->hits = 0;
    fa->misses = 0;
}

/* Stats for fully associative cache*/
void cache_full_associative_stats(struct cache_full_associative *fa) {
    printf("fa(%d) ", fa->size);
    printf("refs: %d ", fa->refs);
    printf("hits: %d ", fa->hits);
    printf("misses: %d ", fa->misses);
    printf("hit-ratio: %f ", (float) fa->hits / fa->refs);
    printf("miss-ratio: %f\n", (float) fa->misses / fa->refs);
}

/* Lookup for fully associative cache*/
unsigned int cache_lookup_full_associative(struct cache_full_associative *fa, unsigned int addr) {
    unsigned int tag, data;
    int i;
    struct cache_entry *ce;
    unsigned int least_accessed = 10000;
    int least_index = 0;
    int found = 0;
    tag = addr >> 2;
    fa->refs += 1;
    for (i = 0; i < fa->size; i++) {
        ce = &fa->cache_array[i];
        /* Determine if addr is in the cache or not.*/
        if (ce->valid && (ce->tag == tag)) {
            /* hit*/
            fa->hits += 1;
            ce->time_stamp += 1;
            found = 1;
            data = ce->data;
        }
        if (ce->time_stamp < least_accessed) {
            least_accessed = ce->time_stamp;
            least_index = i;
        }
    }
    if (found == 0) {
        /* miss */
        fa->misses += 1;
        ce = &fa->cache_array[least_index];
        ce->valid = 1;
        ce->tag = tag;
        ce->time_stamp = 1;
        ce->data = *((unsigned int *) addr);
        data = ce->data;
    }
    return data;
}
