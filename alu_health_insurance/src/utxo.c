#include "utxo.h"
#include <string.h>

static UTXO utxos[MAX_UTXOS];
static int utxo_count = 0;

void init_utxo() {
    utxo_count = 0;
    memset(utxos, 0, sizeof(utxos));
}

UTXO* create_utxo(const char *tx_id, const char *receiver, uint64_t amount) {
    if (utxo_count >= MAX_UTXOS) return NULL;
    
    strcpy(utxos[utxo_count].transaction_id, tx_id);
    strcpy(utxos[utxo_count].receiver_address, receiver);
    utxos[utxo_count].amount = amount;
    utxos[utxo_count].is_spent = 0;
    
    utxo_count++;
    return &utxos[utxo_count - 1];
}

UTXO* get_unspent_utxo(const char *tx_id, const char *receiver) {
    for (int i = 0; i < utxo_count; i++) {
        if (utxos[i].is_spent == 0 &&
            strcmp(utxos[i].transaction_id, tx_id) == 0 &&
            strcmp(utxos[i].receiver_address, receiver) == 0) {
            return &utxos[i];
        }
    }
    return NULL;
}

int spend_utxo(const char *tx_id, const char *receiver) {
    UTXO *u = get_unspent_utxo(tx_id, receiver);
    if (!u) return 0; // Not found or already spent (Double spend)
    u->is_spent = 1;
    return 1;
}
