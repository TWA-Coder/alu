#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "transaction.h"

#define MAX_TRANSACTIONS_PER_BLOCK 100

// Define Block structure as per requirements
typedef struct {
    char block_id[SHA256_HEX_LENGTH]; // the hash of the block
    time_t timestamp;
    uint32_t transaction_count;
    char previous_hash[SHA256_HEX_LENGTH];
    char merkle_root[SHA256_HEX_LENGTH];
    uint32_t nonce;
    char miner_id[135];
    uint32_t difficulty;
    
    // The transactions are stored directly in the block (or can be pointers, but array simplifies persistence)
    Transaction transactions[MAX_TRANSACTIONS_PER_BLOCK];
} Block;

// Chain state
typedef struct {
    uint32_t current_difficulty;
    uint32_t block_reward;
    uint32_t last_retarget_block;
} ChainState;

// Computes the Merkle Root for an array of transactions from scratch using SHA-256
void compute_merkle_root(Transaction *txs, uint32_t count, char *merkle_root_hex);

// Helper to hash transaction fields for Merkle Tree leaves and signing
void hash_transaction_fields(const Transaction *tx, unsigned char *hash_out);

#endif // BLOCKCHAIN_H
