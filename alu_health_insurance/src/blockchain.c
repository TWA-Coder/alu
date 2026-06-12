#include "blockchain.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void hash_transaction_fields(const Transaction *tx, unsigned char *hash_out) {
    // Serialize fields
    // Assuming a straightforward serialization buffer
    char buffer[1024];
    int len = snprintf(buffer, sizeof(buffer), "%s%s%s%llu%d%lld%u", 
                        tx->transaction_id, 
                        tx->sender_address, 
                        tx->receiver_address, 
                        (unsigned long long)tx->amount, 
                        tx->transaction_type, 
                        (long long)tx->timestamp, 
                        tx->sender_nonce);
                        
    compute_sha256((unsigned char*)buffer, len, hash_out);
}

void compute_merkle_root(Transaction *txs, uint32_t count, char *merkle_root_hex) {
    if (count == 0) {
        memset(merkle_root_hex, '0', SHA256_HEX_LENGTH - 1);
        merkle_root_hex[SHA256_HEX_LENGTH - 1] = '\0';
        return;
    }

    // Leaf hashes
    unsigned char (*hashes)[SHA256_DIGEST_LENGTH] = malloc(count * SHA256_DIGEST_LENGTH);
    for (uint32_t i = 0; i < count; i++) {
        hash_transaction_fields(&txs[i], hashes[i]);
    }

    uint32_t num_hashes = count;
    while (num_hashes > 1) {
        uint32_t new_num_hashes = (num_hashes + 1) / 2;
        unsigned char (*next_hashes)[SHA256_DIGEST_LENGTH] = malloc(new_num_hashes * SHA256_DIGEST_LENGTH);
        
        for (uint32_t i = 0; i < new_num_hashes; i++) {
            unsigned char pair[SHA256_DIGEST_LENGTH * 2];
            memcpy(pair, hashes[i*2], SHA256_DIGEST_LENGTH);
            
            if (i*2 + 1 < num_hashes) {
                memcpy(pair + SHA256_DIGEST_LENGTH, hashes[i*2 + 1], SHA256_DIGEST_LENGTH);
            } else {
                memcpy(pair + SHA256_DIGEST_LENGTH, hashes[i*2], SHA256_DIGEST_LENGTH); // Duplicate last
            }
            compute_sha256(pair, SHA256_DIGEST_LENGTH * 2, next_hashes[i]);
        }
        free(hashes);
        hashes = next_hashes;
        num_hashes = new_num_hashes;
    }
    
    // Convert to hex
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(merkle_root_hex + (i * 2), "%02x", hashes[0][i]);
    }
    merkle_root_hex[SHA256_HEX_LENGTH - 1] = '\0';
    free(hashes);
}
