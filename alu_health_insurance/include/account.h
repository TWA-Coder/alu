#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <stdint.h>
#define MAX_ACCOUNTS 1000

typedef struct {
    char address[135];
    uint64_t balance;
    uint32_t nonce;
} Account;

void init_accounts();
Account* get_account(const char *address);
Account* create_account(const char *address, uint64_t initial_balance);
int transfer_balance(const char *sender, const char *receiver, uint64_t amount);
int consume_nonce(const char *sender, uint32_t incoming_nonce);

#endif
