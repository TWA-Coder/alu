#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdint.h>
#include <time.h>
#include "crypto_utils.h"

// Define transaction types as per requirements
typedef enum {
    TX_POLICY_ENROLLMENT,
    TX_PREMIUM_PAYMENT,
    TX_REINSURANCE_CONTRIBUTION,
    TX_SERVICE_REQUEST,
    TX_PREAUTH_REQUEST,
    TX_PREAUTH_APPROVE,
    TX_CLAIM_SUBMISSION,
    TX_CLAIM_APPROVAL,
    TX_CLAIM_REJECTION,
    TX_CLAIM_SETTLEMENT,
    TX_TOKEN_TRANSFER
} TransactionType;

typedef struct {
    char transaction_id[SHA256_HEX_LENGTH];
    char sender_address[135];   // Hex representation of compressed EC public key
    char receiver_address[135];
    uint64_t amount;
    TransactionType transaction_type;
    time_t timestamp;
    uint32_t sender_nonce; // Snapshot of sender's account nonce
    unsigned char digital_signature[MAX_SIGNATURE_LENGTH];
    unsigned int signature_len;
} Transaction;

#endif // TRANSACTION_H
