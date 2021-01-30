#include <stdio.h>
#include <stdlib.h>
#include "cache.h"
#include "utils.h"
/* Initialize cache_direct_mapped */
void cache_direct_mapped_init(struct cache_direct_mapped *dm, int size)
{
    struct cache_entry *cache_array;
    /* Allocte size number of cache entries for cache_array */
    cache_array = (struct cache_entry *) \
	malloc_helper(sizeof(struct cache_entry) * size,
                  "cache_direct_mapped_init() : cache_entry");
    dm->cache_array = cache_array;
    dm->size = size;
    dm->refs = 0;
    dm->hits = 0;
    dm->misses = 0;
}


void cache_direct_mapped_stats(struct cache_direct_mapped *dm)
{
    printf("Stats for Direct Mapped Cache of size %d\n", dm->size);
    printf("refs = %d\n", dm->refs);
    printf("hits = %d\n", dm->hits);
    printf("misses = %d\n", dm->misses);
}
/* Cache Direct Mapped Lookup */
unsigned int cache_lookup_direct_mapped(struct cache_direct_mapped *dm,
                                        unsigned int addr)
{
    unsigned int tag;
    unsigned int index;
    /* The index_mask is used extract the index number from the addr */
    unsigned int index_mask;
    /* The index_bits is the number of bits need for the index, this is
    * used to extract the tag from the addr.
    */
    unsigned int index_bits = 0;
    unsigned int dm_size = dm->size;
    struct cache_entry *ce;
    /* Extract the index */
    index_mask = (dm->size) - 1;
    index = (addr >> 2) & index_mask;
    /* Simple log_2(n) calculation */
    while (dm_size > 1) {
        index_bits += 1;
        dm_size >>= 1;
    }
    /* Extract the tag */
    tag = addr >> (index_bits + 2);
    /* Lookup the cache_entry */
    ce = &dm->cache_array[index];
    /* Determine if addr is in the cache or not. */
    dm->refs += 1;
    if (ce->valid && (ce->tag == tag)) {
        /* hit */
        dm->hits += 1;
    } else {
        /* miss */
        dm->misses += 1;
        ce->valid = 1;
        ce->tag = tag;
        ce->data = *((unsigned int *) addr);
    }
    return ce->data;
}
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
    cstate->dm_count = 1;
    for (i = 0; i < cstate->dm_count; i++) {
        cache_direct_mapped_init(&dm_array[i], sizes[i]);
    }
    cstate->dm_array = dm_array;
}
/* Cache deallocation */
void cache_state_free(struct cache_state *cstate) {
    int i;

    free(cstate->dm_array);
}
/* Cache State Statistics */
void cache_state_stats(struct cache_state *cstate) {
    /* This just prints the stats for the first direct mapped cache.
    * You will need to add more code here.
    */
    cache_direct_mapped_stats(&cstate->dm_array[0]);
}
/* Cache Lookup */
unsigned int cache_lookup(struct cache_state *cstate, unsigned int addr)
{
    unsigned int data;
    /* This just does a lookup on the first direct mapped cache.
    * You will need to add lookups for the other direct mapped caches
    * and the other cache types */
    data = cache_lookup_direct_mapped(&cstate->dm_array[0], addr);
    /* Put sanity check here */
    return data;
}
