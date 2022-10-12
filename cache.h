void mhLoad (void *, size_t, void *);
void mhStore(void *, size_t, void *);
int incLRU(unsigned long long);
int cacheFunc(unsigned long long);




extern unsigned long g_hits;
extern unsigned long g_misses;
extern unsigned long g_evictions;
extern unsigned long g_dirty_evictions;

extern unsigned long long g_block_size;

extern unsigned long long g_sets;
extern unsigned long long g_set_bits;
extern unsigned long long g_lines;
extern unsigned long long g_blocks;
extern unsigned long long g_block_bits;
extern unsigned long long g_tag_bits;

extern bool g_save;

extern unsigned long long g_offMask;
extern unsigned long long g_setMask;
extern unsigned long long g_tagMask;

extern int g_policy;

typedef struct {
    unsigned long long set_b; /* set bits */
    unsigned long long tag_b; /* tag bits */
    unsigned long long off_b; /* offset bits */
    int lru;
    bool bitPLRU;
    bool valid; /* valid bit */
    bool dirty; /* dirty bit */
} block_t;

extern block_t **g_cache;