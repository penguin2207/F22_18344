#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cache.h"


unsigned long g_hitsL1;
unsigned long g_missesL1;
unsigned long g_evictionsL1;
unsigned long g_dirty_evictionsL1;
bool g_saveL1;
int g_errorL1;


unsigned long g_hitsL2;
unsigned long g_missesL2;
unsigned long g_evictionsL2;
unsigned long g_dirty_evictionsL2;
bool g_saveL2;
int g_errorL2;


// Function to increment lru
int incLRUL1(unsigned long long sb) {
    for (int i = 0; i < (int)g_linesL1; ++i) {
        if (g_cacheL1[sb][i].valid)
            g_cacheL1[sb][i].lru += 1;
    }
    return 0;
}

int clearPLRUSL1(unsigned long long sb){
  for (int i = 0; i < (int)g_linesL1; ++i) {
            g_cacheL1[sb][i].bitPLRU = false;
  }
  return 0;
}

int incLRUL2(unsigned long long sb) {
    for (int i = 0; i < (int)g_linesL2; ++i) {
        if (g_cacheL2[sb][i].valid)
            g_cacheL2[sb][i].lru += 1;
    }
    return 0;
}

int clearPLRUSL2(unsigned long long sb){
  for (int i = 0; i < (int)g_linesL2; ++i) {
            g_cacheL2[sb][i].bitPLRU = false;
  }
  return 0;
}


void mhLoad(void *addr, size_t size, void *instAddr){
  g_saveL1 = false;
  if(cacheFuncL1((unsigned long long)addr) == -1) g_errorL1++;
}

void mhStore(void *addr, size_t size, void *instAddr){
  g_saveL1 = true;
  if(cacheFuncL1((unsigned long long)addr) == -1) g_errorL1++;
}


// Policy Key: 0 -> Random, 1 -> LRU, 2 -> bit-pLRU
int cacheFuncL1(unsigned long long data) {
    // Get set bits, offset bits, tag bits
    unsigned long long tagBits =
        ((data & (~((unsigned long long)((g_setMaskL1 << g_block_bitsL1) | g_offMaskL1)))) >>
        (g_block_bitsL1 + g_set_bitsL1)) & (unsigned long long)(g_tagMaskL1);
    unsigned long long setBits = (data & (g_setMaskL1 << g_block_bitsL1)) >> g_block_bitsL1;
    unsigned long long offBits = data & g_offMaskL1;
    // Initialize index of lru block and empty block
    int lruInd = 0;
    int plruInd = -1;
    int emptyBlockInd = -1;
    int plrus = 0;
    // Check for hit and find lru (since both can be found in one traversal)
    for (int i = 0; i < (int)g_linesL1; ++i) {
        if (g_cacheL1[setBits][i].valid &&
            (g_cacheL1[setBits][i].tag_b == tagBits)) {
            // Check if saving or loading to set dirty bit
            if (g_saveL1)
                g_cacheL1[setBits][i].dirty = true;
            // Since hit, want to retain this block as most used
            if(g_policyL1 == 1){
              incLRUL1(setBits);
              g_cacheL1[setBits][i].lru = 1;
            }
            if(g_policyL1 == 2){
              g_cacheL1[setBits][i].bitPLRU = true;
            }
            g_hitsL1 += 1;
            return 1;
        }
        // Find empty or lru block in case no hit
        if(g_policyL1 == 0){}
        else if(g_policyL1 == 1){
          if (g_cacheL1[setBits][i].valid) {
              if (g_cacheL1[setBits][i].lru >= (g_cacheL1[setBits][lruInd].lru))
                  lruInd = i;
          } else
              emptyBlockInd = i;
        }
        else if (g_policyL1 == 2){
              if (!g_cacheL1[setBits][i].bitPLRU) plruInd = i;
              else plrus += 1;
        } else return -1;
    }
    if(g_policyL1 == 0){
      int randLine = rand() % g_linesL1;
      if(g_cacheL1[setBits][randLine].valid){
        if(cacheFuncL2(data) == 1){
          if (g_cacheL1[setBits][randLine].dirty)
            g_dirty_evictionsL1 += 1;
          g_missesL1 += 1;
          g_evictionsL1 += 1;
          return 3;
        }
        if (g_cacheL1[setBits][randLine].dirty)
          g_dirty_evictionsL1 += 1;
        g_cacheL1[setBits][randLine].set_b = setBits;
        g_cacheL1[setBits][randLine].tag_b = tagBits;
        g_cacheL1[setBits][randLine].off_b = offBits;
        g_cacheL1[setBits][randLine].valid = true;
        g_cacheL1[setBits][randLine].dirty = g_saveL1;
        g_missesL1 += 1;
        g_evictionsL1 += 1;
        return 3;
      }else{
        g_cacheL1[setBits][randLine].set_b = setBits;
        g_cacheL1[setBits][randLine].tag_b = tagBits;
        g_cacheL1[setBits][randLine].off_b = offBits;
        g_cacheL1[setBits][randLine].valid = true;
        g_cacheL1[setBits][randLine].dirty = g_saveL1;
        g_missesL1 += 1;
        return 2;
      }
    }
    else if(g_policyL1 == 1){
      // Increment lru if no hit so new add will be most recently used
      incLRUL1(setBits);
      // Write into empty or lru block
      if (emptyBlockInd != -1) {
          // If empty block somewhere
          g_cacheL1[setBits][emptyBlockInd].set_b = setBits;
          g_cacheL1[setBits][emptyBlockInd].tag_b = tagBits;
          g_cacheL1[setBits][emptyBlockInd].off_b = offBits;
          g_cacheL1[setBits][emptyBlockInd].lru = 1;
          g_cacheL1[setBits][emptyBlockInd].valid = true;
          g_cacheL1[setBits][emptyBlockInd].dirty = g_saveL1;
          g_missesL1 += 1;
          return 2;
      } else {
          // If no empty blocks use lru to evict
          if (g_cacheL1[setBits][lruInd].valid) {
              if(cacheFuncL2(data) == 1){
                if (g_cacheL1[setBits][lruInd].dirty)
                  g_dirty_evictionsL1 += 1;
                g_missesL1 += 1;
                g_evictionsL1 += 1;
                return 3;
              }
              if (g_cacheL1[setBits][lruInd].dirty)
                g_dirty_evictionsL1 += 1;
              g_cacheL1[setBits][lruInd].set_b = setBits;
              g_cacheL1[setBits][lruInd].tag_b = tagBits;
              g_cacheL1[setBits][lruInd].off_b = offBits;
              g_cacheL1[setBits][lruInd].lru = 1;
              g_cacheL1[setBits][lruInd].valid = true;
              g_cacheL1[setBits][lruInd].dirty = g_saveL1;
              g_missesL1 += 1;
              g_evictionsL1 += 1;
              return 3;
          } else
              return -1;
      }
    } else if (g_policyL1 == 2){
          if(plrus == (int)g_linesL1){
            clearPLRUSL1(setBits);
            plruInd = 0;
          }
          // If no empty blocks use lru to evict
          if (plruInd != -1) {
              if(g_cacheL1[setBits][plruInd].valid){
                if(cacheFuncL2(data) == 1){
                  if (g_cacheL1[setBits][lruInd].dirty)
                    g_dirty_evictionsL1 += 1;
                  g_missesL1 += 1;
                  g_evictionsL1 += 1;
                  return 3;
                }
                g_evictionsL1 += 1;
              }
              if (g_cacheL1[setBits][plruInd].valid && g_cacheL1[setBits][plruInd].dirty)
                g_dirty_evictionsL1 += 1;
              g_cacheL1[setBits][plruInd].set_b = setBits;
              g_cacheL1[setBits][plruInd].tag_b = tagBits;
              g_cacheL1[setBits][plruInd].off_b = offBits;
              g_cacheL1[setBits][plruInd].bitPLRU = true;
              g_cacheL1[setBits][plruInd].valid = true;
              g_cacheL1[setBits][plruInd].dirty = g_saveL1;
              g_missesL1 += 1;
              return 3;
          } else
              return -1;
    }
    return -1;
}

int cacheFuncL2(unsigned long long data) {
    // Get set bits, offset bits, tag bits
    unsigned long long tagBits =
        ((data & (~((unsigned long long)((g_setMaskL2 << g_block_bitsL2) | g_offMaskL2)))) >>
        (g_block_bitsL2 + g_set_bitsL2)) & (unsigned long long)(g_tagMaskL2);
    unsigned long long setBits = (data & (g_setMaskL2 << g_block_bitsL2)) >> g_block_bitsL2;
    unsigned long long offBits = data & g_offMaskL2;
    // Initialize index of lru block and empty block
    int lruInd = 0;
    int plruInd = -1;
    int emptyBlockInd = -1;
    int plrus = 0;
    // Check for hit and find lru (since both can be found in one traversal)
    for (int i = 0; i < (int)g_linesL2; ++i) {
        if (g_cacheL2[setBits][i].valid &&
            (g_cacheL2[setBits][i].tag_b == tagBits)) {
            // Check if saving or loading to set dirty bit
            if (g_saveL2)
                g_cacheL2[setBits][i].dirty = true;
            // Since hit, want to retain this block as most used
            if(g_policyL2 == 1){
              incLRUL2(setBits);
              g_cacheL2[setBits][i].lru = 1;
            }
            if(g_policyL2 == 2){
              g_cacheL2[setBits][i].bitPLRU = true;
            }
            g_hitsL2 += 1;
            return 1;
        }
        // Find empty or lru block in case no hit
        if(g_policyL2 == 0){}
        else if(g_policyL2 == 1){
          if (g_cacheL2[setBits][i].valid) {
              if (g_cacheL2[setBits][i].lru >= (g_cacheL2[setBits][lruInd].lru))
                  lruInd = i;
          } else
              emptyBlockInd = i;
        }
        else if (g_policyL2 == 2){
              if (!g_cacheL2[setBits][i].bitPLRU) plruInd = i;
              else plrus += 1;
        } else return -1;
    }
    if(g_policyL2 == 0){
      int randLine = rand() % g_linesL2;
      if(g_cacheL2[setBits][randLine].valid){
        // IDEALLY WOULD SAVE OVERWRITTEN DATA NOW INTO LOWER MEM
        // Instead just replace with load request as though reading from
        // lower mem
        if (g_cacheL2[setBits][randLine].dirty)
          g_dirty_evictionsL2 += 1;
        g_cacheL2[setBits][randLine].set_b = setBits;
        g_cacheL2[setBits][randLine].tag_b = tagBits;
        g_cacheL2[setBits][randLine].off_b = offBits;
        g_cacheL2[setBits][randLine].valid = true;
        g_cacheL2[setBits][randLine].dirty = g_saveL2;
        g_missesL2 += 1;
        g_evictionsL2 += 1;
        return 3;
      }else{
        g_cacheL2[setBits][randLine].set_b = setBits;
        g_cacheL2[setBits][randLine].tag_b = tagBits;
        g_cacheL2[setBits][randLine].off_b = offBits;
        g_cacheL2[setBits][randLine].valid = true;
        g_cacheL2[setBits][randLine].dirty = g_saveL2;
        g_missesL2 += 1;
        return 2;
      }
    }
    else if(g_policyL2 == 1){
      // Increment lru if no hit so new add will be most recently used
      incLRUL2(setBits);
      // Write into empty or lru block
      if (emptyBlockInd != -1) {
          // If empty block somewhere
          g_cacheL2[setBits][emptyBlockInd].set_b = setBits;
          g_cacheL2[setBits][emptyBlockInd].tag_b = tagBits;
          g_cacheL2[setBits][emptyBlockInd].off_b = offBits;
          g_cacheL2[setBits][emptyBlockInd].lru = 1;
          g_cacheL2[setBits][emptyBlockInd].valid = true;
          g_cacheL2[setBits][emptyBlockInd].dirty = g_saveL2;
          g_missesL2 += 1;
          return 2;
      } else {
          // If no empty blocks use lru to evict
          if (g_cacheL2[setBits][lruInd].valid) {
              // IDEALLY WOULD SAVE OVERWRITTEN DATA NOW INTO LOWER MEM
              // Instead just replace with load request as though reading from
              // lower mem
              if (g_cacheL2[setBits][lruInd].dirty)
                g_dirty_evictionsL2 += 1;
              g_cacheL2[setBits][lruInd].set_b = setBits;
              g_cacheL2[setBits][lruInd].tag_b = tagBits;
              g_cacheL2[setBits][lruInd].off_b = offBits;
              g_cacheL2[setBits][lruInd].lru = 1;
              g_cacheL2[setBits][lruInd].valid = true;
              g_cacheL2[setBits][lruInd].dirty = g_saveL2;
              g_missesL2 += 1;
              g_evictionsL2 += 1;
              return 3;
          } else
              return -1;
      }
    } else if (g_policyL2 == 2){
          if(plrus == (int)g_linesL2){
            clearPLRUSL2(setBits);
            plruInd = 0;
          }
          // If no empty blocks use lru to evict
          if (plruInd != -1) {
              if(g_cacheL2[setBits][plruInd].valid){
                g_evictionsL2 += 1;
              }
              // IDEALLY WOULD SAVE OVERWRITTEN DATA NOW INTO LOWER MEM
              // Instead just replace with load request as though reading from
              // lower mem
              if (g_cacheL2[setBits][plruInd].valid && g_cacheL2[setBits][plruInd].dirty)
                g_dirty_evictionsL2 += 1;
              g_cacheL2[setBits][plruInd].set_b = setBits;
              g_cacheL2[setBits][plruInd].tag_b = tagBits;
              g_cacheL2[setBits][plruInd].off_b = offBits;
              g_cacheL2[setBits][plruInd].bitPLRU = true;
              g_cacheL2[setBits][plruInd].valid = true;
              g_cacheL2[setBits][plruInd].dirty = g_saveL2;
              g_missesL2 += 1;
              return 3;
          } else
              return -1;
    }
    return -1;
}
