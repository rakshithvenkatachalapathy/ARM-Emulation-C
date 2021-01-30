#include "stdio.h"
#include "cache_sa.h"
#include "stdlib.h"

void cache_set_associative_init(struct cache_set_associative *sa, int size) {
    struct cache_entry **cache_array;
    int i;
    cache_array = (struct cache_entry **) \
              malloc_helper(sizeof(struct cache_entry *) * size, "cache_set_associative_init() : cache_entry");
    for (i = 0; i < size / 4; i++) {
        cache_array[i] = (struct cache_entry *) \
                 malloc_helper(4 * sizeof(struct cache_entry), "cache_set_associative_init(): cache_entry");
    }
    sa->cache_array = cache_array;
    sa->size = size;
    sa->refs = 0;
    sa->hits = 0;
    sa->misses = 0;
}

void cache_set_associative_stats(struct cache_set_associative *sa) {
    printf("sa(%d) ", sa->size);
    printf("refs: %d ", sa->refs);
    printf("hits: %d ", sa->hits);
    printf("misses: %d ", sa->misses);
    printf("hit-ratio: %f ", (float) sa->hits / sa->refs);
    printf("miss-ratio: %f\n", (float) sa->misses / sa->refs);
}

/* lookup for set associative cache*/
unsigned int cache_lookup_set_associative(struct cache_set_associative *sa, unsigned int addr) {
    unsigned int tag, data;
    int i, index;
    struct cache_entry *ce;
    unsigned int least_accessed = 10000;
    int least_index = 0;
    int found = 0, set_size = 4;
    unsigned int index_bits = 0, indicies, temp;
    unsigned int sa_size = sa->size;
    indicies = sa_size / set_size;
    temp = indicies;
    sa->refs += 1;
    while (temp > 0) {
        index_bits += 1;
        temp >>= 1;
    }
    index = (addr >> 2) % sa_size / 4; //Get the inedx
    tag = addr >> (index_bits); // get the tag
    tag = addr >> 2; // get the tag

    for (i = 0; i < 4; i++) {
        ce = &sa->cache_array[index][i];
        /* Determine if addr is in the cache or not.*/
        if (ce->valid && (ce->tag == tag)) {
            /* hit*/
            sa->hits += 1;
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
        sa->misses += 1;
        ce = &sa->cache_array[index][least_index]; //acesses check
        ce->valid = 1;
        ce->tag = tag;
        ce->time_stamp = 1;
        ce->data = *((unsigned int *) addr);
        data = ce->data;
    }
    return data;
}
