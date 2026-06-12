#include "account.h"
#include <string.h>

static Account accounts[MAX_ACCOUNTS];
static int account_count = 0;

void init_accounts() {
    account_count = 0;
    memset(accounts, 0, sizeof(accounts));
}

Account* get_account(const char *address) {
    for (int i = 0; i < account_count; i++) {
        if (strcmp(accounts[i].address, address) == 0) {
            return &accounts[i];
        }
    }
    return NULL;
}

Account* create_account(const char *address, uint64_t initial_balance) {
    if (account_count >= MAX_ACCOUNTS) return NULL;
    if (get_account(address) != NULL) return get_account(address);

    strcpy(accounts[account_count].address, address);
    accounts[account_count].balance = initial_balance;
    accounts[account_count].nonce = 0; // Starts at 0
    account_count++;
    
    return &accounts[account_count - 1];
}

int transfer_balance(const char *sender, const char *receiver, uint64_t amount) {
    Account *s = get_account(sender);
    Account *r = get_account(receiver);
    if (!s || !r) return 0;
    if (s->balance < amount) return 0;
    
    s->balance -= amount;
    r->balance += amount;
    return 1;
}

int consume_nonce(const char *sender, uint32_t incoming_nonce) {
    Account *s = get_account(sender);
    if (!s) return 0;
    // sender_nonce equals account's current nonce + 1? Wait, requirement:
    // "The network validates that the incoming sender_nonce equals the account's current nonce plus 1."
    // Oh, so if account is 0, tx must send 1.
    // "an integer starting at 0 that increments by 1... validated that sender_nonce equals current nonce plus 1"
    if (incoming_nonce == (s->nonce + 1)) {
        s->nonce++; // increment on success
        return 1;
    }
    return 0; // Invalid nonce, replay attack!
}
