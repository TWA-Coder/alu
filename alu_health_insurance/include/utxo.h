#ifndef UTXO_H
#define UTXO_H

#include <stdint.h>
#include "crypto_utils.h"

#define MAX_UTXOS 5000

typedef struct {
    char transaction_id[SHA256_HEX_LENGTH];
    char receiver_address[135];
    uint64_t amount;
    int is_spent;
} UTXO;

void init_utxo();
UTXO* create_utxo(const char *tx_id, const char *receiver, uint64_t amount);
UTXO* get_unspent_utxo(const char *tx_id, const char *receiver);
int spend_utxo(const char *tx_id, const char *receiver);

#endif
