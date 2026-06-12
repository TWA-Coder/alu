#ifndef INSURANCE_LOGIC_H
#define INSURANCE_LOGIC_H

#include "transaction.h"
#include "crypto_utils.h"
#include <time.h>

#define MAX_POLICIES 5000

typedef enum {
    POLICY_ACTIVE,
    POLICY_EXPIRED,
    POLICY_RENEWED
} PolicyStatus;

typedef struct {
    char member_id[135];
    char policy_id[64];
    char coverage_plan[32];
    time_t enrollment_date;
    time_t expiry_date;
    PolicyStatus status;
} Policy;

void init_insurance_state();
int enroll_policy(const char *member_id, const char *policy_id, const char *plan);
Policy* get_policy(const char *policy_id);
int renew_policy(const char *policy_id);

int pay_premium(const char *sender, uint64_t amount, EC_KEY *sender_key);
int submit_claim(const char *provider_id, const char *policy_id, uint64_t amount, EC_KEY *provider_key);
int settle_claim(const char *provider_id, uint64_t approved_amount, EC_KEY *insurance_key, EC_KEY *reinsurance_key);

// Fraud checks based on requirements
int check_fraud_heuristics(const char *provider_id, const char *member_id, uint64_t amount);

#endif
