/*
 * Copyright 2002-2020 Intel Corporation.
 * 
 * This software is provided to you as Sample Source Code as defined in the accompanying
 * End User License Agreement for the Intel(R) Software Development Products ("Agreement")
 * section 1.L.
 * 
 * This software and the related documents are provided as is, with no express or implied
 * warranties, other than those that are expressly stated in the License.
 */

/*! @file
 *  This file contains an ISA-portable cache simulator
 *  data cache hierarchies
 */


#include "pin.H"

#include <iostream>
#include <fstream>
#include <cassert>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cache.h"

using std::cerr;
using std::endl;
using std::string;

unsigned long long g_block_sizeL1;
unsigned long long g_setsL1;
unsigned long long g_set_bitsL1;
unsigned long long g_linesL1;
unsigned long long g_blocksL1;
unsigned long long g_block_bitsL1;
unsigned long long g_offMaskL1;
unsigned long long g_setMaskL1;
unsigned long long g_tag_bitsL1;
unsigned long long g_tagMaskL1;
int g_policyL1;
block_t **g_cacheL1;

unsigned long long g_block_sizeL2;
unsigned long long g_setsL2;
unsigned long long g_set_bitsL2;
unsigned long long g_linesL2;
unsigned long long g_blocksL2;
unsigned long long g_block_bitsL2;
unsigned long long g_offMaskL2;
unsigned long long g_setMaskL2;
unsigned long long g_tag_bitsL2;
unsigned long long g_tagMaskL2;
int g_policyL2;
block_t **g_cacheL2;


/* ===================================================================== */
/* Commandline Switches */
/* ===================================================================== */

KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE,    "pintool",
    "o", "memhier.stats", "specify dcache file name");
KNOB<UINT32> KnobL1CacheSize(KNOB_MODE_WRITEONCE, "pintool",
    "l1c","2", "cache size in kilobytes");
KNOB<UINT32> KnobL1LineSize(KNOB_MODE_WRITEONCE, "pintool",
    "l1b","64", "cache block size in bytes");
KNOB<UINT32> KnobL1Associativity(KNOB_MODE_WRITEONCE, "pintool",
    "l1a","2", "cache associativity (1 for direct mapped)");
KNOB<UINT32> KnobL1Policy(KNOB_MODE_WRITEONCE, "pintool",
    "l1p","2", "cache replacement policy");

KNOB<UINT32> KnobL2CacheSize(KNOB_MODE_WRITEONCE, "pintool",
    "l2c","2", "cache size in kilobytes");
KNOB<UINT32> KnobL2LineSize(KNOB_MODE_WRITEONCE, "pintool",
    "l2b","64", "cache block size in bytes");
KNOB<UINT32> KnobL2Associativity(KNOB_MODE_WRITEONCE, "pintool",
    "l2a","2", "cache associativity (1 for direct mapped)");
KNOB<UINT32> KnobL2Policy(KNOB_MODE_WRITEONCE, "pintool",
    "l2p","2", "cache replacement policy");

/* ===================================================================== */

INT32 Usage()
{
    cerr <<
        "This tool is a cache simulator.\n"
        "\n";

    cerr << KNOB_BASE::StringKnobSummary();

    cerr << endl;

    return -1;
}

/* ===================================================================== */

VOID Load(ADDRINT addr, UINT32 size, ADDRINT instAddr)
{
  mhLoad((void*)addr,size,(void*)instAddr);
}

/* ===================================================================== */

VOID Store(ADDRINT addr, UINT32 size, ADDRINT instAddr)
{
  mhStore((void*)addr,size,(void*)instAddr);
}


/* ===================================================================== */

VOID Instruction(INS ins, void * v)
{
    UINT32 memOperands = INS_MemoryOperandCount(ins);

    // Instrument each memory operand. If the operand is both read and written
    // it will be processed twice.
    // Iterating over memory operands ensures that instructions on IA-32 with
    // two read operands (such as SCAS and CMPS) are correctly handled.
    for (UINT32 memOp = 0; memOp < memOperands; memOp++)
    {
        const UINT32 size = INS_MemoryOperandSize(ins, memOp);
        
        if (INS_MemoryOperandIsRead(ins, memOp))
        {
                // map sparse INS addresses to dense IDs
          const ADDRINT iaddr = INS_Address(ins);

          INS_InsertPredicatedCall(
                        ins, IPOINT_BEFORE,  (AFUNPTR) Load,
                        IARG_MEMORYOP_EA, memOp,
                        IARG_UINT32, size,
                        IARG_ADDRINT, iaddr,
                        IARG_END);
        }
        
        if (INS_MemoryOperandIsWritten(ins, memOp))
        {
          const ADDRINT iaddr = INS_Address(ins);

          INS_InsertPredicatedCall(
                        ins, IPOINT_BEFORE,  (AFUNPTR) Store,
                        IARG_MEMORYOP_EA,memOp,
                        IARG_UINT32, size,
                        IARG_ADDRINT, iaddr,
                        IARG_END);
        }
    }
}

/* ===================================================================== */



int freeCacheL1() {
    for (int i = 0; i < (int)g_setsL1; ++i)
        free(g_cacheL1[i]);
    free(g_cacheL1);
    return 0;
}

int freeCacheL2() {
    for (int i = 0; i < (int)g_setsL2; ++i)
        free(g_cacheL2[i]);
    free(g_cacheL2);
    return 0;
}

VOID Fini(int code, VOID * v)
{

    std::ofstream out(KnobOutputFile.Value().c_str(), std::ios_base::app);

    //Output your memory hierarchy's stats here
    /*
    csim_stats_t *stats = malloc(sizeof(csim_stats_t));
    stats->hits = g_hits;
    stats->misses = g_misses;
    stats->evictions = g_evictions;
    for (int i = 0; i < g_sets; ++i) {
        for (int j = 0; j < g_lines; ++j) {
            if (g_cache[i][j].valid && g_cache[i][j].dirty)
                g_dirty_bytes += (unsigned long)g_blocks;
        }
    }
    stats->dirty_bytes = g_dirty_bytes;
    stats->dirty_evictions = (unsigned long)g_blocks * g_dirty_evictions;
    */
    out << KnobL1Policy.Value() << ", ";
    out << KnobL1CacheSize.Value() << ", ";
    out << KnobL1Associativity.Value() << ", ";
    out << g_setsL1 << ", ";
    out << g_hitsL1 << ", ";
    out << g_missesL1 << ", ";
    out << g_evictionsL1 << ", ";
    out << ((float)(g_missesL1)) / ((float)(g_missesL1+g_hitsL1)) << ", "; 
    out << g_errorL1 << ", ";
    out << KnobL2Policy.Value() << ", ";
    out << KnobL2CacheSize.Value() << ", ";
    out << KnobL2Associativity.Value() << ", ";
    out << g_setsL2 << ", ";
    out << g_hitsL2 << ", ";
    out << g_missesL2 << ", ";
    out << g_evictionsL2 << ", ";
    out << ((float)(g_missesL2)) / ((float)(g_missesL2+g_hitsL2)) << ", "; 
    out << g_errorL2 << std::endl;
        
    out.close();
    freeCacheL1();
    freeCacheL2();
}

/* ===================================================================== */




int main(int argc, char *argv[])
{
    PIN_InitSymbols();

    if( PIN_Init(argc,argv) )
    {
        return Usage();
    }

    g_set_bitsL1 = (int)(log((double)((KnobL1CacheSize.Value()/KnobL1LineSize.Value()) / KnobL1Associativity.Value()))/log((double)2));// (cachesize / linesize) / associativity
    g_setsL1 = (int)(((KnobL1CacheSize.Value()/KnobL1LineSize.Value()) / KnobL1Associativity.Value()));
    g_linesL1 = KnobL1Associativity.Value();// associativty
    g_block_bitsL1 = (int)(log((double)((KnobL1LineSize.Value())))/log((double)2));// (cachesize / linesize)
    g_block_sizeL1 = (KnobL1LineSize.Value());
    g_tag_bitsL1 = 64 - g_set_bitsL1 - g_block_bitsL1;
    g_policyL1 = (int)KnobL1Policy.Value();
    g_errorL1 = 0;

    g_set_bitsL2 = (int)(log((double)((KnobL2CacheSize.Value()/KnobL2LineSize.Value()) / KnobL2Associativity.Value()))/log((double)2));// (cachesize / linesize) / associativity
    g_setsL2 = (int)(((KnobL2CacheSize.Value()/KnobL2LineSize.Value()) / KnobL2Associativity.Value()));
    g_linesL2 = KnobL2Associativity.Value();// associativty
    g_block_bitsL2 = (int)(log((double)((KnobL2LineSize.Value())))/log((double)2));// (cachesize / linesize)
    g_block_sizeL2 = (KnobL2LineSize.Value());
    g_tag_bitsL2 = 64 - g_set_bitsL2 - g_block_bitsL2;
    g_policyL2 = (int)KnobL2Policy.Value();
    g_errorL2 = 0;


    // Pre-allocate cache:
    g_cacheL1 = (block_t **)malloc((g_setsL1) *
                                 sizeof(block_t *));
    for (int i = 0; i < (int)g_setsL1; ++i) {
        g_cacheL1[i] =
            (block_t *)malloc(((unsigned long long)g_linesL1) * sizeof(block_t));
    }
    // Initialize cache values
    for (int i = 0; i < (int)g_setsL1; ++i) {
        for (int j = 0; j < (int)g_linesL1; ++j) {
            g_cacheL1[i][j].valid = false;
            g_cacheL1[i][j].dirty = false;
            g_cacheL1[i][j].bitPLRU = false;
            g_cacheL1[i][j].lru = 0;
            g_cacheL1[i][j].set_b = 0;
            g_cacheL1[i][j].off_b = 0;
            g_cacheL1[i][j].tag_b = 0;
        }
    }

    g_offMaskL1 = g_block_sizeL1 - 1;
    g_setMaskL1 = g_setsL1 - 1;
    for (int i = 0; i< (int)g_tag_bitsL1; i++){
      g_tagMaskL1 |= 1 << i;
    }


    // Pre-allocate cache:
    g_cacheL2 = (block_t **)malloc((g_setsL2) *
                                 sizeof(block_t *));
    for (int i = 0; i < (int)g_setsL2; ++i) {
        g_cacheL2[i] =
            (block_t *)malloc(((unsigned long long)g_linesL2) * sizeof(block_t));
    }
    // Initialize cache values
    for (int i = 0; i < (int)g_setsL2; ++i) {
        for (int j = 0; j < (int)g_linesL2; ++j) {
            g_cacheL2[i][j].valid = false;
            g_cacheL2[i][j].dirty = false;
            g_cacheL2[i][j].bitPLRU = false;
            g_cacheL2[i][j].lru = 0;
            g_cacheL2[i][j].set_b = 0;
            g_cacheL2[i][j].off_b = 0;
            g_cacheL2[i][j].tag_b = 0;
        }
    }

    g_offMaskL2 = g_block_sizeL2 - 1;
    g_setMaskL2 = g_setsL2 - 1;
    for (int i = 0; i< (int)g_tag_bitsL2; i++){
      g_tagMaskL2 |= 1 << i;
    }







    INS_AddInstrumentFunction(Instruction, 0);
    PIN_AddFiniFunction(Fini, 0);

    // Never returns

    PIN_StartProgram();
    
    return 0;
}

/* ===================================================================== */
/* eof */
/* ===================================================================== */
