/* cache.h - Public data structures and functions from cache.c */
#ifndef CACHE_H
#define CACHE_H

#include "utils.h"

/* Cache simulation structs */
//full_associative
struct cache_entry {
    unsigned int valid;
    unsigned int tag;
    unsigned int data;
    /* The time_stamp can be used in fully associative and set associative
     * caches to approximate LRU.
     */
    unsigned int time_stamp;
};

struct cache_direct_mapped {
    struct cache_entry *cache_array;
    int size;
    int refs;
    int hits;
    int misses;
};

struct cache_state {
    struct cache_direct_mapped *dm_array;
    int dm_count;
    /* Add other cache types here */
};

//void cache_state_init(struct cache_state *cstate);
/* Initialize cache data structures */
void cache_state_init(struct cache_state *cstate) {
    struct cache_direct_mapped *dm_array;
    int i;
    int sizes[] = {8, 32, 128, 1024};

    dm_array = (struct cache_direct_mapped *)
            malloc_helper(sizeof(struct cache_direct_mapped) * 4,
                          "cache_state_init() : cache_direct_mapped");

    /* This only initializes the first direct mapped cache.
     * You will need to initialize the other direct mapped caches
     * and the other cache types.
     */
    cstate->dm_count = 4;
    for (i = 0; i < cstate->dm_count; i++) {
        cache_direct_mapped_init(&dm_array[i], sizes[i]);
    }
    cstate->dm_array = dm_array;
}

void cache_state_free(struct cache_state *cstate);

void cache_direct_mapped_stats(struct cache_direct_mapped *dm) {
    printf("Stats for Direct Mapped Cache of size %d\n", dm->size);
    printf("refs = %d\n", dm->refs);
    printf("hits = %d\n", dm->hits);
    printf("misses = %d\n", dm->misses);
}

void cache_state_stats(struct cache_state *cstate) {
    for (int i = 0; i < 4; i++)
        cache_direct_mapped_stats(&cstate->dm_array[i]);
}

unsigned int cache_lookup(struct cache_state *cstate, unsigned int addr);

#endif /* CACHE_H */