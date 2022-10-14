void mhLoad (void *, size_t, void *);
void mhStore(void *, size_t, void *);
int incLRUL1(unsigned long long);
int incLRUL2(unsigned long long);
int clearPLRUSL1(unsigned long long);
int clearPLRUSL2(unsigned long long);
int cacheFuncL1(unsigned long long);
int cacheFuncL2(unsigned long long);

extern unsigned long g_hitsL1;
extern unsigned long g_missesL1;
extern unsigned long g_evictionsL1;
extern unsigned long g_dirty_evictionsL1;

extern unsigned long long g_block_sizeL1;

extern unsigned long long g_setsL1;
extern unsigned long long g_set_bitsL1;
extern unsigned long long g_linesL1;
extern unsigned long long g_blocksL1;
extern unsigned long long g_block_bitsL1;
extern unsigned long long g_tag_bitsL1;

extern bool g_saveL1;

extern unsigned long long g_offMaskL1;
extern unsigned long long g_setMaskL1;
extern unsigned long long g_tagMaskL1;

extern int g_policyL1;
extern int g_errorL1;

typedef struct {
    unsigned long long set_b; /* set bits */
    unsigned long long tag_b; /* tag bits */
    unsigned long long off_b; /* offset bits */
    int lru;
    bool bitPLRU;
    bool valid; /* valid bit */
    bool dirty; /* dirty bit */
} block_t;

extern block_t **g_cacheL1;

extern unsigned long g_hitsL2;
extern unsigned long g_missesL2;
extern unsigned long g_evictionsL2;
extern unsigned long g_dirty_evictionsL2;

extern unsigned long long g_block_sizeL2;

extern unsigned long long g_setsL2;
extern unsigned long long g_set_bitsL2;
extern unsigned long long g_linesL2;
extern unsigned long long g_blocksL2;
extern unsigned long long g_block_bitsL2;
extern unsigned long long g_tag_bitsL2;

extern bool g_saveL2;

extern unsigned long long g_offMaskL2;
extern unsigned long long g_setMaskL2;
extern unsigned long long g_tagMaskL2;

extern int g_policyL2;
extern int g_errorL2;

extern block_t **g_cacheL2;