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

unsigned long long g_block_size;
unsigned long long g_sets;
unsigned long long g_set_bits;
unsigned long long g_lines;
unsigned long long g_blocks;
unsigned long long g_block_bits;
unsigned long long g_offMask;
unsigned long long g_setMask;
unsigned long long g_tag_bits;
unsigned long long g_tagMask;
int g_policy;
block_t **g_cache;

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
    "pol","2", "cache replacement policy");

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



int freeCache() {
    for (int i = 0; i < (int)g_sets; ++i)
        free(g_cache[i]);
    free(g_cache);
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
    out << g_hits << ", ";
    out << g_misses << ", ";
    out << g_evictions << ", ";
    out << ((float)(g_misses)) / ((float)(g_misses+g_hits)) << std::endl;
        
    out.close();
    freeCache();
}

/* ===================================================================== */




int main(int argc, char *argv[])
{
    PIN_InitSymbols();

    if( PIN_Init(argc,argv) )
    {
        return Usage();
    }

    g_set_bits = (int)(log((double)((KnobL1CacheSize.Value()/KnobL1LineSize.Value()) / KnobL1Associativity.Value()))/log((double)2));// (cachesize / linesize) / associativity
    g_sets = (int)(((KnobL1CacheSize.Value()/KnobL1LineSize.Value()) / KnobL1Associativity.Value()));
    g_lines = KnobL1Associativity.Value();// associativty
    g_block_bits = (int)(log((double)((KnobL1LineSize.Value())))/log((double)2));// (cachesize / linesize)
    g_block_size = (KnobL1LineSize.Value());
    g_tag_bits = 64 - g_set_bits - g_block_bits;
    g_policy = (int)KnobL1Policy.Value();


    // Pre-allocate cache:
    g_cache = (block_t **)malloc((g_sets) *
                                 sizeof(block_t *));
    for (int i = 0; i < (int)g_sets; ++i) {
        g_cache[i] =
            (block_t *)malloc(((unsigned long long)g_lines) * sizeof(block_t));
    }
    // Initialize cache values
    for (int i = 0; i < (int)g_sets; ++i) {
        for (int j = 0; j < (int)g_lines; ++j) {
            g_cache[i][j].valid = false;
            g_cache[i][j].dirty = false;
            g_cache[i][j].bitPLRU = false;
            g_cache[i][j].lru = 0;
            g_cache[i][j].set_b = 0;
            g_cache[i][j].off_b = 0;
            g_cache[i][j].tag_b = 0;
        }
    }

    g_offMask = g_block_size - 1;
    g_setMask = g_sets - 1;
    for (int i = 0; i< (int)g_tag_bits; i++){
      g_tagMask |= 1 << i;
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
