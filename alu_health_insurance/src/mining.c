#include "mining.h"
#include "account.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

static ChainState *chain_state = NULL;

void init_mining_engine(ChainState *state) {
    chain_state = state;
    if (state->current_difficulty == 0) {
        state->current_difficulty = 2; // Default starting difficulty
    }
    state->block_reward = 50; // AHT
}

int meets_difficulty(const char *hash_hex, uint32_t difficulty) {
    for (uint32_t i = 0; i < difficulty; i++) {
        if (hash_hex[i] != '0') {
            return 0;
        }
    }
    return 1;
}

Block* mine_solo(const char *miner_id) {
    if (!chain_state) return NULL;
    
    // 1. Sort mempool
    sort_mempool_priority();
    
    // 2. Select top priority transactions
    int m_count = 0;
    MempoolEntry *entries = get_mempool_entries(&m_count);
    
    Block *new_block = malloc(sizeof(Block));
    memset(new_block, 0, sizeof(Block));
    new_block->transaction_count = 0;
    
    for (int i = 0; i < m_count && new_block->transaction_count < MAX_TRANSACTIONS_PER_BLOCK; i++) {
        if (entries[i].status == STATUS_PENDING) {
            new_block->transactions[new_block->transaction_count] = entries[i].tx;
            new_block->transaction_count++;
        }
    }
    
    if (new_block->transaction_count == 0) {
        free(new_block);
        return NULL; // Nothing to mine
    }
    
    // 3. Setup block header
    new_block->timestamp = time(NULL);
    new_block->difficulty = chain_state->current_difficulty;
    strcpy(new_block->miner_id, miner_id);
    
    memset(new_block->previous_hash, '0', SHA256_HEX_LENGTH - 1);
    new_block->previous_hash[SHA256_HEX_LENGTH - 1] = '\0';
    
    // 4. Compute Merkle Root
    compute_merkle_root(new_block->transactions, new_block->transaction_count, new_block->merkle_root);
    
    // 5. Proof of Work loop
    char block_buffer[4096];
    char hash_hex[SHA256_HEX_LENGTH];
    
    new_block->nonce = 0;
    while (1) {
        int len = snprintf(block_buffer, sizeof(block_buffer), "%llu%u%s%s%u%s%u",
                           (unsigned long long)new_block->timestamp,
                           new_block->transaction_count,
                           new_block->previous_hash,
                           new_block->merkle_root,
                           new_block->nonce,
                           new_block->miner_id,
                           new_block->difficulty);
                           
        compute_sha256_hex((unsigned char*)block_buffer, len, hash_hex);
        
        if (meets_difficulty(hash_hex, new_block->difficulty)) {
            strcpy(new_block->block_id, hash_hex);
            break;
        }
        new_block->nonce++;
    }
    
    // Reward the miner
    Account* miner_acc = get_account(miner_id);
    if (!miner_acc) {
        miner_acc = create_account(miner_id, 0);
    }
    if (miner_acc) miner_acc->balance += chain_state->block_reward;
    
    // 6. Cleanup confirmed txs from mempool
    remove_confirmed_transactions(new_block->transactions, new_block->transaction_count);
    
    return new_block;
}

Block* mine_pool(const char *pool_id, int miners, double *hash_contributions) {
    // Highly similar to solo mine, but distributes reward proportionally based on contributions
    Block* block = mine_solo(pool_id);
    if (block) {
        // deduct full reward from pool account (was deposited by mine_solo) and distribute
        Account* pool_acc = get_account(pool_id);
        if (pool_acc) {
            pool_acc->balance -= chain_state->block_reward;
            
            for (int i = 0; i < miners; i++) {
                uint64_t share = (uint64_t)(chain_state->block_reward * hash_contributions[i]);
                char miner_addr[135];
                sprintf(miner_addr, "pool_miner_%d", i);
                
                Account* m_acc = get_account(miner_addr);
                if (!m_acc) {
                    m_acc = create_account(miner_addr, 0);
                }
                if (m_acc) m_acc->balance += share;
            }
        }
    }
    return block;
}

void retarget_difficulty(Block *last_10_blocks, ChainState *state) {
    time_t total_time = 0;
    for (int i = 1; i < 10; i++) {
        total_time += (last_10_blocks[i].timestamp - last_10_blocks[i-1].timestamp);
    }
    double average_time = total_time / 10.0;
    
    uint32_t old_diff = state->current_difficulty;
    if (average_time < 30.0) {
        state->current_difficulty++;
    } else if (average_time > 90.0) {
        if (state->current_difficulty > 1) {
            state->current_difficulty--;
        }
    }
    
    printf("Difficulty Retarget: Old Diff: %u, New Diff: %u, Avg Time: %.2f sec\n", old_diff, state->current_difficulty, average_time);
}
