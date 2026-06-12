#include "insurance_logic.h"
#include "account.h"
#include "mempool.h"
#include "transaction.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

static Policy policies[MAX_POLICIES];
static int policy_count = 0;

void init_insurance_state() {
    policy_count = 0;
    memset(policies, 0, sizeof(policies));
}

int enroll_policy(const char *member_id, const char *policy_id, const char *plan) {
    if (policy_count >= MAX_POLICIES) return 0;
    
    strcpy(policies[policy_count].member_id, member_id);
    strcpy(policies[policy_count].policy_id, policy_id);
    strcpy(policies[policy_count].coverage_plan, plan);
    
    policies[policy_count].enrollment_date = time(NULL);
    // 365 days in seconds
    policies[policy_count].expiry_date = policies[policy_count].enrollment_date + (365 * 24 * 60 * 60);
    policies[policy_count].status = POLICY_ACTIVE;
    
    policy_count++;
    return 1;
}

Policy* get_policy(const char *policy_id) {
    for (int i = 0; i < policy_count; i++) {
        if (strcmp(policies[i].policy_id, policy_id) == 0) {
            // Check dynamic expiry
            if (policies[i].expiry_date < time(NULL) && policies[i].status == POLICY_ACTIVE) {
                policies[i].status = POLICY_EXPIRED;
            }
            return &policies[i];
        }
    }
    return NULL;
}

int renew_policy(const char *policy_id) {
    Policy *p = get_policy(policy_id);
    if (!p) return 0;
    
    p->expiry_date = time(NULL) + (365 * 24 * 60 * 60);
    p->status = POLICY_RENEWED;
    return 1;
}

int pay_premium(const char *sender, uint64_t amount, EC_KEY *sender_key) {
    Transaction tx1, tx2;
    memset(&tx1, 0, sizeof(Transaction));
    memset(&tx2, 0, sizeof(Transaction));
    
    Account *acc = get_account(sender);
    if (!acc || acc->balance < amount) return 0;
    
    uint64_t reinsurance_amount = amount * 0.05; // 5%
    uint64_t insurance_amount = amount - reinsurance_amount;
    
    sprintf(tx1.transaction_id, "PREM_%u", (unsigned int)rand());
    strcpy(tx1.sender_address, sender);
    strcpy(tx1.receiver_address, "InsurancePoolWallet");
    tx1.amount = insurance_amount; 
    tx1.transaction_type = TX_PREMIUM_PAYMENT;
    tx1.timestamp = time(NULL);
    tx1.sender_nonce = acc->nonce; // First transaction takes the exact current nonce
    
    ec_sign_data((unsigned char*)tx1.transaction_id, strlen(tx1.transaction_id), sender_key, tx1.digital_signature, &tx1.signature_len);
    
    sprintf(tx2.transaction_id, "REINS_%u", (unsigned int)rand());
    strcpy(tx2.sender_address, sender);
    strcpy(tx2.receiver_address, "ReinsurancePoolWallet");
    tx2.amount = reinsurance_amount; 
    tx2.transaction_type = TX_REINSURANCE_CONTRIBUTION;
    tx2.timestamp = time(NULL);
    tx2.sender_nonce = acc->nonce + 1; // Since it's batched sequentially
    
    ec_sign_data((unsigned char*)tx2.transaction_id, strlen(tx2.transaction_id), sender_key, tx2.digital_signature, &tx2.signature_len);
    
    add_to_mempool(&tx1, 10, STATUS_PENDING);
    add_to_mempool(&tx2, 10, STATUS_PENDING);
    
    return 1;
}

int check_fraud_heuristics(const char *provider_id, const char *member_id, uint64_t amount) {
    // Basic Stub for amount fraud rule
    // "Abnormal claim amount: exceeds 2 times historical average"
    // Since history tracking requires iterating all claims, we'll implement a static >5000 AHT suspicion here.
    if (amount > 5000) return 1;
    
    // Implement duplicate and high-frequency in advanced versions
    return 0;
}

int submit_claim(const char *provider_id, const char *policy_id, uint64_t amount, EC_KEY *provider_key) {
    Policy *p = get_policy(policy_id);
    if (!p) return 0;
    
    if (p->status == POLICY_EXPIRED) {
        printf("Policy EXPIRED. Claim rejected immediately.\n");
        return 0;
    }
    
    Account *acc = get_account(provider_id);
    uint32_t nonce = acc ? acc->nonce : 0;
    
    Transaction tx;
    memset(&tx, 0, sizeof(Transaction));
    sprintf(tx.transaction_id, "CLAIM_SUB_%u", (unsigned int)rand());
    strcpy(tx.sender_address, provider_id);
    strcpy(tx.receiver_address, "InsurancePoolWallet"); 
    tx.amount = amount;
    tx.transaction_type = TX_CLAIM_SUBMISSION;
    tx.timestamp = time(NULL);
    tx.sender_nonce = nonce;
    
    ec_sign_data((unsigned char*)tx.transaction_id, strlen(tx.transaction_id), provider_key, tx.digital_signature, &tx.signature_len);
    
    MempoolStatus status = STATUS_PENDING;
    if (check_fraud_heuristics(provider_id, p->member_id, amount)) {
        status = STATUS_SUSPICIOUS;
    }
    
    return add_to_mempool(&tx, 5, status); // Claim submissions get base priority
}

int settle_claim(const char *provider_id, uint64_t approved_amount, EC_KEY *insurance_key, EC_KEY *reinsurance_key) {
    // Insurance pool pays first 1,000 AHT.
    // If over 1,000 AHT, reinsurance pool covers remainder in a split transaction!
    
    Account *ins_acc = get_account("InsurancePoolWallet");
    if (!ins_acc) return 0;

    Transaction tx1;
    memset(&tx1, 0, sizeof(Transaction));
    sprintf(tx1.transaction_id, "SETTLE_INS_%u", (unsigned int)rand());
    strcpy(tx1.sender_address, "InsurancePoolWallet");
    strcpy(tx1.receiver_address, provider_id);
    tx1.amount = (approved_amount > 1000) ? 1000 : approved_amount;
    tx1.transaction_type = TX_CLAIM_SETTLEMENT;
    tx1.timestamp = time(NULL);
    tx1.sender_nonce = ins_acc->nonce;

    ec_sign_data((unsigned char*)tx1.transaction_id, strlen(tx1.transaction_id), insurance_key, tx1.digital_signature, &tx1.signature_len);
    add_to_mempool(&tx1, 20, STATUS_PENDING); // Priority fee manually set high
    
    if (approved_amount > 1000) {
        Account *reins_acc = get_account("ReinsurancePoolWallet");
        if (reins_acc && reins_acc->balance >= (approved_amount - 1000)) {
            Transaction tx2;
            memset(&tx2, 0, sizeof(Transaction));
            sprintf(tx2.transaction_id, "SETTLE_REINS_%u", (unsigned int)rand());
            strcpy(tx2.sender_address, "ReinsurancePoolWallet");
            strcpy(tx2.receiver_address, provider_id);
            tx2.amount = approved_amount - 1000;
            tx2.transaction_type = TX_CLAIM_SETTLEMENT;
            tx2.timestamp = time(NULL);
            tx2.sender_nonce = reins_acc->nonce;
            
            ec_sign_data((unsigned char*)tx2.transaction_id, strlen(tx2.transaction_id), reinsurance_key, tx2.digital_signature, &tx2.signature_len);
            add_to_mempool(&tx2, 20, STATUS_PENDING);
        } else {
            printf("ReinsurancePoolWallet insufficient funds. Requires manual intervention.\n");
            // Partial approval triggers
        }
    }
    
    return 1;
}
