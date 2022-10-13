#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cache.h"


unsigned long g_hits;
unsigned long g_misses;
unsigned long g_evictions;
unsigned long g_dirty_evictions;
bool g_save;
int g_error;


// Function to increment lru
int incLRU(unsigned long long sb) {
    for (int i = 0; i < (int)g_lines; ++i) {
        if (g_cache[sb][i].valid)
            g_cache[sb][i].lru += 1;
    }
    return 0;
}

int clearPLRUS(unsigned long long sb){
  for (int i = 0; i < (int)g_lines; ++i) {
            g_cache[sb][i].bitPLRU = false;
  }
  return 0;
}



void mhLoad(void *addr, size_t size, void *instAddr){
  g_save = false;
  if(cacheFunc((unsigned long long)addr) == -1) g_error++;
}

void mhStore(void *addr, size_t size, void *instAddr){
  g_save = true;
  if(cacheFunc((unsigned long long)addr) == -1) g_error++;
}


// Policy Key: 0 -> Random, 1 -> LRU, 2 -> bit-pLRU
int cacheFunc(unsigned long long data) {
    // Get set bits, offset bits, tag bits
    unsigned long long tagBits =
        ((data & (~((unsigned long long)((g_setMask << g_block_bits) | g_offMask)))) >>
        (g_block_bits + g_set_bits)) & (unsigned long long)(g_tagMask);
    unsigned long long setBits = (data & (g_setMask << g_block_bits)) >> g_block_bits;
    unsigned long long offBits = data & g_offMask;
    // Initialize index of lru block and empty block
    int lruInd = 0;
    int plruInd = -1;
    int emptyBlockInd = -1;
    int plrus = 0;
    // Check for hit and find lru (since both can be found in one traversal)
    for (int i = 0; i < (int)g_lines; ++i) {
        if (g_cache[setBits][i].valid &&
            (g_cache[setBits][i].tag_b == tagBits)) {
            // Check if saving or loading to set dirty bit
            if (g_save)
                g_cache[setBits][i].dirty = true;
            // Since hit, want to retain this block as most used
            if(g_policy == 1){
              incLRU(setBits);
              g_cache[setBits][i].lru = 1;
            }
            if(g_policy == 2){
              g_cache[setBits][i].bitPLRU = true;
            }
            g_hits += 1;
            return 1;
        }
        // Find empty or lru block in case no hit
        if(g_policy == 0){}
        else if(g_policy == 1){
          if (g_cache[setBits][i].valid) {
              if (g_cache[setBits][i].lru >= (g_cache[setBits][lruInd].lru))
                  lruInd = i;
          } else
              emptyBlockInd = i;
        }
        else if (g_policy == 2){
              if (!g_cache[setBits][i].bitPLRU) plruInd = i;
              else plrus += 1;
        } else return -1;
    }
    if(g_policy == 0){
      int randLine = rand() % g_lines;
      if(g_cache[setBits][randLine].valid){
        // IDEALLY WOULD SAVE OVERWRITTEN DATA NOW INTO LOWER MEM
        // Instead just replace with load request as though reading from
        // lower mem
        if (g_cache[setBits][randLine].dirty)
          g_dirty_evictions += 1;
        g_cache[setBits][randLine].set_b = setBits;
        g_cache[setBits][randLine].tag_b = tagBits;
        g_cache[setBits][randLine].off_b = offBits;
        g_cache[setBits][randLine].valid = true;
        g_cache[setBits][randLine].dirty = g_save;
        g_misses += 1;
        g_evictions += 1;
        return 3;
      }else{
        g_cache[setBits][randLine].set_b = setBits;
        g_cache[setBits][randLine].tag_b = tagBits;
        g_cache[setBits][randLine].off_b = offBits;
        g_cache[setBits][randLine].valid = true;
        g_cache[setBits][randLine].dirty = g_save;
        g_misses += 1;
        return 2;
      }
    }
    else if(g_policy == 1){
      // Increment lru if no hit so new add will be most recently used
      incLRU(setBits);
      // Write into empty or lru block
      if (emptyBlockInd != -1) {
          // If empty block somewhere
          g_cache[setBits][emptyBlockInd].set_b = setBits;
          g_cache[setBits][emptyBlockInd].tag_b = tagBits;
          g_cache[setBits][emptyBlockInd].off_b = offBits;
          g_cache[setBits][emptyBlockInd].lru = 1;
          g_cache[setBits][emptyBlockInd].valid = true;
          g_cache[setBits][emptyBlockInd].dirty = g_save;
          g_misses += 1;
          return 2;
      } else {
          // If no empty blocks use lru to evict
          if (g_cache[setBits][lruInd].valid) {
              // IDEALLY WOULD SAVE OVERWRITTEN DATA NOW INTO LOWER MEM
              // Instead just replace with load request as though reading from
              // lower mem
              if (g_cache[setBits][lruInd].dirty)
                g_dirty_evictions += 1;
              g_cache[setBits][lruInd].set_b = setBits;
              g_cache[setBits][lruInd].tag_b = tagBits;
              g_cache[setBits][lruInd].off_b = offBits;
              g_cache[setBits][lruInd].lru = 1;
              g_cache[setBits][lruInd].valid = true;
              g_cache[setBits][lruInd].dirty = g_save;
              g_misses += 1;
              g_evictions += 1;
              return 3;
          } else
              return -1;
      }
    } else if (g_policy == 2){
          if(plrus == (int)g_lines){
            clearPLRUS(setBits);
            plruInd = 0;
          }
          // If no empty blocks use lru to evict
          if (plruInd != -1) {
              if(g_cache[setBits][plruInd].valid){}
              // IDEALLY WOULD SAVE OVERWRITTEN DATA NOW INTO LOWER MEM
              // Instead just replace with load request as though reading from
              // lower mem
              if (g_cache[setBits][plruInd].valid && g_cache[setBits][plruInd].dirty)
                g_dirty_evictions += 1;
              g_cache[setBits][plruInd].set_b = setBits;
              g_cache[setBits][plruInd].tag_b = tagBits;
              g_cache[setBits][plruInd].off_b = offBits;
              g_cache[setBits][plruInd].bitPLRU = true;
              g_cache[setBits][plruInd].valid = true;
              g_cache[setBits][plruInd].dirty = g_save;
              g_misses += 1;
              g_evictions += 1;
              return 3;
          } else
              return -1;
    }
    return -1;
}

