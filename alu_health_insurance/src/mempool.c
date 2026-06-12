#include "mempool.h"
#include <string.h>
#include <stdlib.h>

static MempoolEntry mempool[MAX_MEMPOOL_SIZE];
static int mempool_count = 0;

void init_mempool() {
    mempool_count = 0;
    memset(mempool, 0, sizeof(mempool));
}

int add_to_mempool(const Transaction *tx, uint32_t fee, MempoolStatus initial_status) {
    if (mempool_count >= MAX_MEMPOOL_SIZE) return 0;
    
    // Check for duplicates
    for (int i = 0; i < mempool_count; i++) {
        if (strcmp(mempool[i].tx.transaction_id, tx->transaction_id) == 0) {
            return 0; // duplicate
        }
    }

    mempool[mempool_count].tx = *tx;
    mempool[mempool_count].fee = fee;
    mempool[mempool_count].status = initial_status;
    mempool_count++;
    return 1;
}

// Qsort comparator: Fee descending, then timestamp ascending
static int compare_priority(const void *a, const void *b) {
    const MempoolEntry *entryA = (const MempoolEntry *)a;
    const MempoolEntry *entryB = (const MempoolEntry *)b;
    
    if (entryA->fee != entryB->fee) {
        return (entryB->fee > entryA->fee) ? 1 : -1; // descending fee
    }
    
    if (entryA->tx.timestamp != entryB->tx.timestamp) {
        return (entryA->tx.timestamp > entryB->tx.timestamp) ? 1 : -1; // ascending timestamp
    }
    
    return 0;
}

void sort_mempool_priority() {
    qsort(mempool, mempool_count, sizeof(MempoolEntry), compare_priority);
}

MempoolEntry* get_mempool_entries(int *count_out) {
    if (count_out) *count_out = mempool_count;
    return mempool;
}

void remove_confirmed_transactions(Transaction *confirmed_txs, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        for (int j = 0; j < mempool_count; j++) {
            if (strcmp(mempool[j].tx.transaction_id, confirmed_txs[i].transaction_id) == 0) {
                // Remove by shifting
                for (int k = j; k < mempool_count - 1; k++) {
                    mempool[k] = mempool[k + 1];
                }
                mempool_count--;
                j--; // check the new element at index j
                break;
            }
        }
    }
}

int mark_suspicious(const char *tx_id) {
    return update_mempool_status(tx_id, STATUS_SUSPICIOUS);
}

int update_mempool_status(const char *tx_id, MempoolStatus new_status) {
    for (int i = 0; i < mempool_count; i++) {
        if (strcmp(mempool[i].tx.transaction_id, tx_id) == 0) {
            mempool[i].status = new_status;
            return 1;
        }
    }
    return 0;
}
