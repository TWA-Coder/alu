#ifndef MINING_H
#define MINING_H

#include "blockchain.h"
#include "mempool.h"

// Initialize mining state, sets current_difficulty etc.
void init_mining_engine(ChainState *state);

// Attempts to solo mine a block with transactions from the mempool.
// Will pick top pending transactions up to MAX_TRANSACTIONS_PER_BLOCK.
// Returns a heap allocated Block on success, NULL if nothing to mine.
Block* mine_solo(const char *miner_id);

// Simulates a mining pool collaborating. Distributes rewards.
Block* mine_pool(const char *pool_id, int miners, double *hash_contributions);

// Computes if a hash meets the current difficulty target (leading zeros)
int meets_difficulty(const char *hash_hex, uint32_t difficulty);

// Adjusts the difficulty every 10 blocks (handled internally when blocks are added)
void retarget_difficulty(Block *last_10_blocks, ChainState *state);

#endif
