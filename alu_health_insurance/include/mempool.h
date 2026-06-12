#ifndef MEMPOOL_H
#define MEMPOOL_H

#include "transaction.h"

#define MAX_MEMPOOL_SIZE 5000

typedef enum {
    STATUS_PENDING,
    STATUS_CONFIRMED,
    STATUS_SUSPICIOUS
} MempoolStatus;

typedef struct {
    Transaction tx;
    uint32_t fee;
    MempoolStatus status;
} MempoolEntry;

void init_mempool();
int add_to_mempool(const Transaction *tx, uint32_t fee, MempoolStatus initial_status);
void sort_mempool_priority();
MempoolEntry* get_mempool_entries(int *count_out);
void remove_confirmed_transactions(Transaction *confirmed_txs, uint32_t count);
int mark_suspicious(const char *tx_id);
int update_mempool_status(const char *tx_id, MempoolStatus new_status);

#endif
